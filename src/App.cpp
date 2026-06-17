#include "App.h"
#include "Logic.h"
#include "Renderer.h"
#include "external/imgui/imgui.h"
#include "imgui.h"
#include "raylib.h"

#include <filesystem>
#include <fstream>
#include <iostream>
#include <math.h>
#include <regex>
#include <sys/types.h>

import Component;
import Button;

// Helper function to find the safe OS directory
std::filesystem::path App::getAppSaveDirectory() {
  const char *homeDir = nullptr;

  // Ask the OS for the user's home profile securely
#ifdef _WIN32
  homeDir = std::getenv("USERPROFILE"); // Windows: C:\Users\Username
#else
  homeDir = std::getenv("HOME"); // Linux/Mac: /home/username
#endif

  std::filesystem::path savePath;

  if (homeDir) {
    // Build the path: C:\Users\Username\Documents\DCDsimulator
    savePath = std::filesystem::path(homeDir) / "Documents" / "DCDsimulator";
  } else {
    // Fallback just in case the OS is being weird
    savePath = std::filesystem::current_path() / "DCD_Saves";
  }

  // If the folder doesn't exist yet, this creates it automatically
  if (!std::filesystem::exists(savePath)) {
    std::filesystem::create_directories(savePath);
  }

  return savePath;
}

App::App() {
  InitWindow(screenWidth, screenHeight, "DCDsimulator");
  SetTargetFPS(60);

  rlImGuiSetup(true);

  // Font (default one)
  Font GetFontDefault(void);

  // camera initialization settings
  camera.zoom = 1.0f;
  camera.rotation = 0.0f;
  camera.target = (Vector2){0.0f, 0.0f};

  // Set the offset to the exact center of the screen
  camera.offset = (Vector2){screenWidth / 2.0f, screenHeight / 2.0f};

  isPlacingComponent = false;
  isDrawingWire = false;
  isColliding = false;

  simulationThread = std::thread(&App::simulationLoop, this);
}

float SCALE_FACTOR = 1.0f;
App::~App() {
  isRunning = false;
  isThreadRunning = false;
  if (simulationThread.joinable()) {
    simulationThread.join();
  }

  rlImGuiShutdown();
  CloseWindow();
}

void App::drawUI() {
  rlImGuiBegin();

  // Saving/loading menu
  ImGui::BeginMainMenuBar();
  if (ImGui::BeginMenu("File")) {

    if (ImGui::MenuItem("Open...")) {
      showLoadPopup = true;
      refreshSaveList(); // <--- ADD THIS HERE!
      // Clear the text box so previous searches don't carry over
      memset(filenameBuffer, 0, sizeof(filenameBuffer));
    }

    if (ImGui::MenuItem("Save As...")) {
      showSavePopup = true;
    }
    ImGui::EndMenu();
  }
  const char *toolText = "Select";
  ImVec4 toolColor = ImVec4(0.2f, 0.6f, 1.0f, 1.0f); // Blue

  // Check which tool is actively selected
  if (currentTool == Tool::Wire) {
    toolText = "Wire";
    toolColor = ImVec4(0.8f, 0.6f, 0.2f, 1.0f); // Orange
  } else if (currentTool == Tool::Move) {
    toolText = "Select";
    toolColor = ImVec4(0.2f, 0.6f, 1.0f, 1.0f); // Blue
  }

  // Calculate the exact pixel width of our text to perfectly center it
  std::string prefix = "Active Tool: ";
  float textWidth = ImGui::CalcTextSize((prefix + toolText).c_str()).x;

  // Push the cursor to the exact middle of the screen
  ImGui::SetCursorPosX((ImGui::GetWindowWidth() - textWidth) * 0.5f);

  ImGui::Text("%s", prefix.c_str());
  ImGui::SameLine(0, 0);
  ImGui::TextColored(toolColor, "%s", toolText);

  ImGui::EndMainMenuBar();

  // Trigger the actual popups if the menu items were clicked
  if (showSavePopup) {
    ImGui::OpenPopup("Save Circuit");
    showSavePopup = false;
  }
  if (showLoadPopup) {
    ImGui::OpenPopup("Load Circuit");
    showLoadPopup = false;
  }

  // --- SAVE POPUP ---
  if (ImGui::BeginPopupModal("Save Circuit", NULL,
                             ImGuiWindowFlags_AlwaysAutoResize)) {
    ImGui::Text("Enter file name (will save as .dcd):");
    ImGui::InputText("##savefile", filenameBuffer,
                     IM_ARRAYSIZE(filenameBuffer));
    if (ImGui::Button("Save", ImVec2(120, 0))) {
      saveToFile(filenameBuffer);
      ImGui::CloseCurrentPopup();
    }
    ImGui::SameLine();
    if (ImGui::Button("Cancel", ImVec2(120, 0))) {
      ImGui::CloseCurrentPopup();
    }
    ImGui::EndPopup();
  }

  // --- LOAD POPUP ---
  ImGui::SetNextWindowSize(ImVec2(400, 0), ImGuiCond_Always);
  if (ImGui::BeginPopupModal("Load Circuit", NULL, ImGuiWindowFlags_NoResize)) {

    ImGui::Text("Search or enter file name:");

    // The Search Bar
    ImGui::SetNextItemWidth(-1);
    ImGui::InputText("##loadfile", filenameBuffer,
                     IM_ARRAYSIZE(filenameBuffer));

    ImGui::Spacing();
    ImGui::Text("Saved Circuits:");

    // The Scrolling Window
    ImGui::BeginChild("SaveList", ImVec2(0, 150), true,
                      ImGuiWindowFlags_HorizontalScrollbar);

    std::string searchQuery = filenameBuffer;

    for (const auto &saveName : availableSaves) {
      if (searchQuery.empty() ||
          saveName.find(searchQuery) != std::string::npos) {
        if (ImGui::Selectable(saveName.c_str())) {
          snprintf(filenameBuffer, sizeof(filenameBuffer), "%s",
                   saveName.c_str());
        }
      }
    }
    ImGui::EndChild();

    ImGui::Spacing();

    // Load and Cancel Buttons
    if (ImGui::Button("Load", ImVec2(120, 0))) {
      loadFromFile(filenameBuffer);
      ImGui::CloseCurrentPopup();
    }
    ImGui::SameLine();
    if (ImGui::Button("Cancel", ImVec2(120, 0))) {
      ImGui::CloseCurrentPopup();
    }

    ImGui::EndPopup();
  }
  // End of saving/loading menu

  // Stick to the left side, full height
  ImGui::SetNextWindowPos(ImVec2(0, 20), ImGuiCond_Always);
  ImGui::SetNextWindowSize(ImVec2(200, (float)screenHeight), ImGuiCond_Always);
  ImGuiWindowFlags windowFlags =
      ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove |
      ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse;

  bool disableMenu = (isPlacingComponent || isDrawingWire);

  // TOOLBOX ELEMENTS BELOW
  ImGui::Begin("Toolbox", nullptr, windowFlags);

  // Use the stored state for BeginDisabled
  if (disableMenu)
    ImGui::BeginDisabled();

  ImGui::Text("Tools");
  ImGui::Separator();
  ImGui::Spacing();

  // LOCK_GUARD safety scope
  {
    std::lock_guard<std::mutex> lock(circuitMutex);

    if (ImGui::Button("Wire", ImVec2(-1, 30))) {
      currentTool = Tool::Wire;
    }
    if (ImGui::Button("Select", ImVec2(-1, 30))) {
      currentTool = Tool::Move;
    }

    ImGui::Text("Logic Gates");
    ImGui::Separator();
    ImGui::Spacing();

    // LIST OF THE COMPONENTS AVAILABLE

    if (ImGui::Button("AND Gate", ImVec2(-1, 30))) {
      isPlacingComponent = true;
      logic.addANDGate();
    }
    if (ImGui::Button("OR Gate", ImVec2(-1, 30))) {
      isPlacingComponent = true;
      logic.addORGate();
    }
    if (ImGui::Button("NOT Gate", ImVec2(-1, 30))) {
      isPlacingComponent = true;
      logic.addNOTGate();
    }
    if (ImGui::Button("NAND Gate", ImVec2(-1, 30))) {
      isPlacingComponent = true;
      logic.addNANDGate();
    }
    if (ImGui::Button("NOR Gate", ImVec2(-1, 30))) {
      isPlacingComponent = true;
      logic.addNORGate();
    }

    ImGui::Text("Other");
    ImGui::Separator();
    ImGui::Spacing();

    if (ImGui::Button("Button", ImVec2(-1, 30))) {
      isPlacingComponent = true;
      logic.addButton();
    }
    if (ImGui::Button("Bulb", ImVec2(-1, 30))) {
      isPlacingComponent = true;
      logic.addBulb();
    }

    // Use the exact same stored state for EndDisabled
    if (disableMenu)
      ImGui::EndDisabled();

    ImGui::Spacing();
    ImGui::Separator();

    // Status text
    ImGui::Text("Current State:");
    if (isPlacingComponent) {
      ImGui::TextColored(ImVec4(0.2f, 0.8f, 0.2f, 1.0f),
                         "Placing Components...");

      if (ImGui::Button("Cancel", ImVec2(-1, 30))) {
        isPlacingComponent = false;

        logic.removeComponentByID(logic.getLastUsedID());

        activeComponent = nullptr;
      }

    } else if (isDrawingWire) {
      ImGui::TextColored(ImVec4(0.8f, 0.6f, 0.2f, 1.0f), "Drawing Wire...");

    } else {
      ImGui::TextColored(ImVec4(0.5f, 0.5f, 0.5f, 1.0f), "Idle");
    }
  }

  ImGui::End();
  rlImGuiEnd();
}

void App::draw() {

  BeginMode2D(camera);
  // drawing below
  // -------------

  Vector2 topLeft = GetScreenToWorld2D((Vector2){0, 0}, camera);
  Vector2 bottomRight = GetScreenToWorld2D(
      (Vector2){(float)GetScreenWidth(), (float)GetScreenHeight()}, camera);

  // GRID
  render.drawGrid(topLeft, bottomRight);
  // LOCK_GUARD safety scope
  {
    std::lock_guard<std::mutex> lock(circuitMutex);
    // DRAWING CURRENT WIRE
    if (isDrawingWire)
      render.drawWire(activeDrawingWire.startPos.x,
                      activeDrawingWire.startPos.y, activeDrawingWire.endPos.x,
                      activeDrawingWire.endPos.y);
    // DRAWING WIRES
    for (auto &wire : visualWires) {
      render.drawWire(wire.startPos.x, wire.startPos.y, wire.endPos.x,
                      wire.endPos.y);
    }

    for (auto &component : logic.getComponents()) {
      std::string type = component->get_type();
      if (type == "NOT_gate") {

        render.drawNOTGate(component->x, component->y,
                           component->get_output_from(0));
        component->set_hitbox(component->x, component->y, component->x + 30.0f,
                              component->y + 20.0f);
      } else if (type == "AND_gate") {

        render.drawANDGate(component->x, component->y);
        component->set_hitbox(component->x, component->y, component->x + 40.0f,
                              component->y + 40.0f);

      } else if (type == "NAND_gate") {

        render.drawNANDGate(component->x, component->y,
                            component->get_output_from(0));
        component->set_hitbox(component->x, component->y, component->x + 50.0f,
                              component->y + 40.0f);
      } else if (type == "OR_gate") {
        render.drawORGate(component->x, component->y);
        component->set_hitbox(component->x, component->y, component->x + 40.0f,
                              component->y + 40.0f);
      } else if (type == "NOR_gate") {
        render.drawNORGate(component->x, component->y,
                           component->get_output_from(0));
        component->set_hitbox(component->x, component->y, component->x + 50.0f,
                              component->y + 40.0f);
      } else if (type == "Bulb") {
        render.drawBulb(component->x, component->y,
                        component->get_output_from(0));
        component->set_hitbox(component->x - 10.0f, component->y - 10.0f,
                              component->x + 10.0f, component->y + 10.0f);
      } else if (type == "Button") {
        render.drawButton(component->x, component->y,
                          component->get_output_from(0));
        component->set_hitbox(component->x, component->y, component->x + 19.0f,
                              component->y + 19.0f);
      }

      // DRAWING INDIVIDUAL PINS
      for (auto pin : component->get_input_pins()) {
        if (component->get_type() != "Junction")
          render.drawPin(component->x + pin.offsetX, component->y + pin.offsetY,
                         pin.isHovered);
      }
      for (auto pin : component->get_output_pins()) {
        render.drawPin(component->x + pin.offsetX, component->y + pin.offsetY,
                       pin.isHovered);
      }
      //  FIX: for DEBUGGING
      // render.drawHitBox(component);
    }

    if (activeComponent && currentTool == Tool::Move) {
      render.drawSelection(activeComponent);
    }
  }
  //---------
  EndMode2D();

  // Circular Menu
  drawRadialMenu();
}

void App::update() {
  // useful variables
  Vector2 mouseWorldPos = GetScreenToWorld2D(GetMousePosition(), camera);
  float snappedX = floor(mouseWorldPos.x / render.getCellSizeSmall()) *
                   render.getCellSizeSmall();
  float snappedY = ceil(mouseWorldPos.y / render.getCellSizeSmall()) *
                   render.getCellSizeSmall();
  Vector2 snappedMousePos = (Vector2){snappedX, snappedY};

  // camera variables
  float maxZoom = 3.0f;
  float minZoom = 0.6f;

  isMouseOverUI = ImGui::GetIO().WantCaptureMouse;

  updateRadialMenu();

  // 2. CRITICAL: If the menu is open, completely skip the rest of the update
  // loop! This stops the camera from zooming and stops wires from drawing.
  if (activeRadialMenu != RadialMenuType::NONE)
    return;
  // ==========================================

  // LOCK_GUARD safety scope
  {
    std::lock_guard<std::mutex> lock(circuitMutex);

    // Hovering over a Pin
    pendingSourcePin = -1;
    pendingTargetPin = -1;

    for (auto &comp : logic.getComponents()) {
      // Check Input Pins
      for (auto &inPin : comp->get_input_pins()) {
        if (comp->x + inPin.offsetX == snappedMousePos.x &&
            comp->y + inPin.offsetY == snappedMousePos.y) {

          inPin.isHovered = 1;

          // If we are already drawing, this pin is a target. Otherwise, it's
          // a source.
          if (isDrawingWire) {
            pendingTargetPin = inPin.pin_number;
            activeTargetComponent = comp.get();
          }
          // else {
          //   pendingSourcePin = inPin.pin_number;
          //   activeComponent = comp.get();
          // }  // --> commented out so to not draw from input pins. (due to
          // the connection order)
        } else {
          inPin.isHovered = 0;
        }
      }

      // Check Output Pins
      for (auto &outPin : comp->get_output_pins()) {
        if (comp->x + outPin.offsetX == snappedMousePos.x &&
            comp->y + outPin.offsetY == snappedMousePos.y) {

          outPin.isHovered = 1;

          if (isDrawingWire) {
            pendingTargetPin = outPin.pin_number;
            activeTargetComponent = comp.get();
          } else {
            pendingSourcePin = outPin.pin_number;
            activeComponent = comp.get();
          }
        } else {
          outPin.isHovered = 0;
        }
      }
    }

    // Currently selected one
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && currentTool == Tool::Move) {
      clickedSomething = false;
      for (auto &comp : logic.getComponents()) {
        Rectangle hitRec;
        hitRec.x = comp->hitbox.startX;
        hitRec.y = comp->hitbox.startY;
        hitRec.width = comp->hitbox.endX - comp->hitbox.startX;
        hitRec.height = comp->hitbox.endY - comp->hitbox.startY;

        if (CheckCollisionPointRec(mouseWorldPos, hitRec)) {

          activeComponent = comp.get();

          // SWITCHING the BUTTON
          if (activeComponent->get_type() == "Button") {
            Button *buttonPtr = dynamic_cast<Button *>(activeComponent);
            if (buttonPtr != nullptr)
              buttonPtr->switch_state();
          }

          clickedSomething = true;
          break;
        }
      }
      // Only clear the active component if didn't click the body,
      // nor hovering a pin, and not in the middle of drawing a wire.
      if (!clickedSomething && !isDrawingWire) {
        activeComponent = nullptr;
      }
    }

    // MOVING AROUND THE WORLD
    if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT)) {
      Vector2 delta = GetMouseDelta();
      // Move target opposite to the mouse drag, scaled by zoom
      camera.target.x -= delta.x / camera.zoom;
      camera.target.y -= delta.y / camera.zoom;
    }

    // code for drawing updating the wire drawing
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && !isPlacingComponent &&
        !isMouseOverUI && pendingSourcePin != -1 && currentTool == Tool::Wire) {
      isDrawingWire = true;
      savedSourcePin = pendingSourcePin;
      activeDrawingWire.startPos = (Vector2){snappedX, snappedY};

      logic.addJunction();

      // 1. SAVE THE ID IMMEDIATELY AFTER CREATING IT
      currentWireStartJunctionID = logic.getLastUsedID();

      auto junction = logic.getComponentByID(currentWireStartJunctionID);
      junction->x = snappedX;
      junction->y = snappedY;
      junction->set_hitbox(junction->x - 5.0f, junction->y - 5.0f,
                           junction->x + 5.0f, junction->y + 5.0f);

      logic.addLogicalConnection(activeComponent, savedSourcePin, junction, 0);
    }

    if (isDrawingWire) {
      if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
        activeDrawingWire.endPos = (Vector2){snappedX, snappedY};

      } else if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
        activeDrawingWire.endPos = (Vector2){snappedX, snappedY};

        isDrawingWire = false;
        visualWires.push_back(activeDrawingWire);

        // 3. saving the wire-begin junction
        auto sourceJunction =
            logic.getComponentByID(currentWireStartJunctionID);

        logic.addJunction();

        // saving the wire-end junction
        auto junction = logic.getComponentByID(logic.getLastUsedID());
        junction->x = snappedX;
        junction->y = snappedY;
        junction->set_hitbox(junction->x - 5.0f, junction->y - 5.0f,
                             junction->x + 5.0f, junction->y + 5.0f);

        // Component -> void (junction is created)
        if (pendingTargetPin == -1) {
          logic.addLogicalConnection(sourceJunction, 0, junction, 0);

        } else { // Component (junction) -> component (junction)
          logic.addLogicalConnection(sourceJunction, 0, junction, 0);
          logic.addLogicalConnection(junction, 0, activeTargetComponent,
                                     pendingTargetPin);
        }

        // 4. Reset the saved ID when finished
        currentWireStartJunctionID = -1;
      }
    }
    // camera zooming
    if (cameraZoom <= maxZoom && cameraZoom >= minZoom)
      cameraZoom += GetMouseWheelMove() * 0.1f;
    else if (cameraZoom > maxZoom)
      cameraZoom = maxZoom;
    else if (cameraZoom < minZoom)
      cameraZoom = minZoom;
    camera.zoom = cameraZoom;

    //--------------
    // PLACING GATES
    // -------------

    if (isPlacingComponent) {
      auto current = logic.getComponentByID(logic.getLastUsedID());
      if (current) {
        current->x = snappedX;
        current->y = snappedY;

        isColliding = false;

        for (auto &component : logic.getComponents()) {
          if (current->getID() != component->getID()) {
            if (current->hitbox.startX <= component->hitbox.endX &&
                current->hitbox.endX >= component->hitbox.startX &&
                current->hitbox.startY <= component->hitbox.endY &&
                current->hitbox.endY >= component->hitbox.startY) {
              isColliding = true;
              break;
            }
          }
        }
      }
    }

    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && isPlacingComponent &&
        !isMouseOverUI && !isColliding) {
      isPlacingComponent = false;

      // -- -AUTO - WIRING LOGIC-- -
      // Grabbing the component that was finished placing
      auto newComp = logic.getComponentByID(logic.getLastUsedID());

      if (newComp) {
        // 1. Check Input Pins: Look for Junctions feeding INTO this component
        for (auto &inPin : newComp->get_input_pins()) {
          float absolutePinX = newComp->x + inPin.offsetX;
          float absolutePinY = newComp->y + inPin.offsetY;

          for (auto &otherComp : logic.getComponents()) {
            if (otherComp->get_type() == "Junction" &&
                otherComp->x == absolutePinX && otherComp->y == absolutePinY) {
              // when a junction is found exactly on our input pin they are
              // connected
              logic.addLogicalConnection(otherComp.get(), 0, newComp,
                                         inPin.pin_number);
            }
          }
        }

        // 2. Check Output Pins: Look for Junctions receiving FROM this
        // component
        for (auto &outPin : newComp->get_output_pins()) {
          float absolutePinX = newComp->x + outPin.offsetX;
          float absolutePinY = newComp->y + outPin.offsetY;

          for (auto &otherComp : logic.getComponents()) {
            if (otherComp->get_type() == "Junction" &&
                otherComp->x == absolutePinX && otherComp->y == absolutePinY) {
              // connecting on the go. if we place a component on already
              // existing wires, they automatically connect
              logic.addLogicalConnection(newComp, outPin.pin_number,
                                         otherComp.get(), 0);
            }
          }
        }
      }
    }

    if (activeComponent && !isDrawingWire && IsKeyPressed(KEY_DELETE)) {

      if (activeComponent->get_type() == "Junction") {
        // Save the exact coordinates before deleting
        float targetX = activeComponent->x;
        float targetY = activeComponent->y;

        // Delete visual-wires connected to this exact coordinate
        for (auto it = visualWires.begin(); it != visualWires.end();) {
          if ((it->startPos.x == targetX && it->startPos.y == targetY) ||
              (it->endPos.x == targetX && it->endPos.y == targetY)) {
            it = visualWires.erase(it); // Erase from screen
          } else {
            ++it;
          }
        }

        // Gather all stacked Junctions at this exact coordinate
        std::vector<uint32_t> junctionsToDestroy;
        for (auto &comp : logic.getComponents()) {
          if (comp->get_type() == "Junction" && comp->x == targetX &&
              comp->y == targetY) {
            junctionsToDestroy.push_back(comp->getID());
          }
        }

        //  Destroy every junction in the stack
        for (uint32_t jID : junctionsToDestroy) {
          logic.removeComponentByID(jID);
        }

      } else {
        // removing normal placeable type components
        logic.removeComponentByID(activeComponent->getID());
      }

      // Clean up pointers
      activeComponent = nullptr;
      activeTargetComponent = nullptr;

      logic.cleanUpJunc();
    }

  } // lock_guard (gone)
}

void App::run() {
  while (!WindowShouldClose()) {

    update();

    BeginDrawing();

    ClearBackground(render.COLOR_OF_BACKGROUND);

    draw();
    drawUI();

    EndDrawing();
  }
}

void App::simulationLoop() {
  // Keeps running as long as the app is open
  while (isRunning) {
    {
      std::lock_guard<std::mutex> lock(circuitMutex);
      logic.computeCircuit();

    } // The lock is automatically released

    // 3. Sleep for ~16ms (approx 60 ticks per second) to yield the CPU
    std::this_thread::sleep_for(std::chrono::milliseconds(16));
  }
}

// SAVING TO FILES with .dcd extension

void App::saveToFile(const std::string &filename) {
  std::lock_guard<std::mutex> lock(
      circuitMutex); // CRITICAL: Stop simulation while saving

  // 1. Get the safe OS directory
  std::filesystem::path path = getAppSaveDirectory();

  // 2. Append the typed filename to it
  path /= filename;

  // 3. Force custom extension
  if (path.extension() != ".dcd") {
    path += ".dcd";
  }

  std::ofstream file(path);
  if (!file.is_open()) {
    std::cout << "Failed to open file for saving! Path: " << path << "\n";
    return;
  }

  // 1. Save Components
  for (auto &comp : logic.getComponents()) {
    file << "COMP " << comp->get_type() << " " << comp->getID() << " "
         << comp->x << " " << comp->y << "\n";
  }

  // 2. Save Connections
  for (auto &conn : logic.getConnections()) {
    file << "CONN " << conn->getSourceID() << " " << conn->getSourcePin() << " "
         << conn->getTargetID() << " " << conn->getTargetPin() << "\n";
  }

  // 3. Save Visual Wires
  for (auto &vw : visualWires) {
    file << "VWIRE " << vw.startPos.x << " " << vw.startPos.y << " "
         << vw.endPos.x << " " << vw.endPos.y << "\n";
  }

  file.close();
  std::cout << "Saved successfully to " << path << "\n";
}

void App::loadFromFile(const std::string &filename) {
  std::lock_guard<std::mutex> lock(
      circuitMutex); // Stopping simulation while loading

  // 1. Get the safe OS directory
  std::filesystem::path path = getAppSaveDirectory();

  // 2. Append the typed filename to it
  path /= filename;

  if (path.extension() != ".dcd") {
    path += ".dcd";
  }

  std::ifstream file(path);
  if (!file.is_open()) {
    std::cout << "Failed to open file for loading! Path: " << path << "\n";
    return;
  }

  // Clears current board completely
  logic.clearAll();
  visualWires.clear();
  activeComponent = nullptr;
  activeTargetComponent = nullptr;

  std::string line;
  uint32_t highestID = 0;

  // Regex patterns mapping exactly to the save format
  std::regex compRegex(R"(^COMP\s+(\w+)\s+(\d+)\s+([-\d.]+)\s+([-\d.]+)$)");
  std::regex connRegex(R"(^CONN\s+(\d+)\s+(\d+)\s+(\d+)\s+(\d+)$)");
  std::regex vwireRegex(
      R"(^VWIRE\s+([-\d.]+)\s+([-\d.]+)\s+([-\d.]+)\s+([-\d.]+)$)");
  std::smatch match;

  while (std::getline(file, line)) {
    if (std::regex_match(line, match, compRegex)) {
      std::string type = match[1].str();
      uint32_t id = std::stoul(match[2].str());
      float x = std::stof(match[3].str());
      float y = std::stof(match[4].str());

      Component *comp = logic.spawnComponentByType(type, id);
      if (comp) {
        comp->x = x;
        comp->y = y;

        // Update hitboxes immediately so they are clickable
        if (type == "Junction") {
          comp->set_hitbox(x - 5.0f, y - 5.0f, x + 5.0f, y + 5.0f);
        } else {
          // Provide a default fallback or replicate your App::draw logic
          comp->set_hitbox(x, y, x + 40.0f, y + 40.0f);
        }

        if (id > highestID)
          highestID = id;
      }
    } else if (std::regex_match(line, match, connRegex)) {
      uint32_t srcID = std::stoul(match[1].str());
      int srcPin = std::stoi(match[2].str());
      uint32_t tgtID = std::stoul(match[3].str());
      int tgtPin = std::stoi(match[4].str());

      Component *source = logic.getComponentByID(srcID);
      Component *target = logic.getComponentByID(tgtID);

      if (source && target) {
        logic.addLogicalConnection(source, srcPin, target, tgtPin);
      }
    } else if (std::regex_match(line, match, vwireRegex)) {
      Wire vw;
      vw.startPos.x = std::stof(match[1].str());
      vw.startPos.y = std::stof(match[2].str());
      vw.endPos.x = std::stof(match[3].str());
      vw.endPos.y = std::stof(match[4].str());
      visualWires.push_back(vw);
    }
  }

  // Ensuring  the next component placed has a fresh, unused ID
  logic.setNextComponentID(highestID);
  file.close();
}

void App::refreshSaveList() {
  availableSaves.clear();
  std::filesystem::path saveDir = getAppSaveDirectory();

  if (std::filesystem::exists(saveDir)) {
    // Looping through every file in the directory
    for (const auto &entry : std::filesystem::directory_iterator(saveDir)) {
      if (entry.is_regular_file() && entry.path().extension() == ".dcd") {
        // Adding the filename (e.g. "circuit1.dcd") to our list
        availableSaves.push_back(entry.path().filename().string());
      }
    }
  }
}

// --- CIRCULAR MENU ---
void App::updateRadialMenu() {
  // Triggering Tools Menu
  if (IsKeyPressed(KEY_TAB)) {
    activeRadialMenu = RadialMenuType::TOOLS;
    radialMenuPos = GetMousePosition();
    currentActivePage = &toolsPage;
  }

  //  Triggering Components Menu
  if (IsKeyPressed(KEY_SPACE)) {
    activeRadialMenu = RadialMenuType::COMPONENTS;
    radialMenuPos = GetMousePosition();
    currentActivePage = &componentPages[currentComponentPage];
  }

  if (activeRadialMenu != RadialMenuType::NONE) {

    // --- PAGEs LOGIC (Scroll Wheel) ---
    if (activeRadialMenu == RadialMenuType::COMPONENTS) {
      float wheel = GetMouseWheelMove();
      if (wheel != 0.0f) {
        int totalPages = componentPages.size();
        if (wheel > 0) { // Scroll Up
          currentComponentPage = (currentComponentPage + 1) % totalPages;
        } else { // Scroll Down
          currentComponentPage =
              (currentComponentPage - 1 + totalPages) % totalPages;
        }
        // Updating  the active page pointer
        currentActivePage = &componentPages[currentComponentPage];
      }
    }

    // --- HOVER LOGIC ---
    Vector2 mousePos = GetMousePosition();
    float dx = mousePos.x - radialMenuPos.x;
    float dy = mousePos.y - radialMenuPos.y;
    float distance = std::sqrt(dx * dx + dy * dy);

    if (distance > 30.0f) {
      float angle = std::atan2(dy, dx);
      if (angle < 0)
        angle += 2.0f * PI;

      float sliceSize = (2.0f * PI) / currentActivePage->items.size();
      angle += sliceSize / 2.0f;
      if (angle >= 2.0f * PI)
        angle -= 2.0f * PI;

      hoveredRadialSector = (int)(angle / sliceSize);
    } else {
      hoveredRadialSector = -1;
    }
  }

  //  Equip Tool on TAB Release
  if (IsKeyReleased(KEY_TAB) && activeRadialMenu == RadialMenuType::TOOLS) {
    if (hoveredRadialSector != -1) {
      std::string selected = currentActivePage->items[hoveredRadialSector];

      // Ignoring empty element
      if (selected != "") {
        if (isPlacingComponent) {
          isPlacingComponent = false;
          logic.removeComponentByID(logic.getLastUsedID());
          activeComponent = nullptr;
        }
        if (selected == "Select")
          currentTool = Tool::Move;
        else if (selected == "Wire")
          currentTool = Tool::Wire;
      }
    }
    activeRadialMenu = RadialMenuType::NONE;
  }

  // Equip Component on SPACE Release
  if (IsKeyReleased(KEY_SPACE) &&
      activeRadialMenu == RadialMenuType::COMPONENTS) {
    if (hoveredRadialSector != -1) {
      std::string selected = currentActivePage->items[hoveredRadialSector];

      // Ignoring empty elements
      if (selected != "") {
        if (isPlacingComponent) {
          isPlacingComponent = false;
          logic.removeComponentByID(logic.getLastUsedID());
          activeComponent = nullptr;
        }

        if (selected == "AND") {
          isPlacingComponent = true;
          logic.addANDGate();
        } else if (selected == "OR") {
          isPlacingComponent = true;
          logic.addORGate();
        } else if (selected == "NOT") {
          isPlacingComponent = true;
          logic.addNOTGate();
        } else if (selected == "NAND") {
          isPlacingComponent = true;
          logic.addNANDGate();
        } else if (selected == "NOR") {
          isPlacingComponent = true;
          logic.addNORGate();
        } else if (selected == "Button") {
          isPlacingComponent = true;
          logic.addButton();
        } else if (selected == "Bulb") {
          isPlacingComponent = true;
          logic.addBulb();
        }
      }
    }
    activeRadialMenu = RadialMenuType::NONE;
  }
}

void App::drawRadialMenu() {
  if (activeRadialMenu == RadialMenuType::NONE || currentActivePage == nullptr)
    return;

  float sliceDeg = 360.0f / currentActivePage->items.size();

  for (size_t i = 0; i < currentActivePage->items.size(); i++) {
    float startAngle = i * sliceDeg - (sliceDeg / 2.0f);
    float endAngle = startAngle + sliceDeg;

    std::string itemName = currentActivePage->items[i];
    bool isEmptySlot = (itemName == "");

    // Color logic (Dim out slots that have no tool assigned)
    Color sectorColor;
    if (isEmptySlot) {
      sectorColor = Fade(DARKGRAY, 0.3f);
    } else {
      sectorColor = (i == hoveredRadialSector) ? Fade(SKYBLUE, 0.8f)
                                               : Fade(DARKGRAY, 0.8f);
    }

    DrawCircleSector(radialMenuPos, 150.0f, startAngle, endAngle, 32,
                     sectorColor);

    // Draw separator lines
    Vector2 lineEnd = {
        radialMenuPos.x + std::cos(startAngle * DEG2RAD) * 150.0f,
        radialMenuPos.y + std::sin(startAngle * DEG2RAD) * 150.0f};
    DrawLineEx(radialMenuPos, lineEnd, 2.0f, Fade(RAYWHITE, 0.5f));

    // Draw Tool Text
    if (!isEmptySlot) {
      float textAngle = i * sliceDeg * DEG2RAD;
      float textRadius = 100.0f;
      Vector2 textPos = {radialMenuPos.x + std::cos(textAngle) * textRadius,
                         radialMenuPos.y + std::sin(textAngle) * textRadius};

      int fontSize = 20;
      int textWidth = MeasureText(itemName.c_str(), fontSize);
      Color textColor = (i == hoveredRadialSector) ? WHITE : LIGHTGRAY;

      DrawText(itemName.c_str(), textPos.x - textWidth / 2,
               textPos.y - fontSize / 2, fontSize, textColor);
    }
  }

  // --- DRAW THE CENTER HOLE ---
  DrawCircleV(radialMenuPos, 45.0f, render.COLOR_OF_BACKGROUND);
  DrawCircleLines(radialMenuPos.x, radialMenuPos.y, 45.0f,
                  Fade(RAYWHITE, 0.8f));
  DrawCircleLines(radialMenuPos.x, radialMenuPos.y, 150.0f,
                  Fade(RAYWHITE, 0.8f));

  // Print the active Category Name in the center
  std::string centerText = currentActivePage->categoryName;
  int w1 = MeasureText(centerText.c_str(), 10);
  DrawText(centerText.c_str(), radialMenuPos.x - w1 / 2, radialMenuPos.y - 12,
           10, ORANGE);

  // Print scrolling hint for components
  if (activeRadialMenu == RadialMenuType::COMPONENTS) {
    std::string hint = "Scroll";
    int w2 = MeasureText(hint.c_str(), 10);
    DrawText(hint.c_str(), radialMenuPos.x - w2 / 2, radialMenuPos.y + 4, 10,
             LIGHTGRAY);
  }
}

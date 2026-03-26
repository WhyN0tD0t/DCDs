#include "App.h"
#include "Logic.h"
#include "Renderer.h"
#include "imgui.h"
#include "raylib.h"

App::App() {
  InitWindow(screenWidth, screenHeight, "DCDsimulator");
  SetTargetFPS(60);

  rlImGuiSetup(true);
}

float SCALE_FACTOR = 1.0f;
App::~App() {
  rlImGuiShutdown();

  CloseWindow();
}

void App::drawUI() {
  rlImGuiBegin();

  // ui drawing code
  bool open = true;
  ImGui::ShowDemoWindow(&open);
  open = true;
  if (ImGui::Begin("Test Window", &open)) {
    ImGui::TextUnformatted(ICON_FA_JEDI);
  }
  ImGui::End();

  rlImGuiEnd();

  if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
    DrawText("Prssed", 0, 0, 20, RED);
}

void App::draw() {

  DrawRectangle(screenWidth / 2, screenHeight / 2, 50, 50, BLUE);
  render.drawNOTGate(400.0f, 500.0f, 1);
  render.drawNOTGate(600.0f, 300.0f, 0);
  for (auto &wire : logic.getWires()) {
    render.drawWire(wire->startPos.x, wire->startPos.y, wire->endPos.x,
                    wire->endPos.y);
  }

  render.drawANDGate(200.0f, 300.0f, 0);

  render.drawGrid(50.0f);
}

void App::update() {

  // code for drawing updating the wire drawing
  if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
    isDrawingWire = true;
    Vector2 startPos = GetMousePosition();
    logic.addWire();
    auto wire = logic.getWireByID(logic.getLastUsedID());
    wire->startPos.x = startPos.x;
    wire->startPos.y = startPos.y;
    wire->endPos.x = startPos.x;
    wire->endPos.y = startPos.y;
  }
  if (isDrawingWire) {

    if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
      Vector2 endPos = GetMousePosition();
      auto wire = logic.getWireByID(logic.getLastUsedID());
      wire->endPos.x = endPos.x;
      wire->endPos.y = endPos.y;
    } else if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
      isDrawingWire = false;
    }
  }
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

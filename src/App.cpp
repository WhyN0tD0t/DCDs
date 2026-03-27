#include "App.h"
#include "Logic.h"
#include "Renderer.h"
#include "imgui.h"
#include "raylib.h"
#include "raymath.h"

#include <math.h>

App::App() {
  InitWindow(screenWidth, screenHeight, "DCDsimulator");
  SetTargetFPS(60);

  rlImGuiSetup(true);

  // camera initialization settings
  camera.zoom = 1.0f;
  camera.rotation = 0.0f;
  camera.target = (Vector2){0.0f, 0.0f};

  // Set the offset to the exact center of the screen
  camera.offset = (Vector2){screenWidth / 2.0f, screenHeight / 2.0f};
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
    ImGui::TextUnformatted(ICON_FA_FILE_INVOICE);
  }
  ImGui::End();

  rlImGuiEnd();

  if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
    DrawText("Prssed", 0, 0, 20, RED);
}

void App::draw() {
  BeginMode2D(camera);
  // drawing below
  // -------------

  Vector2 topLeft = GetScreenToWorld2D((Vector2){0, 0}, camera);
  Vector2 bottomRight = GetScreenToWorld2D(
      (Vector2){(float)screenWidth, (float)screenHeight}, camera);

  DrawRectangle(screenWidth / 2, screenHeight / 2, 50, 50, BLUE);
  render.drawNOTGate(400.0f, 500.0f, 1);
  render.drawNOTGate(600.0f, 300.0f, 0);
  for (auto &wire : logic.getWires()) {
    render.drawWire(wire->startPos.x, wire->startPos.y, wire->endPos.x,
                    wire->endPos.y);
  }

  render.drawANDGate(200.0f, 300.0f, 0);

  render.drawGrid(topLeft, bottomRight);

  //---------
  EndMode2D();
}

void App::update() {
  // useful variables
  Vector2 mouseWorldPos = GetScreenToWorld2D(GetMousePosition(), camera);
  float snappedX =
      floor(mouseWorldPos.x / render.getCellSize()) * render.getCellSize();
  float snappedY =
      floor(mouseWorldPos.y / render.getCellSize()) * render.getCellSize();
  Vector2 snappedMousePos = (Vector2){snappedX, snappedY};

  // MOVING AROUND THE WORLD
  if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT)) {
    Vector2 delta = GetMouseDelta();
    // Move target opposite to the mouse drag, scaled by zoom
    camera.target.x -= delta.x / camera.zoom;
    camera.target.y -= delta.y / camera.zoom;
  }

  // code for drawing updating the wire drawing
  if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
    isDrawingWire = true;
    Vector2 startPos = GetMousePosition();
    logic.addWire();
    auto wire = logic.getWireByID(logic.getLastUsedID());
    wire->startPos.x = snappedX;
    wire->startPos.y = snappedY;
    wire->endPos.x = snappedX;
    wire->endPos.y = snappedY;
  }
  if (isDrawingWire) {
    if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
      Vector2 endPos = GetMousePosition();
      auto wire = logic.getWireByID(logic.getLastUsedID());
      wire->endPos.x = snappedX;
      wire->endPos.y = snappedY;
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

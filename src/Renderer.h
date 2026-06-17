#ifndef RENDERER_H
#define RENDERER_H

#include "raylib.h"
#include "rlImGui.h"

#include <memory>

import Component;

class Renderer {
private:
  float cellSize = 20.0f;

public:
  void drawANDGate(float x, float y);
  void drawNANDGate(float x, float y, bool outputState);
  void drawORGate(float x, float y);
  void drawNORGate(float x, float y, bool outputState);
  void drawNOTGate(float x, float y, bool outputState);

  void drawBulb(float x, float y, bool outputState);
  void drawButton(float x, float y, bool state);

  void drawWire(float x1, float y1, float x2, float y2);

  void drawPin(float x, float y, bool isHovered);

  void drawGrid(Vector2 topLeft, Vector2 bottomRight);

  void drawSelection(Component *component);

  // For DEBUGGING
  void drawHitBox(const std::unique_ptr<Component> &component);

  float getCellSize() { return cellSize; }
  float getCellSizeSmall() { return cellSize / 2.0f; }
  // COLORS
  Color COLOR_OF_CMPNTS = RAYWHITE;
  Color COLOR_OF_HIGH_STATE = RED;
  Color COLOR_OF_BACKGROUND = {30, 34, 42, 255};
  Color COLOR_OF_GRID = {60, 64, 72, 200};
  Color COLOR_OF_GRID_SMALL = {60, 64, 72, 50};
  Color COLOR_OF_SELECTION = {0, 200, 72, 255};

  Color COLOR_DEBUG = {200, 0, 0, 100};
};

#endif

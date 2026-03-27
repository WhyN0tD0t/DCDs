#ifndef RENDERER_H
#define RENDERER_H

#include "raylib.h"
#include "rlImGui.h"

class Renderer {
private:
  float SF = 1.5f; // scale factor ( ZOOM 1x)
  float cellSize = 20.0f;

public:
  void drawANDGate(float x, float y, bool outputState);
  void drawORGate(float x, float y, bool outputState);
  void drawNOTGate(float x, float y, bool outputState);

  void drawWire(float x1, float y1, float x2, float y2);

  void drawGrid(Vector2 topLeft, Vector2 bottomRight);

  float getCellSize() { return cellSize; }
  // COLORS
  Color COLOR_OF_CMPNTS = RAYWHITE;
  Color COLOR_OF_HIGH_STATE = RED;
  Color COLOR_OF_BACKGROUND = {30, 34, 42, 255};
  Color COLOR_OF_GRID = {60, 64, 72, 200};
};

#endif

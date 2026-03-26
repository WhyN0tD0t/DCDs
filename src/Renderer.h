#ifndef RENDERER_H
#define RENDERER_H

#include "rlImGui.h"

class Renderer {
private:
  float SF = 1.5f; // scale factor ( ZOOM 1x)

public:
  void drawANDGate(float x, float y, bool outputState);
  void drawORGate(float x, float y, bool outputState);
  void drawNOTGate(float x, float y, bool outputState);

  void drawWire(float x1, float y1, float x2, float y2);

  void drawGrid(float offset);

  // COLORS
  Color COLOR_OF_CMPNTS = RAYWHITE;
  Color COLOR_OF_HIGH_STATE = RED;
  Color COLOR_OF_BACKGROUND = {30, 34, 42, 255};
};

#endif

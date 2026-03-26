#include "Renderer.h"
#include "imgui.h"
#include "raylib.h"

void Renderer::drawNOTGate(float x, float y, bool outputState) {

  // triangle vertices
  Vector2 v1 = {x, y};
  Vector2 v2 = {x, y + 30.0f * SF};
  Vector2 v3 = {x + 30.0f * SF, y + 15.0f * SF};

  // triangle lines
  DrawLineEx(v1, v2, 2.0f, COLOR_OF_CMPNTS);
  DrawLineEx(v2, v3, 2.0f, COLOR_OF_CMPNTS);
  DrawLineEx(v3, v1, 2.0f, COLOR_OF_CMPNTS);

  // bubble of NOT gate
  float radius = 7.5f * SF;
  float innerRadius = radius - 2.0f;

  Vector2 center = {v3.x + radius, v3.y};

  // bubble coloring
  if (outputState == 0)
    DrawRing(center, innerRadius, radius, 0.0f, 360.0f, 36, COLOR_OF_CMPNTS);
  else if (outputState == 1) {
    DrawCircle(v3.x + radius, v3.y, innerRadius, COLOR_OF_HIGH_STATE);
    DrawRing(center, innerRadius, radius, 0.0f, 360.0f, 36, COLOR_OF_CMPNTS);
  }
}

void Renderer::drawANDGate(float x, float y, bool outputState) {
  Vector2 v1 = {x, y};                          // left-upper corner
  Vector2 v2 = {x, y + 30.0f * SF};             // left-lower corner
  Vector2 v3 = {x + 10 * SF, y};                // begin of the arc
  Vector2 v4 = {x + 10 * SF, y + 30.0f * SF};   // end of the arc
  Vector2 vcenter = {x + 10 * SF, y + 15 * SF}; // center of the arc

  // deleting an additional unconditional line of the arc
  Vector2 v3a = {x + 10 * SF, y + 1};          // begin of the arc
  Vector2 v4a = {x + 10 * SF, y + 29.1f * SF}; // end of the arc

  DrawLineEx(v1, v2, 2.0f, COLOR_OF_CMPNTS);
  DrawLineEx(v1, v3, 2.0f, COLOR_OF_CMPNTS);
  DrawLineEx(v2, v4, 2.0f, COLOR_OF_CMPNTS);

  DrawCircleSectorLines(vcenter, 15.0f * SF, -90.0f, 90.0f, 20,
                        COLOR_OF_CMPNTS);
  DrawCircleSectorLines(vcenter, 15.5f * SF, -90.0f, 90.0f, 20,
                        COLOR_OF_CMPNTS);

  DrawLineEx(v3a, v4a, 4.0f, COLOR_OF_BACKGROUND);
}

void Renderer::drawWire(float x1, float y1, float x2, float y2) {
  Vector2 start = {x1, y1};
  Vector2 end = {x2, y2};

  Vector2 xmid = {x2, y1};
  Vector2 ymid = {x1, y2};

  // probably a loop should be implemented to draw a staircase line
  // the way that user wants it to look like, until the wire reaches
  // its destination (or the left click of the mouse is let off)

  // first x then y
  DrawLineEx(start, xmid, 2.0f, COLOR_OF_CMPNTS);
  DrawLineEx(xmid, end, 2.0f, COLOR_OF_CMPNTS);

  // first y then x
  // DrawLineEx(start, ymid, 2.0f, COLOR_OF_CMPNTS);
  // DrawLineEx(ymid, end, 2.0f, COLOR_OF_CMPNTS);
}

void Renderer::drawGrid(float offset) { DrawGrid(5, offset); }

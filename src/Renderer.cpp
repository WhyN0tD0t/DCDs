#include "Renderer.h"

#include "imgui.h"
#include "raylib.h"

#include <cstdlib>
#include <math.h>

import Component;

void Renderer::drawNOTGate(float x, float y, bool outputState) {

  // triangle vertices
  Vector2 v1 = {x, y};
  Vector2 v2 = {x, y + 20.0f};
  Vector2 v3 = {x + 20.0f, y + 10.0f};

  // triangle lines
  DrawLineEx(v1, v2, 2.0f, COLOR_OF_CMPNTS);
  DrawLineEx(v2, v3, 2.0f, COLOR_OF_CMPNTS);
  DrawLineEx(v3, v1, 2.0f, COLOR_OF_CMPNTS);

  // bubble of NOT gate
  float radius = getCellSizeSmall() / 2;
  float innerRadius = radius - 2.0f;

  Vector2 center = {v3.x + radius, v3.y};

  // bubble coloring
  if (outputState == 0)
    DrawRing(center, innerRadius, radius, 0.0f, 360.0f, 36, COLOR_OF_CMPNTS);
  else if (outputState == 1) {
    DrawCircle(v3.x + radius, v3.y, innerRadius + 1.0f, COLOR_OF_HIGH_STATE);
    DrawRing(center, innerRadius, radius, 0.0f, 360.0f, 36, COLOR_OF_CMPNTS);
  }
}

void Renderer::drawORGate(float x, float y) {

  // upper arc
  DrawRing({x, y + 40}, 38.0f, 40.0f, -90.0f, -30.0f, 60, COLOR_OF_CMPNTS);
  // lower arc
  DrawRing({x, y}, 38.0f, 40.0f, 90.0f, 30.0f, 60, COLOR_OF_CMPNTS);
  // left-connecting arc
  DrawRing({x - 34, y + 20}, 38.0f, 40.0f, 30.0f, -30.0f, 60, COLOR_OF_CMPNTS);

  //-------------
  // input cables
  //-------------

  DrawLineEx({x, y + 10}, {x + 3, y + 10}, 2.0f, COLOR_OF_CMPNTS);

  DrawLineEx({x, y + 30}, {x + 3, y + 30}, 2.0f, COLOR_OF_CMPNTS);

  DrawLineEx({x + 33, y + 20}, {x + 40, y + 20}, 2.0f, COLOR_OF_CMPNTS);
}

void Renderer::drawNORGate(float x, float y, bool outputState) {

  // upper arc
  DrawRing({x, y + 40}, 38.0f, 40.0f, -90.0f, -30.0f, 60, COLOR_OF_CMPNTS);
  // lower arc
  DrawRing({x, y}, 38.0f, 40.0f, 90.0f, 30.0f, 60, COLOR_OF_CMPNTS);
  // left-connecting arc
  DrawRing({x - 34, y + 20}, 38.0f, 40.0f, 30.0f, -30.0f, 60, COLOR_OF_CMPNTS);

  //-------------
  // input cables
  //-------------

  DrawLineEx({x, y + 10}, {x + 3, y + 10}, 2.0f, COLOR_OF_CMPNTS);

  DrawLineEx({x, y + 30}, {x + 3, y + 30}, 2.0f, COLOR_OF_CMPNTS);

  // bubble of NAND gate
  float radius = getCellSizeSmall() / 2;
  float innerRadius = radius - 2.0f;

  Vector2 center = {x + 38, y + 20};

  // negation circle at the end
  if (outputState == 0)
    DrawRing(center, innerRadius, radius, 0.0f, 360.0f, 36, COLOR_OF_CMPNTS);
  else if (outputState == 1) {
    DrawCircle(center.x, center.y, innerRadius + 1.0f, COLOR_OF_HIGH_STATE);
    DrawRing(center, innerRadius, radius, 0.0f, 360.0f, 36, COLOR_OF_CMPNTS);
  }

  // extended part at the end
  DrawLineEx({center.x + 5, center.y}, {center.x + 12, center.y}, 2.0f,
             COLOR_OF_CMPNTS);
}

void Renderer::drawANDGate(float x, float y) {
  Vector2 v1 = {x, y};                // left-upper corner
  Vector2 v2 = {x, y + 40.0f};        // left-lower corner
  Vector2 v3 = {x + 15, y};           // begin of the arc
  Vector2 v4 = {x + 15, y + 40.0f};   // end of the arc
  Vector2 vcenter = {x + 15, y + 20}; // center of the arc

  DrawLineEx(v1, v2, 2.0f, COLOR_OF_CMPNTS);
  DrawLineEx(v1, v3, 2.0f, COLOR_OF_CMPNTS);
  DrawLineEx(v2, v4, 2.0f, COLOR_OF_CMPNTS);

  DrawRing(vcenter, 19.0f, 21.0f, -90.0f, 90.0f, 20, COLOR_OF_CMPNTS);

  // extended part at the end
  DrawLineEx({x + 36, y + 20}, {x + 40, y + 20}, 2.0f, COLOR_OF_CMPNTS);
}

void Renderer::drawNANDGate(float x, float y, bool outputState) {
  Vector2 v1 = {x, y};                // left-upper corner
  Vector2 v2 = {x, y + 40.0f};        // left-lower corner
  Vector2 v3 = {x + 15, y};           // begin of the arc
  Vector2 v4 = {x + 15, y + 40.0f};   // end of the arc
  Vector2 vcenter = {x + 15, y + 20}; // center of the arc

  DrawLineEx(v1, v2, 2.0f, COLOR_OF_CMPNTS);
  DrawLineEx(v1, v3, 2.0f, COLOR_OF_CMPNTS);
  DrawLineEx(v2, v4, 2.0f, COLOR_OF_CMPNTS);

  DrawRing(vcenter, 19.0f, 21.0f, -90.0f, 90.0f, 20, COLOR_OF_CMPNTS);

  // bubble of NAND gate
  float radius = getCellSizeSmall() / 2;
  float innerRadius = radius - 2.0f;

  Vector2 center = {vcenter.x + 25, vcenter.y};

  // negation circle at the end
  if (outputState == 0)
    DrawRing(center, innerRadius, radius, 0.0f, 360.0f, 36, COLOR_OF_CMPNTS);
  else if (outputState == 1) {
    DrawCircle(center.x, center.y, innerRadius + 1.0f, COLOR_OF_HIGH_STATE);
    DrawRing(center, innerRadius, radius, 0.0f, 360.0f, 36, COLOR_OF_CMPNTS);
  }

  // extended part at the end
  DrawLineEx({center.x + 5, center.y}, {center.x + 10, center.y}, 2.0f,
             COLOR_OF_CMPNTS);
}
// BULB
void Renderer::drawBulb(float x, float y, bool outputState) {
  if (outputState == 1) {
    DrawRing({x, y}, 9.0f, 11.0f, 0.0f, 360.0f, 60, COLOR_OF_CMPNTS);
    DrawCircle(x, y, 9.0f, COLOR_OF_HIGH_STATE);
  } else {
    DrawRing({x, y}, 9.0f, 11.0f, 0.0f, 360.0f, 60, COLOR_OF_CMPNTS);
  }
}

void Renderer::drawButton(float x, float y, bool state) {
  Rectangle rec{x, y, getCellSize(), getCellSize()};
  DrawRectangleLinesEx(rec, 2.0f, COLOR_OF_CMPNTS);
  if (state == 0) {
    DrawText("0", x + 7, y + 5, 12, COLOR_OF_CMPNTS);
  } else if (state == 1) {
    DrawText("1", x + 10, y + 5, 12, COLOR_OF_CMPNTS);
  }
}

void Renderer::drawWire(float x1, float y1, float x2, float y2) {
  Vector2 start = {x1, y1};
  Vector2 end = {x2, y2};

  Vector2 xmid = {x2, y1};
  Vector2 ymid = {x1, y2};

  // probably a loop should be implemented to draw a staircase line
  // the way that user wants it to look like, until the wire reaches
  // its destination (or the left click of the mouse is let off)

  if (abs(x2 - x1) > abs(y2 - y1)) {

    // first x then y
    DrawLineEx(start, xmid, 2.0f, COLOR_OF_CMPNTS);
    DrawLineEx(xmid, end, 2.0f, COLOR_OF_CMPNTS);
  } else {

    // first y then x
    DrawLineEx(start, ymid, 2.0f, COLOR_OF_CMPNTS);
    DrawLineEx(ymid, end, 2.0f, COLOR_OF_CMPNTS);
  }
}

void Renderer::drawPin(float x, float y, bool isHovered) {
  if (isHovered)
    DrawCircle(x, y, 1.5f, COLOR_OF_SELECTION);
  else
    DrawCircle(x, y, 1.5f, COLOR_OF_CMPNTS);
}

void Renderer::drawGrid(Vector2 topLeft, Vector2 bottomRight) {
  int startX = floor(topLeft.x / cellSize) * cellSize;
  int startY = floor(topLeft.y / cellSize) * cellSize;
  int endX = bottomRight.x;
  int endY = bottomRight.y;

  // SMALLER GRID FOR WIRES
  for (int x = startX; x <= endX + cellSize / 2; x += cellSize / 2) {
    DrawLine(x, startY, x, endY + cellSize / 2, COLOR_OF_GRID_SMALL);
  }
  // Draw horizontal lines
  for (int y = startY; y <= endY + cellSize / 2; y += cellSize / 2) {
    DrawLine(startX, y, endX + cellSize / 2, y, COLOR_OF_GRID_SMALL);
  }

  // Draw vertical lines
  for (int x = startX; x <= endX + cellSize; x += cellSize) {
    DrawLine(x, startY, x, endY + cellSize, COLOR_OF_GRID);
  }
  // Draw horizontal lines
  for (int y = startY; y <= endY + cellSize; y += cellSize) {
    DrawLine(startX, y, endX + cellSize, y, COLOR_OF_GRID);
  }
}

void Renderer::drawHitBox(const std::unique_ptr<Component> &component) {
  Vector2 startPos = {component->hitbox.startX, component->hitbox.startY};
  Vector2 endPos = {component->hitbox.endX, component->hitbox.endY};

  Rectangle box{startPos.x, startPos.y, abs(endPos.x - startPos.x),
                abs(endPos.y - startPos.y)};
  DrawRectangleLinesEx(box, 2.0f, COLOR_DEBUG);
}

void Renderer::drawSelection(Component *component) {
  Vector2 startPos = {component->hitbox.startX, component->hitbox.startY};
  Vector2 endPos = {component->hitbox.endX, component->hitbox.endY};

  Rectangle box{startPos.x, startPos.y, abs(endPos.x - startPos.x),
                abs(endPos.y - startPos.y)};
  DrawRectangleLinesEx(box, 2.0f, COLOR_OF_SELECTION);
}

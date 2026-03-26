#ifndef APP_H
#define APP_H

#include "Logic.h"
#include "Renderer.h"
#include "rlImGui.h"

class App {
public:
  App();
  ~App();

  void run();
  bool isDrawingWire;

private:
  void draw();
  void drawUI();
  void update();

  Renderer render;
  Logic logic;

  bool isRunning = true;
  int screenWidth = 1280;
  int screenHeight = 720;

  bool showToolBox = true;
};

#endif

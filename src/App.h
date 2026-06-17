#ifndef APP_H
#define APP_H

#include "Logic.h"
#include "Renderer.h"
#include "raylib.h"
#include "rlImGui.h"
#include <memory>

#include <atomic>
#include <cstdlib>
#include <filesystem>
#include <mutex>
#include <thread>

struct Wire {
  Vector2 startPos;
  Vector2 endPos;
};

enum class Tool { Wire, Move };

class App {
public:
  App();
  ~App();

  void run();
  bool isDrawingWire;
  bool isPlacingComponent;
  bool isMouseOverUI;
  bool isColliding;
  bool clickedSomething;

  // selected tools from ImGui ToolBox
  bool guiWireTool;
  bool guiSelectTool;

  float cameraZoom = 1.0f;
  bool showToolBox = true;

private:
  std::thread simulationThread;
  std::mutex circuitMutex;
  std::atomic<bool> isThreadRunning{true};

  void simulationLoop();

  void draw();
  void drawUI();
  void update();

  // for saving/loading
  void saveToFile(const std::string &filepath);
  void loadFromFile(const std::string &filepath);

  // File menu states
  bool showSavePopup = false;
  bool showLoadPopup = false;
  char filenameBuffer[256] = "my_circuit"; // Default text in the box
                                           //
  std::vector<std::string> availableSaves;
  void refreshSaveList();
  std::filesystem::path getAppSaveDirectory();
  // --------------------------------------------------------------

  Renderer render;
  Logic logic;

  std::vector<Wire> visualWires;
  Wire activeDrawingWire;

  Tool currentTool;

  Component *activeComponent = nullptr;
  Component *activeTargetComponent = nullptr;
  // FIX:
  //  deal with below later, maybe the first one is not needed due to active
  //  comp
  // Component *pendingSourceComponent = nullptr;
  int pendingSourcePin = -1;
  int pendingTargetPin = -1;

  int savedSourcePin = -1;

  int currentWireStartJunctionID = -1;

  bool isRunning = true;
  int screenWidth = 1280;
  int screenHeight = 720;

  Camera2D camera = {0};

  // Circular Menu
  struct MenuPage {
    std::string categoryName;
    std::vector<std::string>
        items; // Always exactly 4 items for perfect 90-degree slices!
  };

  enum class RadialMenuType { NONE, TOOLS, COMPONENTS };
  RadialMenuType activeRadialMenu = RadialMenuType::NONE;

  Vector2 radialMenuPos = {0, 0};
  int hoveredRadialSector = -1;

  // The single Tools page
  MenuPage toolsPage = {"TOOLS", {"Select", "Wire", "", ""}};

  // The scrolling Component pages (Pad with "" to keep it exactly 4 slices)
  std::vector<MenuPage> componentPages = {
      {"GATES 1/2", {"AND", "OR", "NOT", "NAND"}},
      {"GATES 2/2", {"NOR", "", "", ""}},
      {"I / O", {"Button", "Bulb", "", ""}}};

  // State trackers
  int currentComponentPage = 0;
  MenuPage *currentActivePage = nullptr;

  void updateRadialMenu();
  void drawRadialMenu();
};

#endif

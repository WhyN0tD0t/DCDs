# ==========================================
# Variables & Flags
# ==========================================
CXX = g++

CXXFLAGS = -g -Wall -std=c++20 -fmodules-ts -I./raylib/include -I./raylib/src -I./src -I./src/external -I./src/external/imgui -I./src/external/rlImGui

LDFLAGS = -L./raylib/src
LIBS = -lraylib -lGL -lm -lpthread -ldl -lrt -lX11

# ==========================================
# Auto-Download Directories
# ==========================================
RAYLIB_DIR = raylib
RAYLIB_LIB = $(RAYLIB_DIR)/src/libraylib.a

IMGUI_DIR = src/external/imgui
RLIMGUI_DIR = src/external/rlImGui

# ==========================================
# File Groupings
# ==========================================

IMGUI_OBJS = $(IMGUI_DIR)/imgui.o $(IMGUI_DIR)/imgui_demo.o $(IMGUI_DIR)/imgui_draw.o \
             $(IMGUI_DIR)/imgui_tables.o $(IMGUI_DIR)/imgui_widgets.o
RLIMGUI_OBJS = $(RLIMGUI_DIR)/rlImGui.o

CPP_SRCS = $(shell find src -type f -name "*.cpp" ! -path "src/external/*")
CPP_OBJS = $(CPP_SRCS:.cpp=.o)

MODULE_OBJS = src/Component.o \
              src/Gate.o src/Bulb.o src/Button.o src/Junction.o src/LogicalConnection.o \
              src/AND_gate.o src/OR_gate.o src/NOT_gate.o src/NAND_gate.o src/NOR_gate.o

TARGET = dcds

# ==========================================
# Main Rules
# ==========================================

all: $(TARGET)

$(TARGET): $(RAYLIB_LIB) $(IMGUI_DIR) $(RLIMGUI_DIR) $(MODULE_OBJS) $(IMGUI_OBJS) $(RLIMGUI_OBJS) $(CPP_OBJS)
	$(CXX) $(MODULE_OBJS) $(CPP_OBJS) $(IMGUI_OBJS) $(RLIMGUI_OBJS) -o $(TARGET) $(LDFLAGS) $(LIBS)

# ==========================================
# Auto-Download Targets (WITH SAFETY CHECKS)
# ==========================================

$(RAYLIB_LIB):
	@if [ ! -d "$(RAYLIB_DIR)" ]; then \
		echo "Raylib folder not found! Downloading..."; \
		git clone --depth 1 https://github.com/raysan5/raylib.git $(RAYLIB_DIR); \
	fi
	@echo "Building Raylib..."
	@$(MAKE) -C $(RAYLIB_DIR)/src

$(IMGUI_DIR):
	@if [ ! -d "$(IMGUI_DIR)" ]; then \
		echo "ImGui not found! Downloading..."; \
		mkdir -p src/external; \
		git clone --depth 1 https://github.com/ocornut/imgui.git $(IMGUI_DIR); \
	fi

$(RLIMGUI_DIR):
	@if [ ! -d "$(RLIMGUI_DIR)" ]; then \
		echo "rlImGui not found! Downloading..."; \
		mkdir -p src/external; \
		git clone --depth 1 https://github.com/raylib-extras/rlImGui.git $(RLIMGUI_DIR); \
	fi

# ==========================================
# EXTERNAL COMPILATION RULES
# ==========================================

$(IMGUI_DIR)/%.o: $(IMGUI_DIR)/%.cpp | $(IMGUI_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(RLIMGUI_DIR)/%.o: $(RLIMGUI_DIR)/%.cpp | $(RLIMGUI_DIR) $(IMGUI_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# ==========================================
# MODULE COMPILATION RULES (Strict Order)
# ==========================================

src/Component.o: src/Component.cppm | $(IMGUI_DIR) $(RLIMGUI_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

src/Gate.o: src/Gate.cppm src/Component.o | $(IMGUI_DIR) $(RLIMGUI_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

src/Bulb.o: src/Bulb.cppm src/Component.o | $(IMGUI_DIR) $(RLIMGUI_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

src/Button.o: src/Button.cppm src/Component.o | $(IMGUI_DIR) $(RLIMGUI_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

src/Junction.o: src/Junction.cppm src/Component.o | $(IMGUI_DIR) $(RLIMGUI_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

src/LogicalConnection.o: src/LogicalConnection.cppm src/Component.o | $(IMGUI_DIR) $(RLIMGUI_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

src/AND_gate.o: src/AND_gate.cppm src/Gate.o | $(IMGUI_DIR) $(RLIMGUI_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

src/OR_gate.o: src/OR_gate.cppm src/Gate.o | $(IMGUI_DIR) $(RLIMGUI_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

src/NOT_gate.o: src/NOT_gate.cppm src/Gate.o | $(IMGUI_DIR) $(RLIMGUI_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

src/NAND_gate.o: src/NAND_gate.cppm src/Gate.o | $(IMGUI_DIR) $(RLIMGUI_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

src/NOR_gate.o: src/NOR_gate.cppm src/Gate.o | $(IMGUI_DIR) $(RLIMGUI_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# ==========================================
# TRADITIONAL C++ RULES
# ==========================================

src/%.o: src/%.cpp $(MODULE_OBJS) | $(IMGUI_DIR) $(RLIMGUI_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# ==========================================
# CLEANING RULES
# ==========================================

clean:
	rm -f $(shell find src -name "*.o") $(TARGET)

clean-all: clean
	rm -rf $(RAYLIB_DIR) src/external

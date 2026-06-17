# Variables
CXX = g++
CXXFLAGS = -g -Wall -std=c++17 -I./raylib/include -I./raylib/src -I./src -I./src/external -I./src/external/imgui
LDFLAGS = -L./raylib/lib
LIBS = -lraylib -lGL -lm -lpthread -ldl -lrt -lX11

# Automatically find all .cpp files in src/
SRCS = $(shell find src -name "*.cpp")
# Change .cpp to .o for the object files
OBJS = $(SRCS:.cpp=.o)
# The final name of your program
TARGET = dcds

# Default rule
all: $(TARGET)

# Link the object files to create the executable
$(TARGET): $(OBJS)
	$(CXX) $(OBJS) -o $(TARGET) $(LDFLAGS) $(LIBS)

# Compile each .cpp file into a .o file
# 3. This rule handles compilation for files in any subfolder
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean up build files
clean:
	rm -f $(shell find src -name "*.o") $(TARGET)

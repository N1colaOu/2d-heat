# Makefile for multiple source files in ./src/
# Builds each .cpp into a separate .exe in ./build/

CXX       = g++
CXXFLAGS  = -fdiagnostics-color=always -g
INCLUDES  = -I/usr/include/eigen3 -I/usr/include/suitesparse
LDLIBS    = -lumfpack -lblas -llapack

SRC_DIR   = src
BUILD_DIR = build

# Find all .cpp files in the source directory
SOURCES   = $(wildcard $(SRC_DIR)/*.cpp)
# Convert each source file name to a target executable
TARGETS   = $(patsubst $(SRC_DIR)/%.cpp, $(BUILD_DIR)/%.exe, $(SOURCES))

# Default target: build all executables
all: $(TARGETS)

# Pattern rule: build any .exe from its corresponding .cpp
$(BUILD_DIR)/%.exe: $(SRC_DIR)/%.cpp
	mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $(INCLUDES) $< -o $@ $(LDLIBS)

# Clean up: remove all executables
clean:
	rm -f $(TARGETS)

.PHONY: all clean
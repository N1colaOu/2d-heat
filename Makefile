# Makefile for multiple source files in ./src/
# Builds each .cpp into a separate .exe in ./build/
# Also creates data0.txt, data1.txt, data2.txt in ./build/ if missing.

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

# Data files that should exist in the build directory
DATA_FILES = $(BUILD_DIR)/data0.txt $(BUILD_DIR)/data1.txt $(BUILD_DIR)/data2.txt

# Default target: first ensure data files exist, then build all executables
all: data $(TARGETS)

# Phony target to create data files if they don't exist
data: $(DATA_FILES)

# Pattern rule: build any .exe from its corresponding .cpp
$(BUILD_DIR)/%.exe: $(SRC_DIR)/%.cpp
	mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $(INCLUDES) $< -o $@ $(LDLIBS)

# Rule to create a data file (empty) if it is missing
$(BUILD_DIR)/%.txt:
	mkdir -p $(BUILD_DIR)
	touch $@

# Clean up: remove all executables and data files
clean:
	rm -f $(TARGETS) $(DATA_FILES)

.PHONY: all clean data
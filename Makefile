# Makefile

CXX := g++
CXXFLAGS := -std=c++17 -Wall -Wextra
TARGET := algorithm_visualizer
SRC_DIRS := . data_structures algorithms visualizer tests

# Find all .cpp files recursively
SRC := $(shell find $(SRC_DIRS) -name '*.cpp')

.PHONY: build run clean

build: clean
	@echo "Compiling..."
	$(CXX) $(CXXFLAGS) $(SRC) -o $(TARGET)
	@echo "Build complete: ./$(TARGET)"

run:
	@./$(TARGET)

clean:
	@echo "Cleaning..."
	@rm -f $(TARGET)

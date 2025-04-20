CXX = g++
CXXFLAGS = -std=c++17 -Wall -pthread -Iinclude  # Added -Iinclude for header files

SRC_DIR = src
OBJ_DIR = obj
INCLUDE_DIR = include

# List all source files, including log.cpp and response_generator.cpp
SRCS = $(wildcard $(SRC_DIR)/*.cpp)
OBJS = $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SRCS))

TARGET = server

# Default target
all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJS)

# Compile source files to object files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Create the object directory if it doesn't exist
$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

# Clean up build files
.PHONY: clean
clean:
	rm -rf $(OBJ_DIR) $(TARGET)

# Automatically generate dependencies for header files
$(OBJ_DIR)/%.d: $(SRC_DIR)/%.cpp | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -M $< > $@

-include $(OBJS:.o=.d)

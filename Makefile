# --- Variables ---
CXX      := g++
CXXFLAGS := -std=c++17 -g -Wall -Wextra -Werror -Wno-unused-parameter
CPPFLAGS := -Isrc/chip8 -Isrc/platform # Tells compiler where to find headers

# List all source files
SRC := src/main.cpp \
       src/chip8/chip8.cpp \
       src/chip8/cpu.cpp \
       src/chip8/memory.cpp \
       src/chip8/timers.cpp \
       src/platform/display.cpp

# Transform src/path/file.cpp -> file.o
OBJS := $(notdir $(SRC:.cpp=.o))
# Transform file.o -> file.d (dependency files)
DEPS := $(OBJS:.o=.d)

# VPATH tells Make where to search for the source files listed in OBJS
VPATH := src:src/chip8:src/platform

# --- Targets ---

.PHONY: all clean

all: emulate

# The final executable
emulate: $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

# Pattern rule for object files
# Includes flags to generate .d files automatically
%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -MMD -MP -c $< -o $@

# Include the generated dependency files
-include $(DEPS)

clean:
	rm -f *.o *.d emulate
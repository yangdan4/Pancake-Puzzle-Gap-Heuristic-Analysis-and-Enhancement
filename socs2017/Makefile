all: debug release

# List headers here
HEADERS = PancakePuzzle.h input_utilities.h IDA.h

# Builds list of sources
SOURCES = $(HEADERS:%.h=%.cpp)

# Defines location of the object files
OBJ_LOC = obj

# Defines the objects for the debug build
OBJ_DEBUG_SUFFIX = debug.o
OBJS_DEBUG = $(SOURCES:%.cpp=$(OBJ_LOC)/%.$(OBJ_DEBUG_SUFFIX))

# Defines the objects for the release build
OBJ_RELEASE_SUFFIX = release.o
OBJS_RELEASE = $(SOURCES:%.cpp=$(OBJ_LOC)/%.$(OBJ_RELEASE_SUFFIX))

# Defines the compiler and the compiler flags
CC = g++
CXXRELEASEFLAGS = -O3 -Wall -pedantic -ansi -Wno-unused-function -Wno-unused-variable -std=c++11
CXXDEBUGFLAGS = -g -Wall -pedantic -ansi -Wno-unused-function -Wno-unused-variable -std=c++11

# Building object files
$(OBJS_DEBUG): $(OBJ_LOC)/%.$(OBJ_DEBUG_SUFFIX) : %.cpp %.h
	$(CC) $(CXXDEBUGFLAGS) -c $< -o $@
	
$(OBJS_RELEASE): $(OBJ_LOC)/%.$(OBJ_RELEASE_SUFFIX) : %.cpp %.h
	$(CC) $(CXXRELEASEFLAGS) -c $< -o $@

# Excecutables for problem generators
DEBUG_GENERATOR_TARGET = bin/generator_debug
RELEASE_GENERATOR_TARGET = bin/generator_release

# Executables for problem solving
DEBUG_SOLVER_TARGET = bin/solver_debug
RELEASE_SOLVER_TARGET = bin/solver_release

# Compiles problem generation targets
$(DEBUG_GENERATOR_TARGET): $(OBJS_DEBUG) generate_cycles.cpp
	$(CC) $(CXXDEBUGFLAGS) -o $(DEBUG_GENERATOR_TARGET) generate_cycles.cpp $(OBJS_DEBUG)

$(RELEASE_GENERATOR_TARGET): $(OBJS_RELEASE) generate_cycles.cpp
	$(CC) $(CXXRELEASEFLAGS) -o $(RELEASE_GENERATOR_TARGET) generate_cycles.cpp $(OBJS_RELEASE)

# Compiles solver targets
$(DEBUG_SOLVER_TARGET): $(OBJS_DEBUG) run_solver.cpp
	$(CC) $(CXXDEBUGFLAGS) -o $(DEBUG_SOLVER_TARGET) run_solver.cpp $(OBJS_DEBUG)

$(RELEASE_SOLVER_TARGET): $(OBJS_RELEASE) run_solver.cpp
	$(CC) $(CXXRELEASEFLAGS) -o $(RELEASE_SOLVER_TARGET) run_solver.cpp $(OBJS_RELEASE)

debug: $(DEBUG_GENERATOR_TARGET) $(DEBUG_SOLVER_TARGET)

release: $(RELEASE_GENERATOR_TARGET) $(RELEASE_SOLVER_TARGET)

clean:
	rm -f $(OBJS_DEBUG) $(OBJS_RELEASE) $(DEBUG_GENERATOR_TARGET) $(RELEASE_GENERATOR_TARGET) $(DEBUG_SOLVER_TARGET) $(RELEASE_SOLVER_TARGET)
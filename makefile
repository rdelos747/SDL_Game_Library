#compiler
CXX = clang++
#compiler flags
CXXFLAGS = -Wall -c -std=c++11
#linked frameworks
LDFLAGS = -framework SDL2 -framework SDL2_image
#name of executable
#EXE = SDL_LIBRARY
#list of objects
OBJS = functions.o sprite.o object.o 

#all: $(EXE)

#$(EXE): $(OBJS)
#	$(CXX) $(LDFLAGS) $(OBJS) -o $@

all: $(OBJS)

functions.o: FUNCTIONS/functions.cpp FUNCTIONS/functions.h
	$(CXX) $(CXXFLAGS) $< -o $@

sprite.o: FUNCTIONS/sprite.cpp FUNCTIONS/sprite.h
	$(CXX) $(CXXFLAGS) $< -o $@

object.o: FUNCTIONS/object.cpp FUNCTIONS/object.h
	$(CXX) $(CXXFLAGS) $< -o $@

clean:
	rm *.o && rm $(EXE)
#compiler
CXX = clang++
#compiler flags
CXXFLAGS = -Wall -c -g -std=c++11
#linked frameworks
#LDFLAGS = -framework SDL2 -framework SDL2_image -framework SDL2_ttf
LDFLAGS = -I/usr/local/include -L/usr/local/lib -lSDL2 -lSDL2_image -lSDL2_ttf
#name of executable
#EXE = SDL_LIBRARY
#list of objects
#OBJS = engine.o sprite.o text.o object.o
OBJS = engine.o timer.o sprite.o object.o text.o

#all: $(EXE)

#$(EXE): $(OBJS)
#	$(CXX) $(LDFLAGS) $(OBJS) -o $@

all: $(OBJS)
	#$(CXX) -dynamiclib $(OBJS) -o lib.dylib -install_name @rpath/lib.dylib $(LDFLAGS)
	ar -r lib.a $(OBJS)

engine.o: FUNCTIONS/engine.cpp FUNCTIONS/engine.h
	$(CXX) $(CXXFLAGS) $< -o $@

sprite.o: FUNCTIONS/sprite.cpp FUNCTIONS/sprite.h
	$(CXX) $(CXXFLAGS) $< -o $@

text.o: FUNCTIONS/text.cpp FUNCTIONS/text.h
	$(CXX) $(CXXFLAGS) $< -o $@

object.o: FUNCTIONS/object.cpp FUNCTIONS/object.h
	$(CXX) $(CXXFLAGS) $< -o $@

timer.o: FUNCTIONS/timer.cpp FUNCTIONS/timer.h
	$(CXX) $(CXXFLAGS) $< -o $@

clean:
	rm *.o && rm *.a && rm $(EXE)
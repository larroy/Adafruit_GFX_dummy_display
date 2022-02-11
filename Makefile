EXEC=dummy-display-example
CXX=g++
SDL_CFLAGS=$(shell sdl2-config --cflags)
SDL_LDFLAGS=$(shell sdl2-config --libs)
CXXFLAGS+=-W -Wall -std=c++11 -DARDUINO=123 -ansi -pedantic -I./ -I./Adafruit-GFX-Library $(SDL_CFLAGS)
LDFLAGS=$(SDL_LDFLAGS)
SRC= $(wildcard *.cpp ./Adafruit-GFX-Library/Adafruit_GFX.cpp)
OBJ= $(SRC:.cpp=.o)

all: $(EXEC)

$(EXEC): $(OBJ)
	$(CXX) -o $@ $^ $(LDFLAGS)

%.o: %.cpp
	$(CXX) -o $@ -c $< $(CXXFLAGS)

.PHONY: clean mrproper

clean:
	@rm -rf *.o

mrproper: clean
	@rm -rf $(EXEC)

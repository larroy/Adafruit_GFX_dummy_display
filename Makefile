EXEC=dummy-display-example
CXX=g++
SDL_CFLAGS=$(shell sdl2-config --cflags)
SDL_LDFLAGS=$(shell sdl2-config --libs)
CXXFLAGS+=-g -std=c++11 -W -Wall -DARDUINO=123 -I./ -I./Adafruit-GFX-Library $(SDL_CFLAGS)
LDFLAGS=$(SDL_LDFLAGS)
SRC= $(wildcard *.cpp ./Adafruit-GFX-Library/Adafruit_GFX.cpp)
OBJ= $(SRC:.cpp=.o)

all: $(EXEC)

$(EXEC): $(OBJ)
	$(CXX) -o $@ $^ $(LDFLAGS)

%.o: %.cpp
	$(CXX) -o $@ -c $< $(CXXFLAGS)

.PHONY: clean mrproper run

clean:
	@rm -rf *.o
	@rm -rf $(EXEC)

mrproper: clean
	@rm -rf $(EXEC)


run:
	./dummy-display-example
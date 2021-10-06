CXX		  := g++
CXX_FLAGS := -g -Wall -Wextra -std=c++17 -Wno-missing-field-initializers -Wno-unused-parameter -Wno-sign-compare -Wno-switch -Wno-narrowing

BIN		:= bin
SRC		:= src
OBJ		:= obj
INC		:= include
INCLUDE	:= ./include/
LIB		:= ./lib/

vpath %.cpp src
vpath %.o obj
vpath %.h include

LIBRARIES	:=  -lmingw32 -lSDL2main -lSDL2 -lSDL2_ttf -lSDL2_image -lSDL2_gfx
EXECUTABLE	:= Game.exe

sources = $(wildcard $(SRC)/*.cpp)
objects  = $(patsubst $(SRC)/%.cpp, $(OBJ)/%.o , $(wildcard $(SRC)/*.cpp))
headers =  $(wildcard $(INC)/*.h)

$(OBJ)/%.o: $(SRC)/%.cpp 
	$(CXX) -c -o $@ $< $(CXX_FLAGS) -I$(INCLUDE) -L$(LIB) $(LIBRARIES)
  
$(BIN)/$(EXECUTABLE): $(objects) 
	$(CXX) -o $@ $^ $(CXX_FLAGS) -I$(INCLUDE) -L$(LIB) $(LIBRARIES)

$(OBJ)/main.o: $(headers) $(sources) 
$(OBJ)/texture.o: texture.h structs.h
$(OBJ)/text.o: text.h texture.h
$(OBJ)/window.o: window.h dimensions.h
$(OBJ)/inputs.o: inputs.h structs.h
$(OBJ)/map.o: map.h entity.h inputs.h structs.h PerlinNoise.h movement.h
$(OBJ)/menu.o: menu.h entity.h text.h inputs.h
$(OBJ)/environment.o: environment.h inputs.h menu.h text.h window.h
$(OBJ)/entity.o: entity.h text.h dimensions.h
$(OBJ)/movement.o: movement.h inputs.h structs.h
$(OBJ)/animation.o: animation.h PerlinNoise.h entity.h inputs.h structs.h movement.h
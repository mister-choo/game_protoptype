#pragma once

#include "structs.h"
#include <utility>

enum Input_Types {
  NON_SPECIAL,
  ENTER = SDLK_RETURN,
  BACKSPACE = SDLK_BACKSPACE,
  ESC = SDLK_ESCAPE,
  UP = SDLK_UP,
  DOWN = SDLK_DOWN,
  LEFT = SDLK_LEFT,
  RIGHT = SDLK_RIGHT,
  NUM_1 = SDLK_1,
  NUM_2 = SDLK_2,
  NUM_3 = SDLK_3,
  NUM_4 = SDLK_4,
  NUM_5 = SDLK_5,
  NUM_6 = SDLK_6,
  MOUSE_CLICK,
  MOUSE_MOTION,
  KEY_DOWN = SDL_KEYDOWN,
  KEY_UP = SDL_KEYUP,
  CLICK_UP = SDL_MOUSEBUTTONUP,
  CLICK_DOWN = SDL_MOUSEBUTTONUP,
  NO_INPUT,
  E_KEY = SDLK_e
};

enum Input_Direction {
  UP_Dir = 1,
  DOWN_Dir = 2,
  LEFT_Dir = 4,
  RIGHT_Dir = 8,
};

using Input = std::pair<Input_Types, Input_Types>;

class Event_handler {
private:
  Input Handle_Input_type();

public:
  SDL_Event event;
  const Uint8 *state;
  Input event_output;
  Event_handler();
  int Get_Directions();
  
  int Soft_Handle();
  Point Get_Mouse_coords();
  bool Get_Mouse_state(Point &p);
};

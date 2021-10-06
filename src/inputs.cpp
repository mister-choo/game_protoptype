#include "../include/inputs.h"

Event_handler::Event_handler() { state = SDL_GetKeyboardState(NULL); }

Input Event_handler::Handle_Input_type() {
  if (event.type == SDL_MOUSEMOTION)
    return std::make_pair(NO_INPUT, MOUSE_MOTION);

  else if (event.type == SDL_MOUSEBUTTONUP || event.type == SDL_MOUSEBUTTONDOWN)
    return std::make_pair((Input_Types)event.type, MOUSE_CLICK);

  else if (event.type == SDL_KEYDOWN || event.type == SDL_KEYUP) {
    switch (event.key.keysym.sym) {
    case SDLK_RETURN:
    case SDLK_ESCAPE:
    case SDLK_DOWN:
    case SDLK_LEFT:
    case SDLK_RIGHT:
    case SDLK_UP:
    case SDLK_1:
    case SDLK_2:
    case SDLK_3:
    case SDLK_4:
    case SDLK_5:
    case SDLK_6:
    case SDLK_e:
      return std::make_pair((Input_Types)event.type,
                            (Input_Types)event.key.keysym.sym);
    }
    return std::make_pair(NO_INPUT, NO_INPUT);
  }
  return std::make_pair(NO_INPUT, NO_INPUT);
}

int Event_handler::Soft_Handle() {
  int ret = (SDL_PollEvent(&event));
  event_output = Handle_Input_type();
  return ret;
}
Point Event_handler::Get_Mouse_coords() {
  int x, y;
  SDL_GetMouseState(&x, &y);
  return Point{x, y};
}
bool Event_handler::Get_Mouse_state(Point &p) {
  return SDL_GetMouseState(&p.x, &p.y) & SDL_BUTTON(SDL_BUTTON_LEFT);
}
int Event_handler::Get_Directions() {
  int dir = 0;
  if (state[SDL_SCANCODE_UP])
    dir |= 1;
  if (state[SDL_SCANCODE_DOWN])
    dir |= 2;
  if (state[SDL_SCANCODE_LEFT])
    dir |= 4;
  if (state[SDL_SCANCODE_RIGHT])
    dir |= 8;
  return dir;
}

#pragma once

#include "entity.h"

#include "text.h"
#include <vector>

enum button_state { BUTTON_UNTOCHED, BUTTON_HOVER, BUTTON_CLICK };

enum button_init { BUTTONS_SINGULAR, BUTTONS_DIFFERENT };

class Button {
public:
  Text_box text;
  Sprite button;
  button_state state;
  void Init(Font *font, SDL_Renderer *Renderer,
            button_init button_setting = BUTTONS_SINGULAR, int i = 0);

  void Load_Button(std::string str, Sint16 size);
  void Set_Coords(Point set_coords, Allign_Settings setting,
                  SDL_Rect area = SDL_Rect{0, 0, SCREEN_WIDTH, SCREEN_HEIGHT});
  void Set_button_state(button_state setting);
  Point Sizes();
  void Render();
  bool in(Point p);
};

class Menu {
public:
  std::vector<Button> buttons;
  SDL_Rect menu_area;
  int free_space;
  SDL_Renderer *Renderer;
  Font *font;

  void Set_Renderer(SDL_Renderer *gRenderer);
  void Set_Font(Font *F);
  void free();
  void Init(std::vector<std::string> button_names, Sint16 text_size,
            button_init button_setting = BUTTONS_SINGULAR);
  Menu() {}
  void Set_Menu_Area(Allign_Settings setting, SDL_Rect menu_area);
  void Render();
  int Targeted(Point coords);
};

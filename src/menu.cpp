#include "../include/menu.h"
#include "inputs.h"

void Button::Init(Font *font, SDL_Renderer *Renderer,
                  button_init button_setting, int i) {
  if (button_setting == BUTTONS_SINGULAR)
    button.Create_Image_Surface("../sprites/buttons.bmp", Renderer);
  else
    button.Create_Image_Surface(
        "../sprites/buttons_second" + std::to_string(i) + ".bmp", Renderer);
  button.Clip_Set_up(Point{1, 3});
  text.texture.Set_Font(font);
  text.texture.SetRenderer(Renderer);
}/////

void Button::Load_Button(std::string str, Sint16 size) {
  text.texture.load_text(str, size);
  text.UpdateSizes_from_src();
  button.Clip_Change(Point{0, 0});
  state = BUTTON_UNTOCHED;
  button.UpdateSizes_from_clip();
}
void Button::Set_Coords(Point set_coords, Allign_Settings setting,
                        SDL_Rect area) {
  if (setting == ALLIGN_NONE) {
    button.coords = set_coords;
  } else {
    button.Allign(setting, area);
    button.coords.y = set_coords.y;
  }
  SDL_Rect button_place = button.GetPlace();
  text.Allign(ALLIGN_CENTER, button_place);
  text.Allign_vertical(ALLIGN_CENTER, button_place);
}
void Button::Set_button_state(button_state setting) {
  state = setting;
  button.Clip_Change(Point{0, (int)setting});
}
Point Button::Sizes() { return Point{button.sizes.w, button.sizes.h}; }
void Button::Render() {
  button.Render();
  text.Render();
}
bool Button::in(Point p) { return p.in_area(button.GetPlace()); }

void Menu::Set_Renderer(SDL_Renderer *gRenderer) { this->Renderer = gRenderer; }
void Menu::Set_Font(Font *F) { font = F; }
void Menu::free() { buttons.clear(); }
void Menu::Init(std::vector<std::string> button_names, Sint16 text_size,
                button_init button_setting) {
  const int size = button_names.size();
  menu_area = {0, 0, 0, 0};
  buttons.resize(size);
  for (int i = 0; i < size; i++) {
    if (button_setting == BUTTONS_SINGULAR) {
      buttons[i].Init(font, Renderer);
    } else
      buttons[i].Init(font, Renderer, BUTTONS_DIFFERENT, i + 1);
    buttons[i].Load_Button(button_names[i], text_size);
    menu_area.h += buttons[i].button.sizes.h;
  }
}

void Menu::Set_Menu_Area(Allign_Settings setting, SDL_Rect area) {
  const int height = menu_area.h;
  if (area.w && area.h) {
    menu_area = area;
    menu_area.h -= (height / buttons.size());
  } else
    menu_area =
        SDL_Rect{0, 0, SCREEN_WIDTH, SCREEN_HEIGHT - (height / buttons.size())};
  int acum = 0;
  const int dh = menu_area.h - height <= 0
                     ? 0
                     : (menu_area.h - height) / (buttons.size() - 1);
  for (auto &i : buttons) {
    i.Set_Coords(Point{0, menu_area.y + (i.Sizes().y + dh) * (acum++)}, setting,
                 menu_area);
  }
  free_space = dh;
}
void Menu::Render() {
  Point coords;
  button_state state =
      SDL_GetMouseState(&coords.x, &coords.y) & SDL_BUTTON(SDL_BUTTON_LEFT)
          ? BUTTON_CLICK
          : BUTTON_HOVER;
  for (auto &i : buttons) {
    i.Set_button_state(i.in(coords) ? state : BUTTON_UNTOCHED);
    i.Render();
  }
}
int Menu::Targeted(Point coords) {
  for (int i = 0; i < buttons.size(); i++) {
    if (buttons[i].in(coords)) {
      return i;
    }
  }
  return -1;
}
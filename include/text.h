#pragma once

#include "texture.h"
#include <SDL2/SDL_ttf.h>
#include <map>
#include <string>

struct Font {
  std::string name;
  std::map<Sint16, TTF_Font *> font_bunches;
  bool empty;
  Font();
  void free();
  Font(std::string set_name) : name(set_name) {}
  bool Add_New_Font(Sint16 size);
  TTF_Font *get_font(Sint16 size);
  ~Font();
};

class Text : public Texture {
private:
  Font *font;
  std::string SavedText;

public:
  int saved_size;
  SDL_Color textColor;
  Text();
  Text(SDL_Renderer *Renderer);

  bool load_text(std::string Text_String, int size = 0,
                 SDL_Renderer *Renderer = NULL);

  void Set_Font(Font *set_font);
  ~Text();
};

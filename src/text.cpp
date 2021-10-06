#include "../include/text.h"

bool Font::Add_New_Font(Sint16 size) {
  TTF_Font *new_font = TTF_OpenFont(name.c_str(), size);
  empty = false;
  if (!new_font)
    return false;
  font_bunches.insert(std::make_pair(size, new_font));
  return true;
}
TTF_Font *Font::get_font(Sint16 size) {
  if ((font_bunches.find(size) == font_bunches.end())) {
    Add_New_Font(size);
    return font_bunches[size];
  } else
    return font_bunches[size];
}
void Font::free() {
  if (!empty && font_bunches.size())
    for (auto &x : font_bunches) {
      if (x.second) {
        // TTF_CloseFont(x.second);
        x.second = nullptr;
      }
      /// font_bunches.erase(x.first);
    }
  empty = true;
}
Font::Font() { empty = true; }
Font::~Font() {
  if (!empty)
    free();
}

Text::Text() {
  saved_size = 0;
  textColor = {1, 1, 1};
  SavedText = "Standart_Text";
}
Text::Text(SDL_Renderer *Renderer) {
  saved_size = 0;
  textColor = {1, 1, 1};
  SavedText = "Standart_Text";
  gRenderer = Renderer;
}

bool Text::load_text(std::string Text_String, int size,
                     SDL_Renderer *Renderer) {
  free();
  if (size != 0)
    saved_size = size;
  else if (size == 0 && saved_size == 0)
    return false;
  if (gRenderer == nullptr && Renderer == nullptr)
    return false;
  else if (gRenderer == nullptr)
    gRenderer = Renderer;
  SavedText = Text_String;

  SDL_Surface *textSurface = TTF_RenderUTF8_Blended(
      font->get_font(saved_size), SavedText.c_str(), textColor);
  if (textSurface == nullptr) {
    printf("Unable to render text surface! SDL_ttf Error: %s\n",
           TTF_GetError());
  } else {
    mTexture = SDL_CreateTextureFromSurface(gRenderer, textSurface);
    if (mTexture == nullptr) {
      printf("Unable to create texture from rendered text! SDL Error: %s\n",
             SDL_GetError());
    } else {
      mWidth = textSurface->w;
      mHeight = textSurface->h;
    }
    SDL_FreeSurface(textSurface);
  }
  return mTexture != nullptr;
}
void Text::Set_Font(Font *set_font) { font = set_font; }
Text::~Text() {}
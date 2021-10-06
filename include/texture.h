#pragma once

#include "SDL2/SDL.h"
#include "structs.h"
#include <SDL2/SDL_image.h>
#include <string>

class Texture {
public:
  SDL_Texture *mTexture;
  int mWidth;
  int mHeight;
  SDL_Renderer *gRenderer;

  Texture();
  Texture(SDL_Renderer *Renderer);

  virtual ~Texture();
  void SetRenderer(SDL_Renderer *Renderer);
  void free();

  void setColorMode(Uint8 red, Uint8 green, Uint8 blue);
  void setBlendMode(SDL_BlendMode blending);
  void setAlpha(Uint8 alpha);
  bool render(Point c, SDL_Rect *clip = NULL, double angle = 0.0,
              double scaling = 1.0, Point center = {-99, -99},
              SDL_RendererFlip flip = SDL_FLIP_NONE,
              SDL_Renderer *Renderer = NULL);
  int getWidth();
  int getHeight();
};

class Image : public Texture {
public:
  bool loadImage(std::string path, SDL_Renderer *Renderer = NULL);
  bool loadImage_from_surf(SDL_Surface *surf, SDL_Renderer *Renderer);
};

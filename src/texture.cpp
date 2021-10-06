#include "../include/texture.h"
#include "structs.h"
Texture::Texture() {
  mTexture = NULL;
  mWidth = 0;
  mHeight = 0;
  gRenderer = NULL;
}
Texture::Texture(SDL_Renderer *Renderer) {
  mTexture = NULL;
  mWidth = 0;
  mHeight = 0;
  gRenderer = Renderer;
}
Texture::~Texture() { free(); }
void Texture::SetRenderer(SDL_Renderer *Renderer) { gRenderer = Renderer; }

void Texture::free() {
  if (mTexture != NULL) {
    SDL_DestroyTexture(mTexture);
    mTexture = NULL;
    mWidth = 0;
    mHeight = 0;
  }
}

void Texture::setColorMode(Uint8 red, Uint8 green, Uint8 blue) {
  SDL_SetTextureColorMod(mTexture, red, green, blue);
}

void Texture::setBlendMode(SDL_BlendMode blending) {
  SDL_SetTextureBlendMode(mTexture, blending);
}

void Texture::setAlpha(Uint8 alpha) { SDL_SetTextureAlphaMod(mTexture, alpha); }

bool Texture::render(Point c, SDL_Rect *clip, double angle, double scaling,
                     Point center, SDL_RendererFlip flip,
                     SDL_Renderer *Renderer) {
  if (gRenderer == NULL && Renderer == NULL)
    return false;
  else if (gRenderer == NULL)
    gRenderer = Renderer;

  SDL_Rect renderQuad = {c.x, c.y, mWidth, mHeight};
  if (clip != NULL) {
    renderQuad.w = floor(clip->w * scaling);
    renderQuad.h = floor(clip->h * scaling);
    /*SDL_Log(
        "\n\nrenderquad: x: %d y: %d w: %d h: %d\nclip: x: %d y: %d w: %d h: "
        "%d\nscaling: "
        "%f\nflip = %d\n",
        renderQuad.x, renderQuad.y, renderQuad.w, renderQuad.h,
        clip ? clip->x : 0, clip ? clip->y : 0, clip ? clip->w : 0,
        clip ? clip->h : 0, scaling, flip);*/
  }

  SDL_RenderCopyEx(gRenderer, mTexture, clip, &renderQuad, angle,
                   center.GetSDL_Point(), flip);
  return true;
}

int Texture::getWidth() { return mWidth; }

int Texture::getHeight() { return mHeight; }

bool Image::loadImage(std::string path, SDL_Renderer *Renderer) {
  free();
  if (gRenderer == NULL && Renderer == NULL)
    return false;
  else if (gRenderer == NULL)
    gRenderer = Renderer;

  SDL_Texture *newTexture = NULL;
  SDL_Surface *loadedSurface = IMG_Load(path.c_str());
  if (loadedSurface == NULL) {
    printf("Unable to load image %s! SDL_image Error: %s\n", path.c_str(),
           IMG_GetError());
  } else {
    SDL_SetColorKey(loadedSurface, SDL_TRUE,
                    SDL_MapRGB(loadedSurface->format, 0, 0, 0));
    newTexture = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);
    if (newTexture == NULL) {
      printf("Unable to create texture from %s! SDL Error: %s\n", path.c_str(),
             SDL_GetError());
    } else {
      mWidth = loadedSurface->w;
      mHeight = loadedSurface->h;
    }
    SDL_FreeSurface(loadedSurface);
  }
  mTexture = newTexture;
  return mTexture != NULL;
}
bool Image::loadImage_from_surf(SDL_Surface *surf, SDL_Renderer *Renderer) {
  free();
  if (gRenderer == NULL && Renderer == NULL)
    return false;
  else if (gRenderer == NULL)
    gRenderer = Renderer;

  SDL_Texture *newTexture = NULL;
  SDL_Surface *loadedSurface = surf;
  if (loadedSurface == NULL) {
    printf("Unable to load surface !\n");
  } else {
    newTexture = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);
    if (newTexture == NULL) {
      printf("Unable to create texture from! SDL Error: %s\n", SDL_GetError());
    } else {
      mWidth = loadedSurface->w;
      mHeight = loadedSurface->h;
    }
    SDL_FreeSurface(loadedSurface);
  }
  mTexture = newTexture;
  return mTexture != NULL;
}
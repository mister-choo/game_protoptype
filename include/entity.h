#pragma once

#include "dimensions.h"
#include "text.h"

enum Allign_Settings {
  ALLIGN_LEFT,
  ALLIGN_RIGHT,
  ALLIGN_CENTER,
  ALLIGN_UP,
  ALLIGN_DOWN,
  ALLIGN_NONE
};

class Entity {
public:
  Texture *src;
  bool ownerShip;

  SDL_Rect sizes;
  SDL_Rect clip;
  Point coords;
  double angle;
  double scaling;
  Point Rot_center;
  SDL_RendererFlip flip;

  void Allign(Allign_Settings setting,
              SDL_Rect area = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT});
  void Allign_vertical(Allign_Settings setting,
                       SDL_Rect area = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT});
  Entity();
  virtual ~Entity();
  void UpdateSizes_from_src();
  void UpdateSizes_from_clip();
  SDL_Rect GetPlace();
  virtual void Clear_Texture();
  void Tie_Entity_To_Texture(Texture *target);
  void Assign_Entity_To_Texture(Texture *target);
  virtual bool Render();
};

class Sprite : public Entity {
public:
  Point grid_frames_size;
  Point grid_slots;
  SDL_Surface *surf;

  Sprite();
  ~Sprite();
  void free();
  bool Create_Image_Surface(std::string File_name, SDL_Renderer *Renderer);
  bool Create_Surface(std::string File_name);
  bool Create_BlankSurface(int width, int height);
  bool Scale_Surface(double scaling);
  virtual bool Convert_Surface_to_Texture(bool clean, SDL_Renderer *Renderer);
  void UpdateSizes_from_surf();
  void Clip_Change(Point frame);
  void Clip_Set_up(Point slots);
};
class Large_Sprite : public Sprite {
public:
  Large_Sprite();
  int textureN_W, textureN_H;
  SDL_Rect part_size;
  bool Convert_Surface_to_Texture(bool clean, SDL_Renderer *Renderer);
  void Clear_Texture();
  bool Render();
};

class Text_box : public Entity {
public:
  Text texture;

  Text_box() { this->Tie_Entity_To_Texture(&texture); }
};
#include "../include/entity.h"
#include "SDL2/SDL2_rotozoom.h"
void Entity::Allign(Allign_Settings setting, SDL_Rect area) {
  switch (setting) {
  case ALLIGN_LEFT:
    coords.x = area.x;
    break;
  case ALLIGN_RIGHT:
    coords.x = area.w - src->getWidth();
    break;
  case ALLIGN_CENTER:
    coords.x = area.x + area.w / 2 - src->getWidth() / 2;
    break;
  }
}
void Entity::Allign_vertical(Allign_Settings setting, SDL_Rect area) {
  switch (setting) {
  case ALLIGN_UP:
    coords.y = area.y;
    break;
  case ALLIGN_DOWN:
    coords.y = area.h - src->getHeight();
    break;
  case ALLIGN_CENTER:
    coords.y = area.y + area.h / 2 - src->getHeight() / 2;
    break;
  }
}

Entity::Entity() {
  ownerShip = false;
  src = nullptr;
  flip = SDL_FLIP_NONE;
  angle = 0.0;
  scaling = 1.0;
}
SDL_Rect Entity::GetPlace() {
  return SDL_Rect{coords.x, coords.y, sizes.w, sizes.h};
}
Entity::~Entity() {
  if (ownerShip && src != nullptr) {
    src->free();
  }
}
void Entity::UpdateSizes_from_src() {
  sizes = {0, 0, src->getWidth(), src->getHeight()};
  clip = {0, 0, src->getWidth(), src->getHeight()};
}
void Entity::UpdateSizes_from_clip() { sizes = {0, 0, clip.w, clip.h}; }
void Entity::Clear_Texture() {
  if (src && ownerShip)
    src->free();
}

void Entity::Tie_Entity_To_Texture(Texture *target) {
  if (src && ownerShip)
    src->free();
  src = target;
  ownerShip = false;
  UpdateSizes_from_src();
}
void Entity::Assign_Entity_To_Texture(Texture *target) {
  if (src && ownerShip)
    src->free();
  src = target;
  ownerShip = true;
  UpdateSizes_from_src();
}
bool Entity::Render() {
  return src->render(coords, &clip, angle, scaling, Rot_center, flip);
}

Sprite::Sprite() { surf = nullptr; }
void Sprite::free() {
  if (surf != nullptr) {
    SDL_FreeSurface(surf);
    surf = nullptr;
  }
}
Sprite::~Sprite() { free(); }

bool Sprite::Create_Image_Surface(std::string File_name,
                                  SDL_Renderer *Renderer) {
  Clear_Texture();
  Image *img = new Image;
  img->SetRenderer(Renderer);
  if (!img->loadImage(File_name, Renderer)) {
    delete img;
    return false;
  }
  src = img;
  ownerShip = true;
  UpdateSizes_from_src();
  return true;
}
bool Sprite::Create_Surface(std::string File_name) {
  free();

  surf = IMG_Load(File_name.c_str());
  if (surf == NULL) {
    printf("Unable to load image %s! SDL_image Error: %s\n", File_name.c_str(),
           IMG_GetError());
    free();
    return false;
  } else
    SDL_SetColorKey(surf, SDL_TRUE, SDL_MapRGB(surf->format, 0, 0, 0));
  UpdateSizes_from_surf();
  return true;
}
bool Sprite::Create_BlankSurface(int width, int height) {
  Uint32 rmask, gmask, bmask, amask;
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
  rmask = 0xff000000;
  gmask = 0x00ff0000;
  bmask = 0x0000ff00;
  amask = 0x000000ff;
#else
  rmask = 0x000000ff;
  gmask = 0x0000ff00;
  bmask = 0x00ff0000;
  amask = 0xff000000;
#endif
  free();
  surf = SDL_CreateRGBSurface(0, width, height, 32, rmask, gmask, bmask, amask);
  if (surf == NULL) {
    printf("Unable to create surface SDL Error: %s\n", SDL_GetError());
    free();
    return false;
  } else
    SDL_SetColorKey(surf, SDL_TRUE, SDL_MapRGB(surf->format, 0, 0, 0));
  UpdateSizes_from_surf();
  return true;
}
void Sprite::UpdateSizes_from_surf() {
  sizes = {0, 0, surf->w, surf->h};
  clip = {0, 0, surf->w, surf->h};
}
void Sprite::Clip_Change(Point frame) {
  if (frame.x >= 0 && frame.x < grid_slots.x && frame.y >= 0 &&
      frame.y < grid_slots.y) {
    Point temp = frame * grid_frames_size;
    clip = SDL_Rect{temp.x, temp.y, grid_frames_size.x, grid_frames_size.y};
  }
}
void Sprite::Clip_Set_up(Point slots) {
  grid_slots = slots;
  grid_frames_size = Point{sizes.w / (slots.x ? slots.x : 1),
                           sizes.h / (slots.y ? slots.y : 1)};
  clip = SDL_Rect{0, 0, grid_frames_size.x, grid_frames_size.y};
  this->UpdateSizes_from_clip();
}
bool Sprite::Convert_Surface_to_Texture(bool clean, SDL_Renderer *Renderer) {
  Clear_Texture();
  Image *img = new Image;
  img->SetRenderer(Renderer);
  if (!img->loadImage_from_surf(surf, Renderer)) {
    delete img;
    return false;
  }
  surf = nullptr;
  src = img;
  ownerShip = true;
  return true;
}
bool Sprite::Scale_Surface(double scaling) {
  SDL_Surface *new_surf = zoomSurface(surf, scaling, scaling, SMOOTHING_OFF);
  SDL_FreeSurface(surf);
  surf = new_surf;
  return surf != nullptr;
}
void Large_Sprite::Clear_Texture() {
  if (src && ownerShip) {
    delete[] src;
    src = nullptr;
  }
}
Large_Sprite::Large_Sprite() { textureN_W = textureN_H = 1; }
bool Large_Sprite::Convert_Surface_to_Texture(bool clean,
                                              SDL_Renderer *Renderer) {
  Clear_Texture();
  Uint32 rmask, gmask, bmask, amask;
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
  rmask = 0xff000000;
  gmask = 0x00ff0000;
  bmask = 0x0000ff00;
  amask = 0x000000ff;
#else
  rmask = 0x000000ff;
  gmask = 0x0000ff00;
  bmask = 0x00ff0000;
  amask = 0xff000000;
#endif
  int surf_width = (surf->w / 8000) + 1;
  int surf_height = (surf->h / 8000) + 1;
  SDL_Surface *surfs[surf_width * surf_height];
  Image *img = new Image[surf_width * surf_height];
  for (int i_y = 0; i_y < surf_height; i_y++) {
    for (int i_x = 0; i_x < surf_width; i_x++) {
      surfs[i_y * surf_height + i_x] =
          SDL_CreateRGBSurface(0, surf->w / surf_width, surf->h / surf_height,
                               32, rmask, gmask, bmask, amask);
      SDL_Rect clip = {i_x * surf->w / surf_width, i_y * surf->h / surf_height,
                       surf->w / surf_width, surf->h / surf_height};
      SDL_BlitSurface(surf, &clip, surfs[i_y * surf_height + i_x], NULL);
      img[i_y * surf_height + i_x].loadImage_from_surf(
          surfs[i_y * surf_height + i_x], Renderer);
      // SDL_FreeSurface(surfs[i_y * surf_height + i_x]);
    }
  }
  src = img;
  ownerShip = true;
  textureN_W = surf_width;
  textureN_H = surf_height;
  part_size.w = surf->w / surf_width;
  part_size.h = surf->h / surf_height;
  return true;
}
bool Large_Sprite::Render() {
  // SDL_Log("%d %d", coords.x, coords.y);
  SDL_Rect view_port = {coords.x, coords.y, 1920, 1080};
  if (coords.x + 1920 < part_size.w) {
    src[0].render(Point{0, 0}, &view_port, angle, 1, Rot_center, flip);
  } else if (coords.x >= part_size.w) {
    view_port.x -= part_size.w;
    src[1].render(Point{0, 0}, &view_port, angle, 1, Rot_center, flip);
  } else {
    view_port.w = part_size.w - coords.x;
    src[0].render(Point{0, 0}, &view_port, angle, 1, Rot_center, flip);
    view_port.x = 0;
    view_port.w = 1920 - view_port.w;
    src[1].render(Point{part_size.w - coords.x, 0}, &view_port, angle, 1,
                  Rot_center, flip);
  }

  /*bool used[textureN_H][textureN_W];
  SDL_Rect clips[textureN_H][textureN_W];
  Point N1 = Point{coords.x / part_size.x, coords.y / part_size.y};
  Point N2 =
      Point{(coords.x + 1920) / part_size.x, (coords.y + 1080) / part_size.y};*/
  /*Point p = coords, c = Point{0, 0};
  int acum_y = 0;
  for (int i_y = coords.y / part_size.h;
       i_y < (coords.y + 1080) / part_size.h + 1;
       i_y++, p.x = coords.x, p.y = 0, acum_y += view_port.h) {
    for (int i_x = coords.x / part_size.w, acum_x = 0;
         i_x < (coords.x + 1920) / part_size.w + 1;
         i_x++, p.x = 0, p.y = coords.y, acum_x += view_port.w) {
      // p = Point{coords.x%1920, coords.y%1080};
      view_port = SDL_Rect{
          p.x, p.y,
          abs(part_size.w - p.x) > 1920 ? 1920 : abs(part_size.w - p.x),
          abs(part_size.h - p.y) > 1080 ? 1080 : abs(part_size.w - p.x)};
      c = Point{acum_x, acum_y};
      src[i_y * textureN_H + i_x].render(c, &view_port, angle, 1, Rot_center,
                                         flip);
    }
  }*/
  return true;
}
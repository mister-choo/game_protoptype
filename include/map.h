#pragma once

#include "PerlinNoise.h"
#include "entity.h"
#include "inputs.h"
#include "movement.h"
#include "structs.h"
#include <array>
#include <vector>

enum Tile_Neighbours {
  NO_NEIGHBOURS = 0,
  UP_NEIGHBOUR = 1,
  LEFT_NEIGHBOUR = 2,
  RIGHT_NEIGHBOUR = 4,
  DOWN_NEIGHBOUR = 8,

  UP_LEFT_NEIGHBOUR = 3,
  UP_RIGHT_NEIGHBOUR = 5,
  LEFT_RIGHT_NEIGHBOUR = 6,
  UP_DOWN_NEIGHBOUR = 9,
  LEFT_DOWN_NEIGHBOUR = 10,
  RIGHT_DOWN_NEIGHBOUR = 12,

  UP_LEFT_RIGHT_NEIGHBOUR = 7,
  UP_LEFT_DOWN_NEIGHBOUR = 11,
  UP_RIGHT_DOWN_NEIGHBOUR = 13,
  LEFT_RIGHT_DOWN_NEIGHBOUR = 14,

  ALL_NEIGHBOURS = 15,

  NOT_WALL = 16
};
enum Alg_enum { VISITED, NOT_VISITED };
void Fill_Bool_Matrix(Matrix<bool> &map, int width, int height);
void Fill_Alg_Matrix(Matrix<Alg_enum> &map, int width, int height);
void Fill_Int_Matrix(Matrix<int> &map, int width, int height);
class Gate : public Sprite {
public:
  Point enter, exit;
  Point collision_map_enter, collision_map_ext;
  Point *view_port;
  void init(SDL_Renderer *Renderer);
  void Locate_exit(Matrix<bool> &loc);
  void Locate_enter(Matrix<bool> &loc);
  void Fix_collision(Matrix<bool> &collision_matrix);
  bool Render();
};
class Map : public Moving {
private:
  PerlinNoise *P;
  Sprite tile_set;
  Sprite tile_tops;
  Sprite tile_tops_flowers;
  Sprite tile_tops_borders;
  Sprite tile_sides_base;
  Sprite tile_sides_greenery;

  Sprite tile_base;
  Sprite cracks;
  Large_Sprite map_img;

  Point tile_size;
  Tile_Neighbours Check_Neighbours(Point p, Matrix<bool> &matrx,
                                   bool val = false);
  void Set_Clip_On_Sides(Tile_Neighbours neighbours, Sprite &sprite);
  bool Fill_Avaliable(Point &p, std::array<Point, 4> dir_s,
                      std::vector<Point> &aval, Matrix<Alg_enum> &visited);
  void init();
  void Generate_Map(unsigned int seed);
  bool Fit_in(Float_Point &dir);
  void Draw_Border_Greenery(Point p);

public:
  Gate gates;
  Matrix<bool> render_map;
  Matrix<bool> collision_map;
  SDL_Rect check_map;
  Float_Point *player_coords;
  const int width, height;
  bool Area_Сondition(Float_Point dir);
  Map(SDL_Renderer *Renderer, unsigned int seed, int width, int height);
  ~Map();
  bool Move();
  std::vector<Point> Find_places(int N, Uint32 seed, Matrix<int>&foe_map);
  void Render_Map(SDL_Renderer *Renderer);
  void Render();
};

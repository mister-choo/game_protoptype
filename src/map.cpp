#include "../include/map.h"
#include <fstream>
#include <iostream>
#include <stack>
void Fill_Bool_Matrix(Matrix<bool> &map, int width, int height) {
  map.resize(height);
  for (int i = 0; i < height; i++)
    map[i].resize(width);
  for (auto &y : map) {
    for (auto &&x : y) {
      x = false;
    }
  }
}
void Fill_Alg_Matrix(Matrix<Alg_enum> &map, int width, int height) {
  map.resize(height);
  for (int i = 0; i < height; i++)
    map[i].resize(width);
  for (auto &y : map) {
    for (auto &&x : y) {
      x = NOT_VISITED;
    }
  }
}
void Fill_Int_Matrix(Matrix<int> &map, int width, int height) {
  map.resize(height);
  for (int i = 0; i < height; i++)
    map[i].resize(width);
  for (auto &y : map) {
    for (auto &&x : y) {
      x = 0;
    }
  }
}

void Gate::init(SDL_Renderer *Renderer) {
  Create_Image_Surface("../sprites/gates.bmp", Renderer);
  Clip_Set_up(Point{2, 1});
}
void Gate::Locate_enter(Matrix<bool> &loc) {
  for (int y = 0, stop = 0; y < 39 && !stop; y++) {
    for (int x = 0; x < 30; x++) {
      if (loc[y][x] && loc[y][x + 1] && loc[y + 1][x] && loc[y + 1][x + 1] &&
          !(loc[y + 2][x] || loc[y + 2][x + 1] || loc[y + 3][x] ||
            loc[y + 3][x + 1])) {
        enter = Point{(x)*75 * 2, (y)*38 * 2};
        collision_map_enter = Point{(x)*4, (y)*4};
        stop = 1;
        break;
      }
    }
  }
}
void Gate::Locate_exit(Matrix<bool> &loc) {
  for (int y = 60 - 4, stop = 0; y >= 30 && !stop; y--) {
    for (int x = 58; x >= 30; x--) {
      if (loc[y][x] && loc[y][x + 1] && loc[y + 1][x] && loc[y + 1][x + 1] &&
          !(loc[y + 2][x] || loc[y + 2][x + 1] || loc[y + 3][x] ||
            loc[y + 3][x + 1])) {
        exit = Point{(x)*75 * 2, (y)*38 * 2 - 20};
        collision_map_ext = Point{x * 4, (y)*4};
        stop = 1;
        break;
      }
    }
  }
}
void Gate::Fix_collision(Matrix<bool> &collision_matrix) {
  Matrix<bool> collision_matrx_enter;
  Fill_Bool_Matrix(collision_matrx_enter, 8, 13);
  Matrix<bool> collision_matrx_exit;
  Fill_Bool_Matrix(collision_matrx_exit, 8, 13);
  std::ifstream in;
  char ignore_char;
  in.open("../src/matrix1.txt", std::ios_base::in);
  int el;
  for (auto &i : collision_matrx_enter) {
    for (auto &&ii : i) {
      in >> el >> ignore_char;
      ii = el;
    }
  }
  in.close();

  in.open("../src/matrix2.txt", std::ios_base::in);
  for (auto &i : collision_matrx_exit) {
    for (auto &&ii : i) {
      in >> el >> ignore_char;
      ii = el;
    }
  }
  in.close();
  for (int y = 0; y < 13; y++)
    for (int x = 0; x < 8; x++) {
      collision_matrix[collision_map_enter.y + y][collision_map_enter.x + x] =
          collision_matrx_enter[y][x];
      collision_matrix[collision_map_ext.y + y][collision_map_ext.x + x] =
          collision_matrx_exit[y][x];
    }
}
bool Gate::Render() {
  coords = enter - *view_port;
  Clip_Change(Point{0, 0});
  Sprite::Render();
  coords = exit - *view_port;
  Clip_Change(Point{1, 0});
  Sprite::Render();
  return true;
}

void Map::init() {
  tile_set.Create_Surface("../sprites/tiles.bmp");
  tile_base.Create_Surface("../sprites/tile_base.bmp");
  tile_tops.Create_Surface("../sprites/tops.bmp");
  tile_tops_flowers.Create_Surface("../sprites/tops_flowers.bmp");
  tile_tops_borders.Create_Surface("../sprites/tiles_tops_borders.bmp");
  tile_sides_base.Create_Surface("../sprites/tiles_sides_base.bmp");
  tile_sides_greenery.Create_Surface("../sprites/tiles_sides_greenery.bmp");
  cracks.Create_Surface("../sprites/cracks.bmp");
  tile_set.Clip_Set_up(Point{4, 4});
  cracks.Clip_Set_up(Point{4, 4});
  tile_tops.Clip_Set_up(Point{2, 2});
  tile_tops_flowers.Clip_Set_up(Point{4, 4});
  tile_tops_borders.Clip_Set_up(Point{4, 4});
  tile_sides_greenery.Clip_Set_up(Point{4, 4});
  tile_sides_base.Clip_Set_up(Point{4, 4});
  tile_base.Clip_Set_up(Point{4, 4});
  tile_size = {tile_set.clip.w, tile_set.clip.h};
}

Map::Map(SDL_Renderer *Renderer, unsigned int seed, int width, int height)
    : width(width), height(height) {
  init();
  P = new PerlinNoise(seed);
  check_map = SDL_Rect{0, 0, width / 2, height / 2};
  Generate_Map(seed);
  Render_Map(Renderer);
  init_timer();
  gates.init(Renderer);
  Set_Speed(300.0);
}
std::vector<Point> Map::Find_places(int N, Uint32 seed, Matrix<int> &foe_map) {
  srand(seed);
  int count = 1;
  std::vector<Point> p_s;
  while (N) {
    for (int y = 15; y < 50 && N; y++) {
      for (int x = 20; x < 58 && N; x++) {
        if (!(rand() % 20) &&
            !(render_map[y][x] || render_map[y + 1][x] ||
              render_map[y][x + 1] || render_map[y + 1][x + 1])) {
          foe_map[y][2 * x] = count++;
          foe_map[y + 1][2 * x] = count++;
          p_s.push_back(Point{x * tile_size.x * 2, y * tile_size.y * 2});
          N--;
        }
      }
    }
  }
  return p_s;
}
Map::~Map() { delete P; }
void Map::Set_Clip_On_Sides(Tile_Neighbours neighbours, Sprite &sprite) {
  switch (neighbours) {
  case NO_NEIGHBOURS:
    sprite.Clip_Change(Point{2, 2});
    break;
  case ALL_NEIGHBOURS:
    sprite.Clip_Change(Point{3, 2});
    break;
  case UP_NEIGHBOUR:
    sprite.Clip_Change(Point{3, 3});
    break;
  case DOWN_NEIGHBOUR:
    sprite.Clip_Change(Point{1, 3});
    break;
  case LEFT_NEIGHBOUR:
    sprite.Clip_Change(Point{0, 3});
    break;
  case RIGHT_NEIGHBOUR:
    sprite.Clip_Change(Point{2, 3});
    break;
  case UP_LEFT_NEIGHBOUR:
    sprite.Clip_Change(Point{1, 1});
    break;
  case UP_RIGHT_NEIGHBOUR:
    sprite.Clip_Change(Point{0, 1});
    break;
  case LEFT_RIGHT_NEIGHBOUR:
    sprite.Clip_Change(Point{0, 2});
    break;
  case UP_DOWN_NEIGHBOUR:
    sprite.Clip_Change(Point{1, 2});
    break;
  case LEFT_DOWN_NEIGHBOUR:
    sprite.Clip_Change(Point{1, 0});
    break;
  case RIGHT_DOWN_NEIGHBOUR:
    sprite.Clip_Change(Point{0, 0});
    break;
  case UP_LEFT_RIGHT_NEIGHBOUR:
    sprite.Clip_Change(Point{3, 1});
    break;
  case UP_LEFT_DOWN_NEIGHBOUR:
    sprite.Clip_Change(Point{3, 0});
    break;
  case UP_RIGHT_DOWN_NEIGHBOUR:
    sprite.Clip_Change(Point{2, 1});
    break;
  case LEFT_RIGHT_DOWN_NEIGHBOUR:
    sprite.Clip_Change(Point{2, 0});
    break;
  }
}
Tile_Neighbours Map::Check_Neighbours(Point p, Matrix<bool> &matrx, bool val) {
  int return_value = NO_NEIGHBOURS;
  const int x = matrx.size();
  const int y = matrx[0].size();
  if ((p.x + 1 < x) && val == matrx[p.y][p.x + 1])
    return_value |= RIGHT_NEIGHBOUR;
  if ((p.x - 1 >= 0) && val == matrx[p.y][p.x - 1])
    return_value |= LEFT_NEIGHBOUR;
  if ((p.y + 1 < y) && val == matrx[p.y + 1][p.x])
    return_value |= DOWN_NEIGHBOUR;
  if ((p.y - 1 >= 0) && val == matrx[p.y - 1][p.x])
    return_value |= UP_NEIGHBOUR;
  return (Tile_Neighbours)return_value;
}

bool Map::Fill_Avaliable(Point &p, std::array<Point, 4> dir_s,
                         std::vector<Point> &aval, Matrix<Alg_enum> &visited) {
  if (!aval.empty())
    aval.clear();
  for (auto &dir : dir_s) {
    if ((p + dir * 2).in_area_strict(check_map) &&
        visited[p.y / 2 + dir.y][p.x / 2 + dir.x]) {
      aval.push_back(dir);
    }
  }
  return !aval.empty();
}

void Map::Generate_Map(unsigned int seed) {
  srand(seed);
  Matrix<bool> map;
  Matrix<Alg_enum> visited;
  std::stack<Point> path;
  Point cur = Point{0, 0}, dir = {0, 0};
  Fill_Bool_Matrix(map, width / 2, height / 2);

  Fill_Alg_Matrix(visited, width / 4, height / 4);
  int count = 0, count2 = 0;
  for (auto &row : map) {
    count2++;
    for (auto &&el : row)
      el = !(count2 % 2) || (count++ % 2);
  }

  visited[0][0] = VISITED;

  path.push(Point{0, 0});
  std::array<Point, 4> dir_s = std::array<Point, 4>{Point{0, 1}, Point{0, -1},
                                                    Point{1, 0}, Point{-1, 0}};
  std::vector<Point> avaliable;
  while (!path.empty()) {
    if (!Fill_Avaliable(cur, dir_s, avaliable, visited)) {
      cur = path.top();
      path.pop();
    } else {
      dir = avaliable[rand() % avaliable.size()];
      map[(cur + dir).y][(cur + dir).x] = false;
      cur = cur + dir;
      cur = cur + dir;
      visited[cur.y / 2][cur.x / 2] = VISITED;
      path.push(cur);
    }
  }

  P->fill_matrix((double)(rand() % 54) / 10.0, width, height);
  Fill_Bool_Matrix(render_map, width, height);
  Fill_Bool_Matrix(collision_map, width * 4, height * 4);
  for (int y = 0; y < height; y++)
    for (int x = 0; x < width; x++) {
      if (map[y / 2][x / 2])
        render_map[y][x] = (bool)(((int)round(P->M[y][x])));
      else
        render_map[y][x] = false;
    }
  Tile_Neighbours check;
  for (int y = 0; y < height; y++, std::cout << std::endl)
    for (int x = 0; x < width; x++) {
      check = Check_Neighbours(Point{x, y}, render_map, true);
      if (!(check & UP_NEIGHBOUR) && !(check & DOWN_NEIGHBOUR))
        render_map[y][x] = false;
    }
  for (int y = 0; y < height * 4; y++)
    for (int x = 0; x < width * 4; x++) {
      check = Check_Neighbours(Point{x / 4, y / 4}, render_map, true);
      if (render_map[y / 4][x / 4] && !(check & DOWN_NEIGHBOUR))
        collision_map[y][x] = false;
      else
        collision_map[y][x] = render_map[y / 4][x / 4];
    }
  gates.Locate_enter(render_map);
  gates.Locate_exit(render_map);
  gates.Fix_collision(collision_map);
}
void Map::Draw_Border_Greenery(Point p) {
  SDL_Rect new_dest = SDL_Rect{p.x * tile_size.x, p.y * tile_size.y, 0, 0};
  new_dest.x -= 6;
  new_dest.y -= 6;
  Tile_Neighbours check = Check_Neighbours(p, render_map, true), check_wall;
  if (!(check & UP_NEIGHBOUR)) {
    tile_tops_borders.Clip_Change(Point{rand() % 4, 1});
    SDL_BlitSurface(tile_tops_borders.surf, &(tile_tops_borders.clip),
                    map_img.surf, &new_dest);
  } else {
    check_wall = Check_Neighbours(p + Point{0, -1}, render_map, true);
    if ((check_wall & UP_NEIGHBOUR) && !(check_wall & DOWN_NEIGHBOUR)) {
      tile_tops_borders.Clip_Change(Point{rand() % 4, 1});
      SDL_BlitSurface(tile_tops_borders.surf, &(tile_tops_borders.clip),
                      map_img.surf, &new_dest);
    }
  }
  if (!(check & DOWN_NEIGHBOUR)) {
    tile_tops_borders.Clip_Change(Point{rand() % 4, 2});
    SDL_BlitSurface(tile_tops_borders.surf, &(tile_tops_borders.clip),
                    map_img.surf, &new_dest);
  } else {
    check_wall = Check_Neighbours(p + Point{0, 1}, render_map, true);
    if ((check_wall & UP_NEIGHBOUR) && !(check_wall & DOWN_NEIGHBOUR)) {
      tile_tops_borders.Clip_Change(Point{rand() % 4, 2});
      SDL_BlitSurface(tile_tops_borders.surf, &(tile_tops_borders.clip),
                      map_img.surf, &new_dest);
    }
  }

  if (!(check & LEFT_NEIGHBOUR)) {
    tile_tops_borders.Clip_Change(Point{rand() % 4, 0});
    SDL_BlitSurface(tile_tops_borders.surf, &(tile_tops_borders.clip),
                    map_img.surf, &new_dest);
  } else {
    check_wall = Check_Neighbours(p + Point{-1, 0}, render_map, true);
    if ((check_wall & UP_NEIGHBOUR) && !(check_wall & DOWN_NEIGHBOUR)) {
      tile_tops_borders.Clip_Change(Point{rand() % 4, 0});
      SDL_BlitSurface(tile_tops_borders.surf, &(tile_tops_borders.clip),
                      map_img.surf, &new_dest);
    }
  }
  if (!(check & RIGHT_NEIGHBOUR)) {
    tile_tops_borders.Clip_Change(Point{rand() % 4, 3});
    SDL_BlitSurface(tile_tops_borders.surf, &(tile_tops_borders.clip),
                    map_img.surf, &new_dest);
  } else {
    check_wall = Check_Neighbours(p + Point{1, 0}, render_map, true);
    if ((check_wall & UP_NEIGHBOUR) && !(check_wall & DOWN_NEIGHBOUR)) {
      tile_tops_borders.Clip_Change(Point{rand() % 4, 3});
      SDL_BlitSurface(tile_tops_borders.surf, &(tile_tops_borders.clip),
                      map_img.surf, &new_dest);
    }
  }
}
void Map::Render_Map(SDL_Renderer *Renderer) {
  map_img.Create_BlankSurface(width * tile_size.x, height * tile_size.y);
  SDL_Rect dest;
  Tile_Neighbours check;

  for (Uint16 y = 0; y < height; y++) {
    for (Uint16 x = 0; x < width; x++) {
      dest = SDL_Rect{x * tile_size.x, y * tile_size.y, 0, 0};
      if (!render_map[y][x]) {
        check = Check_Neighbours(Point{x, y}, render_map, false);
        Set_Clip_On_Sides(check, tile_set);
        tile_base.Clip_Change(Point{x % 4, y % 4});
        cracks.Clip_Change(Point{rand() % 4, rand() % 4});

        SDL_BlitSurface(tile_base.surf, &(tile_base.clip), map_img.surf, &dest);

        SDL_BlitSurface(cracks.surf, &(cracks.clip), map_img.surf, &dest);
        if (!(rand() % 15)) {
          tile_tops_flowers.Clip_Change(Point{rand() % 4, rand() % 4});
          SDL_BlitSurface(tile_tops_flowers.surf, &(tile_tops_flowers.clip),
                          map_img.surf, &dest);
        }
        SDL_BlitSurface(tile_set.surf, &(tile_set.clip), map_img.surf, &dest);
      } else {
        check = Check_Neighbours(Point{x, y}, render_map, true);
        Set_Clip_On_Sides(check, tile_tops_borders);
        if ((check & UP_NEIGHBOUR) && !(check & DOWN_NEIGHBOUR)) {
          tile_sides_base.Clip_Change(Point{x % 4, y % 4});
          tile_sides_greenery.Clip_Change(Point{rand() % 4, rand() % 4});
          SDL_BlitSurface(tile_sides_base.surf, &(tile_sides_base.clip),
                          map_img.surf, &dest);
          SDL_BlitSurface(tile_sides_greenery.surf, &(tile_sides_greenery.clip),
                          map_img.surf, &dest);
        } else {

          tile_tops.Clip_Change(Point{x % 2, y % 2});
          tile_tops_flowers.Clip_Change(Point{rand() % 4, rand() % 4});
          SDL_BlitSurface(tile_tops.surf, &(tile_tops.clip), map_img.surf,
                          &dest);
          SDL_BlitSurface(tile_tops_flowers.surf, &(tile_tops_flowers.clip),
                          map_img.surf, &dest);
        }
      }
    }
  }
  for (Uint16 y = 0; y < height; y++) {
    for (Uint16 x = 0; x < width; x++) {
      check = Check_Neighbours(Point{x, y}, render_map, true);
      if (render_map[y][x]) {
        if (!((check & UP_NEIGHBOUR) && !(check & DOWN_NEIGHBOUR))) {
          Draw_Border_Greenery(Point{x, y});
        }
      }
    }
  }
  map_img.Scale_Surface(2);
  map_img.UpdateSizes_from_surf();
  map_img.Convert_Surface_to_Texture(true, Renderer);
  double scale = 1;
  Coords = {0.0, 0.0};
  map_img.coords = Point{0, 0};
  map_img.scaling = scale;
  map_img.src->SetRenderer(Renderer);
}
bool Map::Area_Сondition(Float_Point dir) {
  if (Get_Point((Coords + dir + Float_Point{1920.0, 1080.0}))
          .in_area(map_img.sizes) &&
      Get_Point((Coords + dir)).in_area(map_img.sizes))
    return true;
  else
    return false;
}
bool Map::Fit_in(Float_Point &dir) {
  if (!Get_Point((Coords + dir + Float_Point{1920, 1080}))
           .in_area(map_img.sizes)) {
    Float_Point temp = Coords + dir + Float_Point{1920, 1080};
    if (temp.x > map_img.sizes.w)
      dir.x -= temp.x - map_img.sizes.w;
    if (temp.y > map_img.sizes.h)
      dir.y -= temp.y - map_img.sizes.h;
  }
  if (!Get_Point((Coords + dir)).in_area(map_img.sizes)) {
    Float_Point temp = Coords + dir;
    if (temp.x < 0)
      dir.x -= temp.x;
    if (temp.y < 0)
      dir.y -= temp.y;
  }
  return Area_Сondition(dir);
}
bool Map::Move() {
  Float_Point dir = {0, 0};
  Float_Point center = Float_Point{960.0, 540.0};
  Float_Point p = *player_coords - Coords;
  Float_Point r_p = p - center;

  dir += r_p;
  if (Area_Сondition(dir)) {
    Coords += dir;
    return true;
  } else {
    if (Fit_in(dir)) {
      Coords += dir;
      return true;
    } else
      return false;
  }
}

void Map::Render() {
  this->Move();
  map_img.coords.x = (int)round(Coords.x);
  map_img.coords.y = (int)round(Coords.y);

  map_img.Render();
  gates.view_port = &map_img.coords;
  gates.Render();
}

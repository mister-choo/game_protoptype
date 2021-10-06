#pragma once

#include "PerlinNoise.h"
#include "entity.h"
#include "inputs.h"
#include "movement.h"
#include <iostream>
class Bullet : public Sprite, public Moving {
public:
  Float_Point vec;
  Matrix<bool> &map;
  Matrix<int> &foes;
  SDL_Rect check;
  Float_Point &viewport;
  bool shot;
  int hit_enemy_id;
  bool play_animation;
  bool Area_Сondition(Float_Point dir);
  Bullet(Float_Point starting_coords, SDL_Renderer *Renderer, double ang,
         Event_handler &set_event, Matrix<bool> &render_map,
         Matrix<int> &foes_map, SDL_Rect free_area,
         Float_Point &viewport_coords);
  bool Move();
  bool Render();
  Float_Point Get_Relative_Center();
};

class Animated : public Sprite {
public:
  Animated();
  Uint32 tick;
  Uint32 time_per_frame;
  int frame_counter;
  bool on_tick();
  virtual bool Upd_frame();
  virtual bool Upd_frame(int par);
};

class Explosion : public Animated {
public:
  Float_Point *viewport;
  Float_Point Coords;
  bool used;
  bool on_tick();
  bool Upd_frame();

  Explosion();

  void Init(Float_Point starting_coords, double ang, SDL_Renderer *Renderer,
            Float_Point &viewport_coords);
  bool Render();
};
class Weapon : public Animated {
public:
  bool firing;
  bool Upd_frame();
  Weapon();
};
class Enemy : public Moving, public Animated {
public:
  Float_Point &player;
  Float_Point &view_port;
  Float_Point vec;
  bool chasing;
  bool play_damage;
  bool play_killed;
  bool play;
  bool inflict_damage;
  Uint32 tick;
  Timer_bool new_dir;
  Timer_bool interrupt;

  Timer_bool secondary_animation;

  int direction;
  Float_Point cashed_dir;
  SDL_Rect check;
  int count;
  int id;
  int hp;
  
  Animated damaged;
  Animated killed;
  Uint32 base_speed;
  Matrix<bool> &collision;
  Matrix<bool> &rend;
  Matrix<int> &foes;
  const Float_Point scale;
  const Float_Point scale2;
  const Float_Point scale3;
  Point map_C;
  bool Upd_frame();
  bool Eyesight(Matrix<bool> map, Float_Point p1, Float_Point p2);
  Enemy(SDL_Renderer *Renderer, Float_Point &player_coords, Point map_coords,
        Float_Point &view, Matrix<bool> &collision_map,
        Matrix<bool> &render_map, int id_set, Matrix<int> &foe_map);
  bool Area_Сondition(const Float_Point &dir);
  void Upd_pos(const Point &P);
  bool free_space(const Float_Point P);
  void clear_space(Float_Point P);
  bool Move();
  void Play_damage();
  bool Damage_Inflicted();
  virtual bool Render();
};
bool Compare_Enemy(Enemy *first, Enemy *second);
bool Compare_Enemy_id(Enemy *first, Enemy *second);
class Player : public Animated, public Moving_Control {
private:
  bool running;

  Uint32 tick_shot;
  Uint32 tick_reload;
  Uint32 tick_to_stop_reload;
  Uint32 tick_to_start_reload;

  Uint32 time_per_shot;
  Uint32 time_to_reload;
  Uint32 time_to_stop_reload;
  Uint32 time_to_start_reload;
  Uint32 weapon_count;
  bool reloading;
  Sprite eyes;
  Sprite scope;
  Timer_bool damage_timer;
  int direction;

  std::array<Explosion, 8> fx;
  const int scale;
  int bullet_count;
  SDL_Rect check;
  Timer<double> speed_t;
  Timer<Uint32> frame_speed;

  bool Upd_frame();
  bool Area_Сondition(Float_Point dir);
  void Action_on_direction_change(int dir);
  double angle_to_mouse();
  Weapon weapon;
  Animated gun_reloading;
  Sprite clip_anim;
  Sprite barrel;
  Sprite bullet_ui;
  std::vector<Bullet *> bullets;
  std::vector<Enemy *> &enemy;

public:
  Float_Point &viewport;
  Matrix<bool> &map;
  Matrix<int> &foes;
  bool Inflict_Damage();
  bool prompt;
  Player(SDL_Renderer *Renderer, Event_handler &set_event, SDL_Rect free_area,
         Float_Point &viewport_coords, Matrix<bool> &collision_map,
         int set_scale, Matrix<int> &foe_map, std::vector<Enemy *> &enemy_set);
  void Action();
  bool Render();
};

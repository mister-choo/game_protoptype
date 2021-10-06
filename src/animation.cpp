
#include "../include/animation.h"
#include <algorithm>
#include <iostream>

Animated::Animated() {
  tick = 0;
  frame_counter = 0;
}

bool Animated::on_tick() {
  if ((SDL_GetTicks() - tick) >= time_per_frame) {
    tick = SDL_GetTicks();
    return true;
  } else
    return false;
}
bool Animated::Upd_frame() {
  if (on_tick()) {
    Clip_Change(Point{frame_counter++, 0});
    frame_counter %= grid_slots.x;
  }
  return (frame_counter ? (frame_counter % grid_slots.x) : (1));
}
bool Animated::Upd_frame(int par) {
  if (on_tick()) {
    Clip_Change(Point{frame_counter++, par});
    frame_counter %= grid_slots.x;
  }
  return (frame_counter ? (frame_counter % grid_slots.x) : (1));
}
bool Player::Upd_frame() {
  if (!damage_timer.Elapsed_with_stop() && damage_timer.set) {
    Clip_Change(Point{damage_timer.Time() / 100, 4 + direction});
  } else if (running && on_tick()) {
    Clip_Change(Point{frame_counter++, direction});
    frame_counter %= grid_slots.x;
  }
}
bool Explosion::on_tick() {
  if ((SDL_GetTicks() - tick) >= time_per_frame) {
    tick = SDL_GetTicks();
    return true;
  } else
    return false;
}
bool Explosion::Upd_frame() {
  if (on_tick()) {
    Clip_Change(Point{frame_counter++, 0});
  }
  return (frame_counter ? (frame_counter % grid_slots.x) : (1));
}
Explosion::Explosion() {
  used = false;
  time_per_frame = 100;
  Rot_center = Point{40, 52};
}
void Explosion::Init(Float_Point starting_coords, double ang,
                     SDL_Renderer *Renderer, Float_Point &viewport_coords) {
  Create_Image_Surface("../sprites/bulletsplash2.bmp", Renderer);
  Clip_Set_up(Point{5, 1});
  angle = ang;
  Coords = starting_coords;
  frame_counter = 0;
  viewport = &viewport_coords;
  used = true;
  tick = SDL_GetTicks();
}
bool Explosion::Render() {
  if (Upd_frame()) {
    coords.x =
        (int)(round(Coords.x +
                    ((angle > 130 && angle < 270) ? -Rot_center.dbl().x / 2
                                                  : Rot_center.dbl().x / 4) -
                    round(viewport->x)));
    coords.y = (int)(round(Coords.y - Rot_center.dbl().y - round(viewport->y)));
    return src->render(coords, &clip, angle, scaling, Rot_center, flip);
  } else {
    used = false;
    frame_counter = 0;
    return false;
  }
}
Weapon::Weapon() {
  firing = false;
  frame_counter = 0;

  Rot_center = Point{29, 37};

  angle = 0.0;
}
bool Weapon::Upd_frame() {
  if (on_tick()) {
    Clip_Change(Point{firing ? frame_counter++ : frame_counter, 0});
    frame_counter %= grid_slots.x;
  }
}
bool Player::Area_Сondition(Float_Point dir) {
  Float_Point corner1 = (Coords + dir + Float_Point{clip.w, clip.h}),
              corner2 = (Coords + dir);
  Float_Point head1 = corner2 + Float_Point{10, 0},
              head2 = corner2 + Float_Point{78, 0},
              head3 = corner2 + Float_Point{78, 58},
              head4 = corner2 + Float_Point{10, 58},
              legs1 = corner2 + Float_Point{0 + 5, 88 - 5},
              legs2 = corner2 + Float_Point{0 + 5, 101 - 5},
              legs3 = corner2 + Float_Point{85, 88 - 5},
              legs4 = corner2 + Float_Point{85, 101 - 5},
              legs5 = corner2 + Float_Point{24, 121 - 5},
              legs6 = corner2 + Float_Point{64, 121 - 5};
  std::vector<Float_Point> points = std::vector<Float_Point>{
      head1, head2, head3, head4, legs1, legs2, legs3, legs4, legs5, legs6};
  Point check_point;
  if (Get_Point(corner1).in_area(check) && Get_Point(corner2).in_area(check)) {
    for (auto &p : points) {
      check_point = Get_Point_floor(
          p / Float_Point{75.0 / 4.0 * scale, 38.0 / 4.0 * scale});
      if (map[check_point.y][check_point.x]) {
        return false;
      }
    }
    return true;
  } else
    return false;
}
void Player::Action_on_direction_change(int dir) {
  if ((int)dir == 0) {
    frame_counter = 1;
    running = false;
  } else {
    if (!running) {
      frame_counter = 0;
      running = true;
      on_tick();
      speed_t.start();
      frame_speed.start();
    }
    if (dir & (int)UP_Dir) {
      direction = 3;
    } else {
      direction = 2;
    }
    if (dir & (int)LEFT_Dir) {
      flip = SDL_FLIP_HORIZONTAL;
      direction = 0;
    }
    if (dir & (int)RIGHT_Dir) {
      flip = SDL_FLIP_NONE;
      direction = 0;
    }
  }
  Clip_Change(Point{frame_counter, direction});
}

Player::Player(SDL_Renderer *Renderer, Event_handler &set_event,
               SDL_Rect free_area, Float_Point &viewport_coords,
               Matrix<bool> &collision_map, int set_scale, Matrix<int> &foe_map,
               std::vector<Enemy *> &enemy_set)
    : Moving_Control(set_event), viewport(viewport_coords), map(collision_map),
      scale(set_scale), foes(foe_map), enemy(enemy_set) {
  running = false;
  check = free_area;
  time_per_frame = 200;
  frame_counter = 0;
  scope.Create_Image_Surface("../sprites/scope.bmp", Renderer);
  bullet_ui.Create_Image_Surface("../sprites/bullet_ui.bmp", Renderer);
  bullet_ui.coords.y = 980;
  clip_anim.Create_Image_Surface("../sprites/clip.bmp", Renderer);
  barrel.Create_Image_Surface("../sprites/barrel.bmp", Renderer);
  gun_reloading.Create_Image_Surface("../sprites/gun_reloading.bmp", Renderer);
  clip_anim.Clip_Set_up(Point{4, 1});
  barrel.Clip_Set_up(Point{8, 1});
  gun_reloading.frame_counter = 0;
  gun_reloading.Clip_Set_up(Point{4, 1});
  eyes.Create_Image_Surface("../sprites/eyes.bmp", Renderer);
  barrel.Rot_center = Point{barrel.clip.w / 2, barrel.clip.h / 2};
  clip_anim.Rot_center = barrel.Rot_center;
  direction = 0;
  time_per_shot = 300;
  reloading = false;
  bullet_count = 8;
  prompt = false;
  time_to_reload = 450;
  damage_timer.range = 400;
  time_to_stop_reload = 100;
  time_to_start_reload = 100;
  gun_reloading.time_per_frame = time_to_reload / 4;
  tick_shot = SDL_GetTicks();
  bullets.resize(8);
  weapon.Create_Image_Surface("../sprites/gun2.bmp", Renderer);
  weapon.Clip_Set_up(Point{5, 1});
  weapon.Clip_Change(Point{0, 0});
  weapon.time_per_frame = time_per_shot / 5;
  for (auto &b : bullets) {
    b = nullptr;
  }
  Rot_center = {0, 0};
  flip = SDL_FLIP_NONE;
  Set_Speed(400.0);
  speed_t.add_new(400.0, 900.0, 2500);
  frame_speed.add_new(80, 340, 2500, -1);
  Create_Image_Surface("../sprites/player.bmp", Renderer);
  Clip_Set_up(Point{4, 6});
  Clip_Change(Point{1, 0});
  // coords = Point{700, 500};
  // Coords = Float_Point{700, 500};
}
double Player::angle_to_mouse() {
  Point p1 = weapon.coords + weapon.Rot_center;
  Point p2 = event.Get_Mouse_coords();
  Sint16 r = p1.rvector(p2);
  double asine = asin(float_ratio(p2.y - p1.y, r)) * 180.0 / (3.14159265);
  double acose = acos(float_ratio(p2.x - p1.x, r)) * 180.0 / (3.14159265);

  if (asine < 0)
    return 360.0 - acose;
  else
    return acose;
}
bool Player::Inflict_Damage() {
  if (!damage_timer.set) {
    damage_timer.start();
    frame_counter = 0;
    return true;
  }
  return false;
}
void Player::Action() {
  if (running)
    Set_Speed(speed_t.Current());
  time_per_frame = frame_speed.Current();
  this->Move(event.Get_Directions());
}
bool Player::Render() {
  coords.x = (int)(round(Coords.x - viewport.x));
  coords.y = (int)(round(Coords.y - viewport.y));
  Upd_frame();
  weapon.coords = coords + Point{10, 10};
  double ang = angle_to_mouse();
  weapon.angle = ang;
  if (round(ang) >= 90.0 && round(ang) <= 270.0) {
    weapon.flip = SDL_FLIP_VERTICAL;
    weapon.Rot_center.y = weapon.clip.h - 37;
    weapon.coords.y = coords.y + 20 + 23 - 25;
  } else {
    weapon.flip = SDL_FLIP_NONE;
    weapon.Rot_center.y = 37;
    weapon.coords.y = coords.y + 20;
  }
  Point p;
  bool frame_shot = SDL_GetTicks() - tick_shot >= time_per_shot;

  if (!reloading) {
    for (auto &bullet : bullets) {
      if (bullet_count && frame_shot && bullet == nullptr &&
          event.Get_Mouse_state(p)) {
        bullet = new Bullet(
            (Point{weapon.coords.x +
                       ((round(ang) >= 270.0 && round(ang) <= 295.0) ||
                                (round(ang) >= 50.0 && round(ang) <= 145.0)
                            ? 25
                            : 0),
                   coords.y + 47 + 5}
                 .dbl() +
             viewport),
            src->gRenderer, ang, event, map, foes, check, viewport);
        bullet->init_timer();
        frame_shot = false;
        tick_shot = SDL_GetTicks();
        bullet_count--;
        weapon.firing = true;
        weapon.frame_counter = 1;
      }
    }
  }
  if (frame_shot || reloading) {
    weapon.firing = false;
    weapon.frame_counter = 0;
  }
  weapon.Upd_frame();
  for (auto &bullet : bullets) {
    if (bullet && bullet->shot) {
      bullet->Render();
    }
    if (bullet && !bullet->shot) {
      if (bullet->play_animation) {
        for (auto &effect : fx) {
          if (!effect.used) {
            effect.Init(bullet->Coords, bullet->angle, weapon.src->gRenderer,
                        viewport);
            break;
          }
        }
      }
      if (bullet->hit_enemy_id) {
        for (auto &en : enemy) {
          if (en->id == bullet->hit_enemy_id)
            en->Play_damage();
        }
      }
      delete bullet;
      bullet = nullptr;
    }
  }
  if (!reloading &&
      (bullet_count <= 0 ||
       (SDL_GetTicks() - tick_to_start_reload > time_to_start_reload &&
        bullet_count < 8 && event.state[SDL_SCANCODE_R]))) {
    tick_reload = SDL_GetTicks();
    tick_to_stop_reload = tick_reload;
    reloading = true;
    weapon.firing = false;
    weapon.frame_counter = 0;
  } else if (SDL_GetTicks() - tick_to_stop_reload > time_to_stop_reload &&
             reloading && bullet_count && event.state[SDL_SCANCODE_R]) {
    tick_to_start_reload = SDL_GetTicks();
    reloading = false;
    weapon.firing = false;
    weapon.frame_counter = 0;
  }
  Uint32 current_time = SDL_GetTicks() - tick_reload;
  if (reloading && (current_time < time_to_reload)) {
    if ((current_time < time_to_reload - 100)) {
      clip_anim.coords = coords - Point{barrel.clip.w, 0};
      barrel.coords = coords - Point{barrel.clip.w, 0};
      clip_anim.Clip_Change(
          Point{((4 * current_time) / (time_to_reload - 100)), 0});
      barrel.Clip_Change(Point{bullet_count, 0});
      barrel.angle = 0;
      clip_anim.angle = 0;
    } else {
      barrel.angle =
          float_ratio((current_time - (time_to_reload - 100.0)), 100.0) * 45.0;
      clip_anim.angle =
          float_ratio((current_time - (time_to_reload - 100.0)), 100.0) * 45.0;
    }
    barrel.Render();
    clip_anim.Render();
  }
  if (reloading && (current_time >= time_to_reload)) {
    bullet_count++;
    tick_reload = SDL_GetTicks();
    if (bullet_count >= 8) {
      reloading = false;
    }
  }
  if (reloading) {
    gun_reloading.coords = weapon.coords;
    gun_reloading.Upd_frame();
  } else {
    gun_reloading.tick = SDL_GetTicks();
    gun_reloading.frame_counter = 0;
  }

  for (auto &effect : fx) {
    if (effect.used) {
      effect.Render();
    }
  }

  scope.coords =
      event.Get_Mouse_coords() - Point{scope.sizes.w / 2, scope.sizes.h / 2};
  Upd_frame();
  for (int x = 1920 - (50 + 5) * bullet_count, count = 0;
       count < bullet_count && x < 1920 - 10; x += 55, count++) {
    bullet_ui.coords.x = x;
    bullet_ui.Render();
  }
  Point displacement =
      (event.Get_Mouse_coords() -
       (coords +
        (flip == SDL_FLIP_HORIZONTAL ? Point{20, 20} : Point{25, 20}))) /
      Point{1920 / 7, 1080 / 7};
  if (direction == 3) {
    return (reloading ? gun_reloading.Render() : weapon.Render()) &&
           src->render(coords, &clip, angle, scaling, Rot_center, flip) &&
           scope.Render();
  } else {
    eyes.coords =
        coords + (flip == SDL_FLIP_HORIZONTAL ? Point{20, 20} : Point{25, 20}) +
        displacement;
    // eyes.flip = flip;
    return src->render(coords, &clip, angle, scaling, Rot_center, flip) &&
           ((!damage_timer.Elapsed_with_stop() && damage_timer.set)
                ? 1
                : eyes.Render()) &&
           (reloading ? gun_reloading.Render() : weapon.Render()) &&
           scope.Render();
  }
}
int max(int a, int b, int c, int d) {
  return a > b ? (a > c ? (a > d ? a : d) : (c > d ? c : d))
               : (b > c ? (b > d ? b : d) : (c > d ? c : d));
}
bool Bullet::Area_Сondition(Float_Point dir) {
  Float_Point corner1 = (Coords + dir + Float_Point{0, clip.h})
                            .relative_rotate_degrees(
                                Coords + dir + Rot_center.dbl(), angle),
              corner2 = (Coords + dir)
                            .relative_rotate_degrees(
                                Coords + dir + Rot_center.dbl(), angle);

  bool sure_check =
      (int)ceil(corner1.y / (38.0 / 4.0 * 2.0)) < (int)map.size() &&
      (int)ceil(corner2.y / (38.0 / 4.0 * 2.0)) < (int)map.size() &&
      (int)ceil(corner1.x / (75.0 / 4.0 * 2.0)) < map[0].size() &&
      (int)ceil(corner2.x / (75.0 / 4.0 * 2.0)) < (int)map[0].size() &&

      (int)floor(corner1.y / (38.0 / 4.0 * 2.0)) >= 0 &&
      (int)floor(corner2.y / (38.0 / 4.0 * 2.0)) >= 0 &&
      (int)floor(corner1.x / (75.0 / 4.0 * 2.0)) >= 0 &&
      (int)floor(corner2.x / (75.0 / 4.0 * 2.0)) >= 0;

  if (sure_check && corner1.in_area_strictest(check) &&
      corner2.in_area_strictest(check)) {
    if (map[(int)floor(corner1.y / (38.0 / 4.0 * 2.0))]
           [(int)floor(corner1.x / (75.0 / 4.0 * 2.0))] ||
        map[(int)floor(corner2.y / (38.0 / 4.0 * 2.0))]
           [(int)floor(corner2.x / (75.0 / 4.0 * 2.0))] ||
        map[(int)ceil(corner1.y / (38.0 / 4.0 * 2.0))]
           [(int)ceil(corner1.x / (75.0 / 4.0 * 2.0))] ||
        map[(int)ceil(corner2.y / (38.0 / 4.0 * 2.0))]
           [(int)ceil(corner2.x / (75.0 / 4.0 * 2.0))] ||
        foes[(int)floor(corner1.y / (38.0))][(int)floor(corner1.x / (75.0))] ||
        foes[(int)floor(corner2.y / (38.0))][(int)floor(corner2.x / (75.0))] ||
        foes[(int)ceil(corner1.y / (38.0))][(int)ceil(corner1.x / (75.0))] ||
        foes[(int)ceil(corner2.y / (38.0))][(int)ceil(corner2.x / (75.0))]) {
      hit_enemy_id = max(
          foes[(int)floor(corner1.y / (38.0))][(int)floor(corner1.x / (75.0))],
          foes[(int)floor(corner2.y / (38.0))][(int)floor(corner2.x / (75.0))],
          foes[(int)ceil(corner1.y / (38.0))][(int)ceil(corner1.x / (75.0))],
          foes[(int)ceil(corner2.y / (38.0))][(int)ceil(corner2.x / (75.0))]);

      play_animation = true;
      return false;
    }
    return true;
  } else
    return false;
}
Bullet::Bullet(Float_Point starting_coords, SDL_Renderer *Renderer, double ang,
               Event_handler &set_event, Matrix<bool> &render_map,
               Matrix<int> &foes_map, SDL_Rect free_area,
               Float_Point &viewport_coords)
    : map(render_map), viewport(viewport_coords), foes(foes_map) {
  angle = ang;
  Coords = starting_coords;
  check = free_area;
  play_animation = false;
  hit_enemy_id = 0;
  vec =
      Float_Point{cos(ang / 180.0 * 3.14159265), sin(ang / 180.0 * 3.14159265)};
  Coords += vec * 70;
  Create_Image_Surface("../sprites/bullet.bmp", Renderer);
  Set_Speed(2000.0);
  Rot_center = Point{9, 8};
  shot = true;
}
Float_Point Bullet::Get_Relative_Center() {
  return (Coords + Float_Point{clip.w / 2, clip.h / 2})
      .relative_rotate_degrees(Coords + Rot_center.dbl(), angle);
}
bool Bullet::Move() {
  Float_Point dir = Float_Point{vec.x, vec.y} * speed * t();
  if (Area_Сondition(dir)) {
    Coords += dir;
    return true;
  }
  shot = false;
  if (dir.x < 0)
    Coords.x += dir.x;
  return false;
}
bool Bullet::Render() {
  if (this->Move()) {
    coords.x = (int)(round(Coords.x - round(viewport.x)));
    coords.y = (int)(round(Coords.y - round(viewport.y)));
  }
  return src->render(coords, &clip, angle, scaling, Rot_center, flip);
}

bool Compare_Enemy(Enemy *first, Enemy *second) {
  return first->coords.y < second->coords.y;
}
bool Compare_Enemy_id(Enemy *first, Enemy *second) {
  return first->id < second->id;
}
bool Enemy::Upd_frame() {
  if (chasing && on_tick()) {
    Clip_Change(Point{frame_counter++, direction});
    frame_counter %= grid_slots.x;
  } else if (!chasing) {
    Clip_Change(Point{0, direction});
  }
  return (frame_counter ? (frame_counter % grid_slots.x) : (1));
}
bool Enemy::Eyesight(Matrix<bool> map, Float_Point p1, Float_Point p2) {
  Float_Point P1 = p1 / scale3, P2 = p2 / scale3;
  Point P;
  double dr = float_ratio(1, (int)round(P1.rvector(P2)));
  for (double t = 0.1; t < 0.9; t += dr) {
    P = Get_Point((P1 * (1.0 - t) + P2 * t));
    if (map[P.y][P.x])
      return false;
  }
  return true;
}
Enemy::Enemy(SDL_Renderer *Renderer, Float_Point &player_coords,
             Point map_coords, Float_Point &view, Matrix<bool> &collision_map,
             Matrix<bool> &render_map, int id_set, Matrix<int> &foe_map)
    : player(player_coords), view_port(view), collision(collision_map),
      rend(render_map), foes(foe_map), scale2(Float_Point{75.0, 38.0}),
      scale(Float_Point{75.0 * 2.0, 38.0 * 2.0}),
      scale3(Float_Point{75.0 / 4.0 * 2.0, 38.0 / 4.0 * 2.0}) {
  chasing = false;
  Create_Image_Surface("../sprites/enemy.bmp", Renderer);
  damaged.Create_Image_Surface("../sprites/enemy_damaged.bmp", Renderer);
  killed.Create_Image_Surface("../sprites/enemy_killed.bmp", Renderer);
  Clip_Set_up(Point{6, 2});
  Clip_Change(Point{0, 0});
  time_per_frame = 100;
  count = 0;
  id = id_set;
  vec = Float_Point{0.0, 0.0};
  Coords = map_coords.dbl();
  new_dir.range = 120;
  play_damage = false;
  play_killed = false;
  damaged.Clip_Set_up(Point{5, 2});
  killed.Clip_Set_up(Point{7, 2});
  damaged.time_per_frame = 100;
  killed.time_per_frame = 100;
  play = true;
  base_speed = 600.0;
  inflict_damage = false;
  hp = 3;
  new_dir.start_displacement(10 * (id - 1));
  interrupt.range = 20;
  interrupt.start();
  direction = 0;
  map_C = Get_Point(Coords / scale);
  Upd_pos(map_C);

  Set_Speed(600.0);
  check = SDL_Rect{0, 0, 60 * 75 * 2, 60 * 38 * 2};
}

bool Enemy::Area_Сondition(const Float_Point &dir) {
  Float_Point corner2 = (Coords + dir + Point{8, 0}.dbl()),
              corner1 = (corner2 + Point{clip.w - 1, clip.h}.dbl());
  const Float_Point legs1 = corner2 + Float_Point{5, 116},
                    legs2 = corner2 + Float_Point{73, 116},
                    center = corner2 + Float_Point{41, 28};
  std::vector<Float_Point> points =
      std::vector<Float_Point>{legs1, legs2, center};
  Point check_point;
  if (corner1.in_area(check) && corner2.in_area(check)) {
    for (auto &p : points) {
      check_point = Get_Point(p / scale3);
      if (collision[check_point.y][check_point.x]) {
        return false;
      }
    }
    return true;
  } else
    return false;
}
void Enemy::Upd_pos(const Point &P) {
  foes[map_C.y][map_C.x] = 0;
  foes[map_C.y + 1][map_C.x] = 0;
  map_C = P;
  foes[map_C.y][map_C.x] = id;
  foes[map_C.y + 1][map_C.x] = id;
}
bool Enemy::Damage_Inflicted() {
  if (inflict_damage) {
    inflict_damage = false;
    return true;
  }
  return false;
}
bool Enemy::free_space(Float_Point P) {
  Point C = Get_Point(P / scale2);
  if ((foes[C.y][C.x] == id)) {
    return true;
  } else if (foes[C.y][C.x] == 0) {
    Upd_pos(C);
    return true;
  } else
    return false;
}
void Enemy::clear_space(Float_Point P) {
  Point C = Get_Point(P / scale2);
  foes[C.y][C.x] = 0;
  foes[C.y + 1][C.x] = 0;
  foes[map_C.y][map_C.x] = 0;
  foes[map_C.y + 1][map_C.x] = 0;
}
bool Enemy::Move() {
  if (!play || secondary_animation.set) {
    t();
    return false;
  }
  if (new_dir.Elapsed_with_restart() && interrupt.Elapsed()) {
    const double R = Coords.rvector(player);
    if (R <= 1000.0 && R >= 10.0 && Eyesight(collision, Coords, player)) {
      if (!chasing) {
        chasing = true;
        frame_counter = 0;
        t();
        Set_Speed(base_speed);
      }
      vec = Coords.dr(player) / R * speed * (-1.0);
      if (vec.x < 0)
        direction = 1;
      else
        direction = 0;
    } else {
      chasing = false;
      frame_counter = 0;
      return false;
    }
    if (R <= 30.0)
      inflict_damage = true;
  }
  if (chasing && interrupt.Elapsed()) {
    const Float_Point dir = vec * t();
    if (Enemy::Area_Сondition(dir)) {
      if (!free_space(Coords + dir)) {
        Set_Speed(speed / 1.05);
        interrupt.start();
        cashed_dir = Float_Point{dir.x, dir.y};
      } else {
        Coords += dir;
      }
      return true;
    } else {
      chasing = false;
      interrupt.start();
    }
  } else if (chasing && !interrupt.Elapsed()) {
    if (Enemy::Area_Сondition(cashed_dir)) {
      Coords += (cashed_dir *= 0.75);
      t();
      return true;
    } else {
      chasing = false;
      return false;
    }
  }
  return false;
}
bool Enemy::Render() {
  if (!play) {
    return false;
  }

  if (!play_killed && !play_damage) {
    coords = Get_Point(Coords - view_port);
    Upd_frame();
    return Sprite::Render();
  } else if (play_damage && !secondary_animation.Elapsed_with_stop()) {
    damaged.coords = Get_Point(Coords - view_port);
    if (!damaged.Upd_frame(direction)) {
      play_damage = false;
    }
    clear_space(Coords);
    return damaged.Render();
  } else if (play_killed && !secondary_animation.Elapsed_with_stop()) {
    killed.coords =
        Get_Point(Coords - view_port +
                  (direction ? (Float_Point{0, 0}) : (Float_Point{-46, 0})));
    if (!killed.Upd_frame(direction)) {
      play = false;
    }
    clear_space(Coords);
    return killed.Render();
  }
  if (play_damage && secondary_animation.Elapsed_with_stop())
    play_damage = false;
  if (play_killed && secondary_animation.Elapsed_with_stop()) {
    play_killed = false;
    play = false;
    clear_space(Coords);
  }
}

void Enemy::Play_damage() {
  if (play) {
    hp--;
    if (hp) {
      play_damage = true;
      damaged.on_tick();
      secondary_animation.range = 5 * 100;
      secondary_animation.start();
    } else {
      play_damage = false;
      play_killed = true;
      secondary_animation.range = 7 * 100;
      secondary_animation.start();
      killed.on_tick();
    }
  }
}
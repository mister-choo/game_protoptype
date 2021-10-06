#include "../include/movement.h"

Moving_Control::Moving_Control(Event_handler &set_event) : event(set_event) {
  tick = 0;
  Coords = Float_Point{0, 0};
};
void Moving::init_timer() { tick = SDL_GetTicks(); }
double Moving::t() {
  if (!tick) {
    tick = SDL_GetTicks();
    return 0;
  }
  Uint32 new_tick = SDL_GetTicks();
  double t = (new_tick - tick) / 1000.f;
  tick = new_tick;
  return t;
}
Moving::Moving() {
  tick = 0;
  Coords = Float_Point{0, 0};
}
void Moving::Set_Speed(double speed) { this->speed = speed; }
bool Moving_Control::Move(int direct) {
  Float_Point dir = {0, 0};
  double d = speed * t();
  if (direct & (int)UP_Dir)
    dir.y -= d;
  if (direct & (int)DOWN_Dir)
    dir.y += d;
  if (direct & (int)LEFT_Dir)
    dir.x -= d;
  if (direct & (int)RIGHT_Dir)
    dir.x += d;

  Float_Point partial_dir_y = Float_Point{0, dir.y},
              partial_dir_x = Float_Point{dir.x, 0};
  if ((dir.x != 0 || dir.y != 0)) {
    if (Area_Сondition(dir)) {
      Coords += dir;
      Action_on_direction_change(direct);
      return true;
    } else {
      if (Area_Сondition(partial_dir_x)) {
        Coords += partial_dir_x;
        Action_on_direction_change(dir.x > 0 ? (int)RIGHT_Dir : (int)LEFT_Dir);
        return true;
      } else if (Area_Сondition(partial_dir_y)) {
        Coords += partial_dir_y;
        Action_on_direction_change(dir.y > 0 ? (int)DOWN_Dir : (int)UP_Dir);
        return true;
      } else {
        Action_on_direction_change(0);
        return false;
      }
    }
  } else {
    Action_on_direction_change(0);
    return false;
  }
}
void Moving_Control::Action_on_direction_change(int dir) {}

Timer_bool::Timer_bool(Uint32 R) {
  range = R;

  tick = SDL_GetTicks();
}
Timer_bool::Timer_bool() {
  tick = SDL_GetTicks();
  set = false;
}
Uint32 Timer_bool::Time() { return SDL_GetTicks() - tick; }
bool Timer_bool::Elapsed() { return (SDL_GetTicks() - tick) >= range; }
void Timer_bool::start() {
  tick = SDL_GetTicks();
  set = true;
}
void Timer_bool::start_displacement(Uint32 displacement) {
  tick = SDL_GetTicks() + displacement;
  set = true;
}
bool Timer_bool::Elapsed_with_restart() {
  if (Elapsed()) {
    start();
    return true;
  }
  return false;
}
bool Timer_bool::Elapsed_with_stop() {
  if (Elapsed()) {
    set = false;
    return true;
  }
  return false;
}
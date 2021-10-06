#pragma once

#include "SDL2/SDL.h"
#include "inputs.h"
#include "structs.h"
#include <vector>
class Moving {
public:
  Uint32 tick;
  double speed;
  Float_Point Coords;
  Moving();
  void Set_Speed(double speed);
  void init_timer();
  double t();
  virtual bool Area_Сondition(Float_Point dir) { return true; };
  virtual bool Move() { return true; };
};

class Moving_Control : public Moving {
private:
  virtual void Action_on_direction_change(int dir);

public:
  Event_handler &event;
  Moving_Control(Event_handler &set_event);
  virtual bool Move(int direct);
};

template <typename T> struct Timed_value {
  Timed_value(T min_val, T max_val, Uint32 time_to_max)
      : min(min_val), max(max_val), range(time_to_max) {}
  Timed_value() {}
  T min, max;
  Uint32 range;
  Uint32 tick;
};

class Timer_bool {
  long long int tick;

public:
  long long int range;
  bool set;
  Timer_bool();
  Timer_bool(Uint32 R);
  Uint32 Time();
  bool Elapsed();
  bool Elapsed_with_restart();
  bool Elapsed_with_stop();
  void start();
  void start_displacement(Uint32 displacement);
};

template <typename T> class Timer {
private:
  Timed_value<T> t;
  int setting;

public:
  void add_new(T min_val, T max_val, Uint32 time_to_max, int set = 1) {
    t.min = min_val;
    t.max = max_val;
    t.range = time_to_max;
    setting = set;
  }
  void start() { t.tick = SDL_GetTicks(); }
  T Current() {
    Uint32 elapsed = SDL_GetTicks() - t.tick;
    if (elapsed >= t.range) {
      return setting > 0 ? t.max : t.min;
    } else if (elapsed <= 1) {
      return setting > 0 ? t.min : t.max;
    } else
      return (setting > 0 ? t.min : t.max) +
             setting *
                 round((t.max - t.min) * ((double)elapsed / (double)t.range));
  }
};
#pragma once
#include "SDL2/SDL.h"
#include <cstdint>
#include <string>
#define M_PI 3.14159265358979323846264338327950288



double float_ratio(int upper, int lower);
double angle_ratio(int upper, int lower);
Uint16 circle_len(Uint16 r);

struct Float_Point {
  double x, y;
  Float_Point &operator+=(const Float_Point &oth);
  Float_Point &operator-=(const Float_Point &oth);
  Float_Point &operator/=(const double &k);
  Float_Point &operator*=(const int &k);
  Float_Point &operator*=(const double &k);
  Float_Point operator+(const Float_Point &rhs);
  Float_Point operator-(const Float_Point &rhs);
  Float_Point operator*(const int &k);
  Float_Point operator*(const double &k);
  Float_Point operator/(const double &k);
  Float_Point operator/(const Float_Point &oth);
  double mod();
  Float_Point dr(Float_Point &oth);
  Float_Point relative_rotate_degrees(Float_Point rot, double angle);
  
  bool in_area(SDL_Rect area);
  bool in_area_strictest(SDL_Rect area);
  double rvector(Float_Point &oth);
  void print(std::string str = "");
};
struct Point {
  Sint32 x, y;
  SDL_Point *GetSDL_Point();
  Point &operator+=(const Point &oth);
  Point &operator-=(const Point &oth);
  Point &operator*=(const int &k);
  Point &operator/=(const int &k);
  Point operator+(const Point &rhs);
  Point operator-(const Point &rhs);
  Point operator*(const Point &k);
  Point operator*(const int &k);
  Point operator/(const int &k);
  Point operator/(const Point &k);
  Float_Point operator*(const double &k);
  Float_Point dbl();
  bool operator==(const Point &oth);
  void paste(SDL_Rect *target);
  Sint16 rvector(Point &oth);
  Point move(Sint16 dx, Sint16 dy);
  void print(std::string str = "");
  Point dr(Point &oth);
  double mod();
  Point circle_point_dr(Sint16 r, double angle);
  Point relative_rotate_radians(Point rot, double angle);
  Point relative_rotate_degrees(Point rot, double angle);
  Point circle_point(Sint16 r, double angle);
  Point rotate_per_point(Point rot, double angle, double modifier = 1.0);

  bool in_area(SDL_Rect area);
  bool in_area_strict(SDL_Rect area);
  bool in_area_strictest(SDL_Rect area);
};

Point Get_Point(Float_Point p);
Point Get_Point_ceil(Float_Point p);
Point Get_Point_floor(Float_Point p);

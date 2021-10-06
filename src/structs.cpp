#include "../include/structs.h"
#include <iostream>
///


double float_ratio(int upper, int lower) {
  return (double)(((double)upper) / ((double)lower));
}
double angle_ratio(int upper, int lower) {
  return M_PI * float_ratio(upper, lower);
}
Uint16 circle_len(Uint16 r) { return (Uint16)(M_PI * 2.0 * r * 2); }

void Float_Point::print(std::string str) {
  std::cout << str << " x: " << x << " y: " << y << std::endl;
}
Float_Point &Float_Point::operator+=(const Float_Point &oth) {
  x += oth.x;
  y += oth.y;
  return *this;
}
Float_Point &Float_Point::operator-=(const Float_Point &oth) {
  x -= oth.x;
  y -= oth.y;
  return *this;
}
Float_Point &Float_Point::operator*=(const int &k) {
  x *= k;
  y *= k;
  return *this;
}
Float_Point &Float_Point::operator*=(const double &k) {
  x *= k;
  y *= k;
  return *this;
}
Float_Point &Float_Point::operator/=(const double &k) {
  x /= k;
  y /= k;
  return *this;
}
Float_Point Float_Point::operator+(const Float_Point &rhs) {
  Float_Point temp = *this;
  return temp += rhs;
}
Float_Point Float_Point::operator*(const int &k) {
  Float_Point temp = *this;
  return temp *= k;
}
Float_Point Float_Point::operator*(const double &k) {
  Float_Point temp = *this;
  return temp *= k;
}
Float_Point Float_Point::operator-(const Float_Point &rhs) {
  return Float_Point{x - rhs.x, y - rhs.y};
}
Float_Point Float_Point::operator/(const Float_Point &oth) {
  return Float_Point{x / oth.x, y / oth.y};
}
Float_Point Float_Point::operator/(const double &k) {
  return Float_Point{x / k, y / k};
}
double Float_Point::mod() { return sqrt(x * x + y * y); }
Float_Point Float_Point::dr(Float_Point &oth) {
  return Float_Point{x - oth.x, y - oth.y};
}
Float_Point Float_Point::relative_rotate_degrees(Float_Point rot,
                                                 double angle) {
  Float_Point dp = rot.dr(*this);
  double r = dp.mod();
  angle = angle / 180.0 * M_PI + atan(float_ratio(dp.y, dp.x));
  dp = Float_Point{(cos(angle) * r), (sin(angle) * r)};
  return rot - dp;
}
bool Float_Point::in_area(SDL_Rect area) {
  if ((int)round(x) < area.x || (int)round(x) > area.x + area.w ||
      (int)round(y) < area.y || (int)round(y) > area.y + area.h)
    return false;
  return true;
}
bool Float_Point::in_area_strictest(SDL_Rect area) {
  if (floor(x) <= area.x + 5 || ceil(x) >= area.x + area.w - 5 ||
      floor(y) <= area.y + 5 || ceil(y) >= area.y + area.h - 5)
    return false;
  return true;
}

double Float_Point::rvector(Float_Point &oth) {
  double xvector = fabs(oth.x - x);
  double yvector = fabs(oth.y - y);
  return sqrt(xvector * xvector + yvector * yvector);
}
Point Get_Point(Float_Point p) {
  return Point{(int)round(p.x), (int)round(p.y)};
}
Point Get_Point_ceil(Float_Point p) {
  return Point{(int)ceil(p.x), (int)ceil(p.y)};
}
Point Get_Point_floor(Float_Point p) {
  return Point{(int)floor(p.x), (int)floor(p.y)};
}
Float_Point Point::dbl() { return Float_Point{(double)x, (double)y}; }
SDL_Point *Point::GetSDL_Point() {
  if (x == -99 || y == -99) {
    return nullptr;
  }
  return (SDL_Point *)this;
}
Point &Point::operator+=(const Point &oth) {
  x += oth.x;
  y += oth.y;
  return *this;
}
Point &Point::operator-=(const Point &oth) {
  x -= oth.x;
  y -= oth.y;
  return *this;
}
Point &Point::operator*=(const int &k) {
  x *= k;
  y *= k;
  return *this;
}
Point &Point::operator/=(const int &k) {
  x /= k;
  y /= k;
  return *this;
}
Point Point::operator/(const Point &k) { return Point{x / k.x, y / k.y}; }
Point Point::operator+(const Point &rhs) {
  Point temp = *this;
  return temp += rhs;
}
Point Point::operator-(const Point &rhs) {
  Point temp = *this;
  return temp -= rhs;
}

Point Point::operator*(const int &k) {
  Point temp = *this;
  return temp *= k;
}
Point Point::operator*(const Point &k) { return Point{x * k.x, y * k.y}; }

Point Point::operator/(const int &k) {
  Point temp = *this;
  return temp /= k;
}

Float_Point Point::operator*(const double &k) {
  return {(double)(x * k), (double)(y * k)};
}
bool Point::operator==(const Point &oth) {
  if (x == oth.x && y == oth.y)
    return true;
  return false;
}
void Point::paste(SDL_Rect *target) {
  target->x = x;
  target->y = y;
}
Sint16 Point::rvector(Point &oth) {
  Sint16 xvector = abs(oth.x - x);
  Sint16 yvector = abs(oth.y - y);
  return (Sint16)round(sqrt(xvector * xvector + yvector * yvector));
}
Point Point::move(Sint16 dx, Sint16 dy) {
  return {(Sint16)(x + dx), (Sint16)(y + dy)};
}
void Point::print(std::string str) {
  std::cout << str << " x: " << x << " y: " << y << std::endl;
}
Point Point::circle_point_dr(Sint16 r, double angle) {
  Sint16 dx = (Sint16)(cos(angle) * r);
  Sint16 dy = (Sint16)(sin(angle) * r);
  return {dx, dy};
}

Point Point::circle_point(Sint16 r, double angle) {
  Sint16 dx = (Sint16)(cos(angle) * r);
  Sint16 dy = (Sint16)(sin(angle) * r);
  return {(Sint16)(x + dx), (Sint16)(y + dy)};
}
Point Point::rotate_per_point(Point rot, double angle, double modifier) {
  Sint16 r = (Sint16)(rot.rvector(*this) * modifier);
  return rot.circle_point(r, angle);
}
double Point::mod() { return sqrt(x * x + y * y); }
Point Point::dr(Point &oth) { return Point{x - oth.x, y - oth.y}; }
Point Point::relative_rotate_radians(Point rot, double angle) {
  Point dp = dr(rot);
  double r = dp.mod();
  double angle1 = asin(dp.y / r);
  double angle2 = acos(dp.x / r);
  dp = Get_Point({((angle1 + angle) * r), (angle2 + angle) * r});
  return rot + dp;
}
Point Point::relative_rotate_degrees(Point rot, double angle) {
  Point dp = rot.dr(*this);
  double r = dp.mod();
  angle = angle / 180.0 * M_PI + atan(float_ratio(dp.y, dp.x));
  dp = Get_Point({(cos(angle) * r), (sin(angle) * r)});
  return rot - dp;
}

bool Point::in_area(SDL_Rect area) {
  if (x < area.x || x > area.x + area.w || y < area.y || y > area.y + area.h)
    return false;
  return true;
}
bool Point::in_area_strict(SDL_Rect area) {
  if (x < area.x || x >= area.x + area.w || y < area.y || y >= area.y + area.h)
    return false;
  return true;
}
bool Point::in_area_strictest(SDL_Rect area) {
  if (x <= area.x || x >= area.x + area.w || y <= area.y ||
      y >= area.y + area.h)
    return false;
  return true;
}

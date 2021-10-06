#pragma once

#include "inputs.h"
#include "menu.h"
#include "text.h"
#include "window.h"
#include <array>
#include <chrono>
const int WINDOW_AMOUNT = 1;

enum Init_option { INIT_NO_STARTING_SCREEN, INIT_WITH_STARTING_SCREEN };
enum Window_creation_option {
  FOCUS_ON_CREATION = true,
  DONT_FOCUS_ON_CREATION = false
};

class Environment {
private:
  SDL_Color clear_color;
  int New_Shit;
  int init_windows;

public:
  SDL_Renderer *gRenderer;
  bool success;
  Text gTextTexture;
  Image gImageTexture;
  Font gFont;
  std::array<Menu, 2> gMenu;

  std::array<Window, WINDOW_AMOUNT> Windows;

  Event_handler input;
  Environment(Init_option opt = INIT_NO_STARTING_SCREEN,
              std::string first_window_name = "Game_Prototype",
              std::string font_name = "../font/20211.ttf");
  ~Environment();

  bool loadFont(int font_size, std::string font_name);
  // bool loadText(std::string Text_String);
  bool DrawText(std::string Text_String, int x, int y, int size,
                SDL_Rect *clip = NULL, double angle = 0.0,
                Point center = {-99, -99},
                SDL_RendererFlip flip = SDL_FLIP_NONE,
                SDL_Renderer *Renderer = NULL);
  Text *TextObject();
  SDL_Renderer *Renderer();
  void Handle_Events(SDL_Event &e);
  bool All_closed();
  void Refocus(unsigned long long int window_num);
  void SetRendererDrawColor(Uint8 r, Uint8 g, Uint8 b, Uint8 a = 0xFF);
  bool NewWindow(std::string Window_Name = "Blank_name",
                 bool Focus_on_Creation = false);
  void Update();
};

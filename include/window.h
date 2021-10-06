#pragma once

#include "SDL2/SDL.h"
#include <string>

class Window {
private:
  SDL_Window *mWindow;
  SDL_Renderer *mRenderer;
  int mWindowID;
  int mWidth;
  int mHeight;

  bool mMouseFocus;
  bool mKeyboardFocus;
  bool mFullScreen;
  bool mMinimized;
  bool mShown;
	
public:
  bool init_flag;
	bool mClosed;
  Window();
  bool init(std::string window_name);
  void handleEvent(SDL_Event &e);
  void focus();
  void render();
  void free();
  int getWidth();
  int getHeight();
  bool hasMouseFocus();
  bool hasKeyboardFocus();
  bool isMinimized();
  bool isShown();

  SDL_Renderer *Fetch_Renderer();
};

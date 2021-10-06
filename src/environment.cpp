#include "../include/environment.h"

Environment::Environment(Init_option opt, std::string first_window_name,
                         std::string font_name) {
  gRenderer = NULL;
  init_windows = 0;
  clear_color = {0x00, 0x00, 0x00};
  gFont.name = font_name;
  success = true;
  if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
    printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
    success = false;
  } else {
    if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1")) {
      printf("Warning: Linear texture filtering not enabled!");
    }

    int imgFlags = IMG_INIT_PNG;
    if (!(IMG_Init(imgFlags) & imgFlags)) {
      printf("SDL_image could not initialize! SDL_image Error: %s\n",
             IMG_GetError());
      success = false;
    }

    if (TTF_Init() == -1) {
      printf("SDL_ttf could not initialize! SDL_ttf Error: %s\n",
             TTF_GetError());
      success = false;
    }
    switch (opt) {
    case INIT_WITH_STARTING_SCREEN:
      if (!Windows[0].init(first_window_name)) {
        printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        success = false;
        break;
      } else {
        init_windows = 1;
        gRenderer = Windows[0].Fetch_Renderer();
        SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
      }
      SDL_RenderSetLogicalSize(gRenderer, SCREEN_WIDTH, SCREEN_HEIGHT);
      gTextTexture.SetRenderer(gRenderer);
      gTextTexture.Set_Font(&gFont);
      gImageTexture.SetRenderer(gRenderer);
      gMenu[0].Set_Renderer(gRenderer);
      gMenu[0].Set_Font(&gFont);
      gMenu[1].Set_Renderer(gRenderer);
      gMenu[1].Set_Font(&gFont);
      break;
    case INIT_NO_STARTING_SCREEN:
      break;
    }
  }
}

void Environment::Update() {
  SDL_Renderer *Renderer;
  for (auto &i : Windows) {
    Renderer = i.Fetch_Renderer();
    if (Renderer != NULL) {
      SDL_RenderPresent(Renderer);
      SDL_Color temp;
      SDL_GetRenderDrawColor(Renderer, &temp.r, &temp.g, &temp.b, &temp.a);
      this->SetRendererDrawColor(clear_color.r, clear_color.g, clear_color.b);
      SDL_RenderClear(Renderer);
      this->SetRendererDrawColor(temp.r, temp.g, temp.b, temp.a);
    }
  }
}

Environment::~Environment() {
  gTextTexture.free();
  gImageTexture.free();
  gFont.free();
  for (auto &Window : Windows) {
    Window.free();
  }
  TTF_Quit();
  IMG_Quit();
  SDL_Quit();
}
void Environment::Refocus(unsigned long long int window_num) {
  if (window_num <= Windows.size()) {
    gRenderer = Windows[window_num].Fetch_Renderer();
    gTextTexture.SetRenderer(gRenderer);
    gImageTexture.SetRenderer(gRenderer);
  }
}
bool Environment::DrawText(std::string Text_String, int x, int y, int size,
                           SDL_Rect *clip, double angle, Point center,
                           SDL_RendererFlip flip, SDL_Renderer *Renderer) {
  if (gTextTexture.load_text(Text_String, size, Renderer))
    return gTextTexture.render(Point{x, y}, clip, angle, 1.0, center, flip,
                               Renderer);
  else
    return false;
}
bool Environment::NewWindow(std::string Window_Name, bool Focus_on_Creation) {
  bool result = false;
  if (init_windows + 1 <= Windows.size()) {
    result = Windows[init_windows].init(Window_Name);
    if (result && Focus_on_Creation) {
      gRenderer = Windows[init_windows++].Fetch_Renderer();
      gTextTexture.SetRenderer(gRenderer);
      gImageTexture.SetRenderer(gRenderer);
    }
  }
  return result;
}
void Environment::Handle_Events(SDL_Event &e) {
  for (auto &i : Windows) {
    if (i.init_flag) {
      i.handleEvent(e);
    }
  }
}
bool Environment::All_closed() {
  int count = 0;
  for (auto &i : Windows) {
    if (i.init_flag && i.mClosed && !i.isShown()) {
      count++;
    }
  }
  if (count == Windows.size())
    return true;
  else
    return false;
}
SDL_Renderer *Environment::Renderer() { return gRenderer; }
Text *Environment::TextObject() { return (&gTextTexture); }
void Environment::SetRendererDrawColor(Uint8 r, Uint8 g, Uint8 b, Uint8 a) {
  SDL_SetRenderDrawColor(gRenderer, r, g, b, a);
}
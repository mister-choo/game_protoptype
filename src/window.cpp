#include "../include/window.h"
#include "../include/dimensions.h"
Window::Window() {
  mWindow = NULL;
  mRenderer = NULL;
  mMouseFocus = false;
  mKeyboardFocus = false;
  mFullScreen = false;
  mShown = false;
  mClosed = true;
  mWindowID = -1;

  init_flag = false;

  mWidth = 0;
  mHeight = 0;
}

bool Window::init(std::string window_name) {
  mWindow = SDL_CreateWindow(window_name.c_str(), SDL_WINDOWPOS_UNDEFINED,
                             SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH,
                             SCREEN_HEIGHT, SDL_WINDOW_RESIZABLE);

  if (mWindow != NULL) {
    mMouseFocus = true;
    init_flag = true;
    mKeyboardFocus = true;
    mWidth = SCREEN_WIDTH;
    mHeight = SCREEN_HEIGHT;
    Uint32 flag = SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC;
    mRenderer = SDL_CreateRenderer(mWindow, -1, flag);
    if (mRenderer == NULL) {
      printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
      SDL_DestroyWindow(mWindow);
      mWindow = NULL;
    } else {
      SDL_SetRenderDrawColor(mRenderer, 0x00, 0x00, 0x00, 0xFF);
      mWindowID = SDL_GetWindowID(mWindow);
      mShown = true;
    }
  } else {
    printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
  }
  return mWindow != NULL && mRenderer != NULL;
}
void Window::handleEvent(SDL_Event &e) {
  if (e.type == SDL_WINDOWEVENT && e.window.windowID == mWindowID) {
    switch (e.window.event) {
    case SDL_WINDOWEVENT_SHOWN:
      mShown = true;
      break;

    case SDL_WINDOWEVENT_HIDDEN:
      mShown = false;
      break;

    case SDL_WINDOWEVENT_SIZE_CHANGED:
      mWidth = e.window.data1;
      mHeight = e.window.data2;
      SDL_RenderPresent(mRenderer);
      break;

    case SDL_WINDOWEVENT_EXPOSED:
      SDL_RenderPresent(mRenderer);
      break;

    case SDL_WINDOWEVENT_ENTER:
      mMouseFocus = true;
      break;

    case SDL_WINDOWEVENT_LEAVE:
      mMouseFocus = false;
      break;

    case SDL_WINDOWEVENT_FOCUS_GAINED:
      mKeyboardFocus = true;
      break;

    case SDL_WINDOWEVENT_FOCUS_LOST:
      mKeyboardFocus = false;
      break;

    case SDL_WINDOWEVENT_MINIMIZED:
      mMinimized = true;
      break;

    case SDL_WINDOWEVENT_MAXIMIZED:
      mMinimized = false;
      break;

    case SDL_WINDOWEVENT_RESTORED:
      mMinimized = false;
      break;

    case SDL_WINDOWEVENT_CLOSE:
      SDL_HideWindow(mWindow);
      mClosed = true;
      break;
    }
  }
}
SDL_Renderer *Window::Fetch_Renderer() { return mRenderer; }

void Window::focus() {
  if (!mShown) {
    SDL_ShowWindow(mWindow);
  }
  SDL_RaiseWindow(mWindow);
}

void Window::render() {
  if (!mMinimized) {
    // SDL_SetRenderDrawColor( mRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
    SDL_RenderPresent(mRenderer);
    SDL_RenderClear(mRenderer);
  }
}

void Window::free() {
  if (mWindow != NULL) {
    SDL_DestroyWindow(mWindow);
  }
  mMouseFocus = false;
  mKeyboardFocus = false;
  mWidth = 0;
  mHeight = 0;
}

int Window::getWidth() { return mWidth; }

int Window::getHeight() { return mHeight; }

bool Window::hasMouseFocus() { return mMouseFocus; }

bool Window::hasKeyboardFocus() { return mKeyboardFocus; }

bool Window::isMinimized() { return mMinimized; }

bool Window::isShown() { return mShown; }
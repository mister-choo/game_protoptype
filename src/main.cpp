#include <algorithm>
#include <iostream>

//#include "../include/Astar.h"
#include "../include/animation.h"
#include "../include/dimensions.h"
#include "../include/entity.h"
#include "../include/environment.h"
#include "../include/map.h"
#include "../include/structs.h"

Environment *Global_SDL;
#define FPS 120
void print_matrix(Matrix<int> &M) {

  std::cout << "---------------------------------"
               "---------------------------\n";
  std::cout.width(2);

  std::cout.fill('0');
  for (auto &y : M) {
    std::cout << std::endl;
    for (auto &&x : y) {
      std::cout << std::left << x;
    }
  }
  std::cout.width(1);
  std::cout << "\n---------------------------------"
               "---------------------------\n"
            << std::endl
            << std::endl;
}
bool Play(int k) {
  bool quit = false;
  bool dead = false;
  Sprite health;
  Matrix<int> foe_map;
  Fill_Int_Matrix(foe_map, 120, 120);
  health.Create_Image_Surface("../sprites/health.bmp", Global_SDL->gRenderer);
  health.Clip_Set_up(Point{1, 6});
  health.Clip_Change(Point{0, 0});
  health.coords = Point{50, 50};
  int HP = 0;
  Input &event = Global_SDL->input.event_output;
  Map map(Global_SDL->gRenderer, (k + 1) * 999, 60, 60);
  std::vector<Enemy *> enemies;
  Uint32 tick = SDL_GetTicks();
  Player player(Global_SDL->gRenderer, (Global_SDL->input),
                SDL_Rect{0, 0, 60 * 75 * 2, 60 * 38 * 2}, map.Coords,
                map.collision_map, 2, foe_map, enemies);
  map.player_coords = &player.Coords;
  player.Coords = Float_Point{map.gates.enter.x + 105, map.gates.enter.y + 100};
  player.coords = Point{
      (map.gates.enter.x + 105) > 1920 / 2
          ? 1920 / 2
          : (map.gates.enter.x + 105) > 1920 / 2,
      (map.gates.enter.y + 100) > 1080 / 2 ? 1080 : (map.gates.enter.y + 100)};
  Sprite E_prompt;
  E_prompt.Create_Image_Surface("../sprites/E_prompt.bmp",
                                Global_SDL->gRenderer);
  player.init_timer();
  map.init_timer();
  std::vector<Point> enemy_places = map.Find_places(12, (k + 1) * 999, foe_map);

  enemies.resize(12);
  for (int i = 0; i < 12; i++) {
    enemies[i] = new Enemy(Global_SDL->gRenderer, player.Coords,
                           enemy_places[i], map.Coords, map.collision_map,
                           map.render_map, i + 1, foe_map);
  }
  while (!quit) {
    if ((SDL_GetTicks() - tick) > 1000 / FPS) {
      tick = SDL_GetTicks();
      while (Global_SDL->input.Soft_Handle()) {
        switch (event.second) {
        case ESC:
          quit = true;
          break;
        case E_KEY:
          if (player.prompt) {
            quit = true;
          }
          break;
        }
      }
      if (HP == 5) {
        dead = true;
        quit = true;
      }
      player.Action();
      map.Render();
      for (int i = 0; i < 12; i++) {
        enemies[i]->Move();
        if (enemies[i]->Damage_Inflicted()) {
          if (player.Inflict_Damage()) {
            HP++;
          }
        }
      }
      std::sort(enemies.begin(), enemies.end(), Compare_Enemy);
      int flag = 1;
      for (int i = 0; i < 12; i++) {
        if (flag && player.coords.y < enemies[i]->coords.y) {

          player.Render();
          flag = 0;
        }
        enemies[i]->Render();
      }
      if (flag)
        player.Render();
      health.Clip_Change(Point{0, HP});
      health.Render();
      Float_Point ext = map.gates.exit.dbl();
      if (player.Coords.rvector(ext) <= 350) {
        player.prompt = true;
        E_prompt.coords = map.gates.coords;
        E_prompt.Render();
      } else
        player.prompt = false;
      // print_matrix(foe_map);
      Global_SDL->Update();
    }
  }
  for (int i = 0; i < 12; i++) {
    delete enemies[i];
  }

  return dead ? false : k < 2 ? player.prompt : false;
}

void Level_Select() {
  bool quit = false;
  Input &event = Global_SDL->input.event_output;
  Global_SDL->gMenu[1].Init(
      std::vector<std::string>{"Уровень 1", "Уровень 2", "Уровень 3", "Назад"},
      60, BUTTONS_DIFFERENT);
  Global_SDL->gMenu[1].Set_Menu_Area(ALLIGN_CENTER,
                                     SDL_Rect{200, 250, 700, 1080 - 150 * 2});
  Global_SDL->gImageTexture.loadImage("../sprites/background2.bmp");
  Uint32 tick = SDL_GetTicks();
  int k = 3;
  while (!quit) {
    if ((SDL_GetTicks() - tick) > 1000 / FPS) {
      tick = SDL_GetTicks();
      while (Global_SDL->input.Soft_Handle()) {
        switch (event.second) {
        case ESC:
          quit = true;
          break;

        case MOUSE_CLICK:
          if (event.first == CLICK_UP) {
            switch (k = Global_SDL->gMenu[1].Targeted(
                        Global_SDL->input.Get_Mouse_coords())) {
            case 0:
            case 1:
            case 2:
              while (Play(k++))
                ;
              bool sec_quit = false;
              while (1) {
                Sprite end_screen;
                Sprite text_box;

                if (sec_quit)
                  break;
                text_box.Tie_Entity_To_Texture(&Global_SDL->gTextTexture);
                end_screen.Create_Image_Surface(
                    "../sprites/background2_end.bmp", Global_SDL->gRenderer);
                end_screen.Render();
                Global_SDL->gTextTexture.textColor = {255, 255, 255};
                Global_SDL->gTextTexture.load_text("Игра окончена", 80,
                                                   Global_SDL->gRenderer);
                text_box.UpdateSizes_from_src();
                text_box.Allign(ALLIGN_CENTER);
                text_box.Allign_vertical(ALLIGN_CENTER);
                text_box.Render();
                Global_SDL->gTextTexture.load_text(
                    "Нажмите Enter, чтобы выйти.", 40, Global_SDL->gRenderer);
                text_box.UpdateSizes_from_src();
                text_box.coords.y += 5 * text_box.sizes.h;
                text_box.Render();

                while (Global_SDL->input.Soft_Handle()) {
                  switch (event.second) {
                  case ENTER:
                    sec_quit = true;
                    break;
                  }
                }
                Global_SDL->Update();
              }
              break;
            }
          case 3:
            quit = true;
            break;
          }
        }
      }
      Global_SDL->gImageTexture.render(Point{0, 0});
      Global_SDL->gMenu[1].Render();
      Global_SDL->Update();
    }
  }
  Global_SDL->gImageTexture.loadImage("../sprites/background.bmp");
}
/*
void Draw_Rect(std::vector<Point> points) {
  for (int i = 0; i < points.size() - 1; i++) {
    SDL_RenderDrawLine(Global_SDL->gRenderer, points[i].x, points[i].y,
                       points[i + 1].x, points[i + 1].y);
  }
}*/
void Draw_Line(Point P1, Point P2) {
  SDL_RenderDrawLine(Global_SDL->gRenderer, P1.x, P1.y, P2.x, P2.y);
}
int main(int argc, char *argv[]) {
  Environment SDL_system(INIT_WITH_STARTING_SCREEN, "Game_Prototype",
                         "../font/20211.ttf");
  Global_SDL = &SDL_system;
  if (!SDL_system.success) {
    std::cout << "smth wrong" << std::endl;
    return -1;
  }
  bool quit = false;
  Input &event = SDL_system.input.event_output;

  SDL_system.gImageTexture.loadImage("../sprites/background.bmp");
  SDL_system.gMenu[0].Init(std::vector<std::string>{"ИГРАТЬ", "ВЫХОД"}, 80);
  SDL_system.gMenu[0].Set_Menu_Area(ALLIGN_CENTER,
                                    SDL_Rect{200, 200, 700, 700});

  Uint32 tick = SDL_GetTicks();

  while (!quit) {
    if ((SDL_GetTicks() - tick) > 1000 / FPS) {
      tick = SDL_GetTicks();

      while (SDL_system.input.Soft_Handle()) {
        switch (event.second) {
        case ESC:
          quit = true;
          break;
        case MOUSE_CLICK:
          if (event.first == CLICK_UP) {
            switch (SDL_system.gMenu[0].Targeted(
                SDL_system.input.Get_Mouse_coords())) {
            case 0:
              Level_Select();
              break;
            case 1:
              quit = true;
              break;
            }
          }
          break;
        }
      }
      SDL_system.gImageTexture.render(Point{0, 0});
      SDL_system.gMenu[0].Render();
      // test.Render();
      SDL_system.Update();
    }
  }
  return 0;
}
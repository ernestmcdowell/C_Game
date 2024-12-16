#include "renderer.h"

void draw_grid(SDL_Renderer *renderer, Camera *camera, int screen_width, int screen_height) {
  SDL_SetRenderDrawColor(renderer, 169, 169, 169, 255);

  for (int x = -camera->x % GRID_SIZE; x < screen_width; x += GRID_SIZE) {
    SDL_RenderDrawLine(renderer, x, 0, x, screen_height);
  }

  for(int y = -camera->y % GRID_SIZE; y < screen_height; y += GRID_SIZE) {
    SDL_RenderDrawLine(renderer, 0, y, screen_width, y);
  }
}

void draw_player(SDL_Renderer *renderer, Camera *camera, Player *player) {
  SDL_Rect rect = {
    player->x - camera->x,
    player->y - camera->y,
    player->width,
    player->height
  };

  SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
  SDL_RenderFillRect(renderer, &rect);
}

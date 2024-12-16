#ifndef RENDERER_H
#define RENDERER_H

#include <SDL2/SDL.h>
#include "camera.h"
#include "player.h"

#define GRID_SIZE 32

void draw_grid(SDL_Renderer *renderer, Camera *camera, int screen_width, int screen_height);
void draw_player(SDL_Renderer *renderer, Camera *camera, Player *player);


#endif



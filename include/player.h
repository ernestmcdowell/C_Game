#ifndef PLAYER_H
#define PLAYER_H

#include "tile.h"
#include <SDL2/SDL.h>

typedef struct {
  int x, y;
  int width, height;
  int speed;
} Player;

void handle_player_input(Player *player, World *world);
int is_colliding(World *world, int x, int y);

#endif

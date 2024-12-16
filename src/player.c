#include "player.h"
#include "tile.h"

void handle_player_input(Player *player, World *world) {
  const Uint8 *keystate = SDL_GetKeyboardState(NULL);

  int new_x = player->x;
  int new_y = player->y;

  if (keystate[SDL_SCANCODE_W])
    new_y -= player->speed;
  if (keystate[SDL_SCANCODE_S])
    new_y += player->speed;
  if (keystate[SDL_SCANCODE_A])
    new_x -= player->speed;
  if (keystate[SDL_SCANCODE_D])
    new_x += player->speed;

  if (!is_colliding(world, new_x, player->y, player->width, player->height)) {
    player->x = new_x; // Update X if no collision
  } else {
    printf("Collision detected at X: %d\n", new_x);
  }

  if (!is_colliding(world, player->x, new_y, player->width, player->height)) {
    player->y = new_y; // Update Y if no collision
  } else {
    printf("Collision detected at Y: %d\n", new_y);
  }
}

int is_colliding(World *world, int x, int y, int width, int height) {
  // Check each corner of the player's bounding box
  int corners[4][2] = {
      {x, y},                         // Top-left
      {x + width - 1, y},             // Top-right
      {x, y + height - 1},            // Bottom-left
      {x + width - 1, y + height - 1} // Bottom-right
  };

  for (int i = 0; i < 4; i++) {
    int tile_x = corners[i][0] / TILE_SIZE;
    int tile_y = corners[i][1] / TILE_SIZE;

    // Check bounds
    if (tile_x < 0 || tile_x >= world->width || tile_y < 0 ||
        tile_y >= world->height) {
      return 1; // Treat out-of-bounds as collidable
    }

    if (world->tiles[tile_y][tile_x].collidable) {
      return 1; // Collision detected
    }
  }

  return 0; // No collision
}

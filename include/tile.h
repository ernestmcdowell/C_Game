#ifndef TILE_H
#define TILE_H

#include <SDL2/SDL.h>

#define TILE_SIZE 32

typedef enum {
  TILE_GRASS,
  TILE_SAND,
  TILE_WATER,
} TileType;

typedef struct {
  TileType type;
  int collidable;
} Tile;

typedef struct {
  Tile **tiles; // 2D array of tiles
  int width;
  int height;
  SDL_Texture *grass_texture;
  SDL_Texture *sand_texture;
  SDL_Texture *water_texture;
} World;

World *create_world(SDL_Renderer *renderer, const char *grass_path,
                    const char *sand_path, const char *water_path, int width,
                    int height);
void destroy_world(World *world);
void render_world(SDL_Renderer *renderer, World *world, int camera_x,
                  int camera_y, int screen_width, int screen_height);

#endif

#include "tile.h"
#include <SDL2/SDL_image.h>
#include <math.h>
#include <perlin.h>
#include <stdlib.h>
#include <time.h>

float scale = 0.06f; // Scale for noise; smaller values mean larger features

World *create_world(SDL_Renderer *renderer, const char *grass_path,
                    const char *sand_path, const char *water_path, int width,
                    int height) {
  World *world = malloc(sizeof(World));
  if (!world) {
    fprintf(stderr, "Failed to allocate memory for world.\n");
    return NULL;
  }

  world->width = width;
  world->height = height;

  // Load textures
  world->grass_texture = IMG_LoadTexture(renderer, grass_path);
  world->sand_texture = IMG_LoadTexture(renderer, sand_path);
  world->water_texture = IMG_LoadTexture(renderer, water_path);

  if (!world->grass_texture || !world->sand_texture || !world->water_texture) {
    fprintf(stderr, "Failed to load textures.\n");
    free(world);
    return NULL;
  }

  // Allocate tiles
  world->tiles = malloc(sizeof(Tile *) * height);
  for (int y = 0; y < height; y++) {
    world->tiles[y] = malloc(sizeof(Tile) * width);
  }

  // Perlin noise for terrain
  int permutation[1024];
  for (int i = 0; i < 256; i++) {
    permutation[i] = i;
  }
  for (int i = 255; i > 0; i--) {
    int j = rand() % (i + 1);
    int temp = permutation[i];
    permutation[i] = permutation[j];
    permutation[j] = temp;
  }
  for (int i = 0; i < 256; i++) {
    permutation[256 + i] = permutation[i];
  }

  // Procedural terrain generation
  srand((unsigned int)time(NULL));
  float scale = 0.1f; // Scale for Perlin noise
  for (int y = 0; y < height; y++) {
    for (int x = 0; x < width; x++) {
      float nx = x * scale;
      float ny = y * scale;

      float noise_value = perlin(nx, ny, permutation);

      // Assign tile type and collidable property based on noise
      if (noise_value < 0.3) {
        world->tiles[y][x].type = TILE_WATER;
        world->tiles[y][x].collidable = 0; // Water is impassable
      } else if (noise_value < 0.5) {
        world->tiles[y][x].type = TILE_SAND;
        world->tiles[y][x].collidable = 0; // Sand is walkable
      } else {
        world->tiles[y][x].type = TILE_GRASS;
        world->tiles[y][x].collidable = 1; // Grass is walkable
      }

      // Debug output
      printf("Tile (%d, %d): Noise: %.2f, Type: %d, Collidable: %d\n", x, y,
             noise_value, world->tiles[y][x].type,
             world->tiles[y][x].collidable);
    }
  }

  return world;
}

void destroy_world(World *world) {
  for (int y = 0; y < world->height; y++) {
    free(world->tiles[y]);
  }
  free(world->tiles);
  SDL_DestroyTexture(world->grass_texture);
  SDL_DestroyTexture(world->sand_texture);
  SDL_DestroyTexture(world->water_texture);
  free(world);
}

void render_world(SDL_Renderer *renderer, World *world, int camera_x,
                  int camera_y, int screen_width, int screen_height) {
  int start_x = camera_x / TILE_SIZE;
  int start_y = camera_y / TILE_SIZE;
  int end_x = (camera_x + screen_width) / TILE_SIZE + 1;
  int end_y = (camera_y + screen_height) / TILE_SIZE + 1;

  // Clamp indices to prevent out-of-bounds access
  start_x = start_x < 0 ? 0 : start_x;
  start_y = start_y < 0 ? 0 : start_y;
  end_x = end_x > world->width ? world->width : end_x;
  end_y = end_y > world->height ? world->height : end_y;

  for (int y = start_y; y < end_y; y++) {
    for (int x = start_x; x < end_x; x++) {
      Tile *tile = &world->tiles[y][x];
      SDL_Rect dest = {x * TILE_SIZE - camera_x, y * TILE_SIZE - camera_y,
                       TILE_SIZE, TILE_SIZE};

      SDL_Texture *texture = NULL;
      if (tile->type == TILE_GRASS)
        texture = world->grass_texture;
      else if (tile->type == TILE_SAND)
        texture = world->sand_texture;
      else if (tile->type == TILE_WATER)
        texture = world->water_texture;

      if (texture) {
        SDL_RenderCopy(renderer, texture, NULL, &dest);
      }
    }
  }
}

#include "camera.h"
#include "player.h"
#include "renderer.h"
#include "tile.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

int main(int argc, char *argv[]) {
  // Initialize SDL
  if (SDL_Init(SDL_INIT_VIDEO) != 0) {
    fprintf(stderr, "Failed to initialize SDL: %s\n", SDL_GetError());
    return 1;
  }

  // Initialize SDL2_image for PNG support
  if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
    fprintf(stderr, "Failed to initialize SDL2_image: %s\n", IMG_GetError());
    SDL_Quit();
    return 1;
  }

  // Create SDL Window and Renderer
  SDL_Window *window =
      SDL_CreateWindow("2D Procedural Game", SDL_WINDOWPOS_CENTERED,
                       SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_RESIZABLE);
  if (!window) {
    fprintf(stderr, "Failed to create SDL window: %s\n", SDL_GetError());
    IMG_Quit();
    SDL_Quit();
    return 1;
  }

  SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");

  SDL_Renderer *renderer = SDL_CreateRenderer(
      window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
  if (!renderer) {
    fprintf(stderr, "Failed to create SDL renderer: %s\n", SDL_GetError());
    SDL_DestroyWindow(window);
    IMG_Quit();
    SDL_Quit();
    return 1;
  }

  // Initialize Camera, Player, and World
  Camera camera = {0, 0};
  Player player = {400, 300, 32, 32, 5}; // Starting player position and speed
  World *world = create_world(
      renderer,
      "/home/user/Documents/Programming/CProjects/SomeGame/assets/"
      "brushprev.jpg",
      "/home/user/Documents/Programming/CProjects/SomeGame/assets/"
      "sand_1.png",
      "/home/user/Documents/Programming/CProjects/SomeGame/assets/grass_1.png",
      500,
      500); // Replace with actual file paths
  if (!world) {
    fprintf(stderr, "Failed to create world.\n");
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit();
    SDL_Quit();
    return 1;
  }

  int running = 1;
  int screen_width = 800, screen_height = 600;

  // Game Loop
  while (running) {
    SDL_Event event;

    // Handle Events
    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_QUIT) {
        running = 0;
      }
    }

    // Dynamically update the window size
    SDL_GetWindowSize(window, &screen_width, &screen_height);

    // Update Player and Camera
    handle_player_input(&player, world);
    update_camera(&camera, &player, screen_width, screen_height);

    // Clear Renderer
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Black background
    SDL_RenderClear(renderer);

    // Render World and Player
    render_world(renderer, world, camera.x, camera.y, screen_width,
                 screen_height);
    draw_player(renderer, &camera, &player);

    // Present Renderer
    SDL_RenderPresent(renderer);

    // Cap the frame rate
    SDL_Delay(16); // ~60 FPS
  }

  // Cleanup
  destroy_world(world);
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  IMG_Quit();
  SDL_Quit();

  return 0;
}

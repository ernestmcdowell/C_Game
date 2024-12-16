#include "camera.h"

void update_camera(Camera *camera, Player *player, int screen_width,
                   int screen_height) {
  camera->x = player->x - screen_width / 2 + player->width / 2;
  camera->y = player->y - screen_height / 2 + player->height / 2;

  // Clamp camera to prevent negative values
  if (camera->x < 0)
    camera->x = 0;
  if (camera->y < 0)
    camera->y = 0;
}

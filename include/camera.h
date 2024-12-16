#ifndef CAMERA_H
#define CAMERA_H

#include "player.h"

typedef struct {
  int x, y;
} Camera;

void update_camera(Camera *camera, Player *player, int screen_width, int screen_height);

#endif

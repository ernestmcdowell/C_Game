#include <math.h>
#include <stdlib.h>

float fade(float t) { return t * t * t * (t * (t * 6 - 15) + 10); }

float lerp(float a, float b, float t) { return a + t * (b - a); }

float grad(int hash, float x, float y) {
  int h = hash & 15;
  float u = h < 8 ? x : y;
  float v = h < 4 ? y : (h == 12 || h == 14 ? x : 0);
  return ((h & 1) == 0 ? u : -u) + ((h & 2) == 0 ? v : -v);
}

float perlin(float x, float y, int *permutation) {
  int X = (int)floor(x) & 255;
  int Y = (int)floor(y) & 255;

  x -= floor(x);
  y -= floor(y);

  float u = fade(x);
  float v = fade(y);

  int aa = permutation[permutation[X] + Y];
  int ab = permutation[permutation[X] + Y + 1];
  int ba = permutation[permutation[X + 1] + Y];
  int bb = permutation[permutation[X + 1] + Y + 1];

  return lerp(lerp(grad(aa, x, y), grad(ba, x - 1, y), u),
              lerp(grad(ab, x, y - 1), grad(bb, x - 1, y - 1), u), v);
}

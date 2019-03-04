#include <math.h>
#include <stdbool.h>
#include <stdlib.h>

#include "utils.h"

#define FIRE_WIDTH 320
#define FIRE_HEIGHT 168
#define RGB_LENGTH 37 * 3

int rgbs[RGB_LENGTH] = {
  0x07,0x07,0x07,
  0x1F,0x07,0x07,
  0x2F,0x0F,0x07,
  0x47,0x0F,0x07,
  0x57,0x17,0x07,
  0x67,0x1F,0x07,
  0x77,0x1F,0x07,
  0x8F,0x27,0x07,
  0x9F,0x2F,0x07,
  0xAF,0x3F,0x07,
  0xBF,0x47,0x07,
  0xC7,0x47,0x07,
  0xDF,0x4F,0x07,
  0xDF,0x57,0x07,
  0xDF,0x57,0x07,
  0xD7,0x5F,0x07,
  0xD7,0x5F,0x07,
  0xD7,0x67,0x0F,
  0xCF,0x6F,0x0F,
  0xCF,0x77,0x0F,
  0xCF,0x7F,0x0F,
  0xCF,0x87,0x17,
  0xC7,0x87,0x17,
  0xC7,0x8F,0x17,
  0xC7,0x97,0x1F,
  0xBF,0x9F,0x1F,
  0xBF,0x9F,0x1F,
  0xBF,0xA7,0x27,
  0xBF,0xA7,0x27,
  0xBF,0xAF,0x2F,
  0xB7,0xAF,0x2F,
  0xB7,0xB7,0x2F,
  0xB7,0xB7,0x37,
  0xCF,0xCF,0x6F,
  0xDF,0xDF,0x9F,
  0xEF,0xEF,0xC7,
  0xFF,0xFF,0xFF
};

struct rgb {
  int r;
  int g;
  int b;
};

struct rgb palette[RGB_LENGTH / 3];

int fire_pixels[FIRE_WIDTH * FIRE_HEIGHT] = {0};

void spread_fire(int src) {
  int pixel = fire_pixels[src];
  if (pixel == 0) {
    fire_pixels[src - FIRE_WIDTH] = 0;
  } else {
    int rand_idx = (int)round(rand() * 3.0) & 3;
    int dst = src - rand_idx + 1;
    fire_pixels[dst - FIRE_WIDTH] = pixel - (rand_idx & 1);
  }
}

void do_fire() {
  for (int x = 0; x < FIRE_WIDTH; x++) {
    for (int y = 1; y < FIRE_HEIGHT; y++) {
      spread_fire(y * FIRE_WIDTH + x);
    }
  }
}

void render_frame(struct renderer_config *r) {
  do_fire();

  SDL_SetRenderDrawColor(r->renderer, 0xFF, 0xFF, 0xFF, 0xFF);
  SDL_RenderClear(r->renderer);

  for (int y = 0; y < r->height; y++) {
    for (int x = 0; x < r->width; x++) {
      int index = fire_pixels[y * r->width + x];
      struct rgb pixel = palette[index];
      int opacity = 255;
      if (pixel.r == 0x07 && pixel.g == 0x07 && pixel.b == 0x07) {
        opacity = 0;
      }
      SDL_SetRenderDrawColor(r->renderer, pixel.r, pixel.g, pixel.b, opacity);
      SDL_RenderDrawPoint(r->renderer, x, y);
    }
  }

  SDL_RenderPresent(r->renderer);
}

int main(int argc, char *args[]) {

  struct renderer_config r;
  r.title = "DOOM fire effect";
  r.width = FIRE_WIDTH;
  r.height = FIRE_HEIGHT;
  r.window = NULL;
  r.renderer = NULL;

  if (!initialize_SDL(&r)) {
    SDL_Log("SDL failed to initialize: %s", SDL_GetError());
    destroy_SDL(&r);
  }

  bool is_rendering = true;

  for (int i = 0; i < FIRE_WIDTH; i++) {
    fire_pixels[(FIRE_HEIGHT - 1) * FIRE_WIDTH + i] = 36;
  }

  for (int i = 0; i < RGB_LENGTH / 3; i++) {
    palette[i].r = rgbs[i * 3 + 0];
    palette[i].g = rgbs[i * 3 + 1];
    palette[i].b = rgbs[i * 3 + 2];
  }

  while (is_rendering) {

    while (SDL_PollEvent(&(r.event)) != 0) {
      if (r.event.type == SDL_QUIT) {
        is_rendering = false;
      }
    }

    render_frame(&r);
  }

  return 0;
}

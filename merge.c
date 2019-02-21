#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "utils.h"

const int ELEMENTS = 256;

void render(struct renderer_config *r, int a[], int n) {
  SDL_SetRenderDrawColor(r->renderer, 0xFF, 0xFF, 0xFF, 0xFF);
  SDL_RenderClear(r->renderer);

  float rect_width = (float)r->width / n;
  for (int i = 0; i < n; i++) {
    int rect_height = a[i];
    // x, y, w, h
    SDL_Rect fill_rect = {i * rect_width, r->height - rect_height, rect_width,
                          rect_height};
    SDL_SetRenderDrawColor(r->renderer, rand(), rand(), rand(), rand());
    SDL_RenderFillRect(r->renderer, &fill_rect);
  }

  SDL_RenderPresent(r->renderer);
  while (SDL_PollEvent(r->event) != 0) {
    if( r->event->type == SDL_QUIT ) {
      destroy_SDL(&r);
    }
  }
  SDL_Delay(15);
}

void merge(int a[], int b[], int c[], int m, int n) {
  int i = 0, j = 0, k = 0;

  while (i < m && j < n) {
    if (a[i] < b[j]) {
      c[k++] = a[i++];
    } else {
      c[k++] = b[j++];
    }
  }

  while (i < m) {
    c[k++] = a[i++];
  }

  while (j < n) {
    c[k++] = b[j++];
  }
}

void merge_sort(struct renderer_config *r, int key[], int n) {
  int j, k, m;
  int w[n];

  for (m = 1; m < n; m *= 2)
    ;
  if (m != n)
    exit(1);

  for (k = 1; k < n; k *= 2) {
    for (j = 0; j < n - k; j += 2 * k) {
      merge(key + j, key + j + k, w + j, k, k);
    }
    for (j = 0; j < n; ++j) {
      key[j] = w[j];
      render(r, key, n);
    }
  }
}

int main(int argc, char *args[]) {
  int key[ELEMENTS];

  struct renderer_config r;
  r.title = "Merge sort animation";
  r.width = 680;
  r.height = 480;
  r.window = NULL;
  r.renderer = NULL;

  for (int i = 0; i < ELEMENTS; i++) {
    int elt = random_int(0, 300);
    key[i] = elt;
  }

  if (!initialize_SDL(&r)) {
    SDL_Log("SDL failed to initialize: %s", SDL_GetError());
    destroy_SDL(&r);
  }

  merge_sort(&r, key, ELEMENTS);
  SDL_Delay(2000);
  destroy_SDL(&r);
  return 0;
}

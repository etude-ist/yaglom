#include <assert.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "utils.h"

struct element {
  bool top;
  bool bottom;
  bool left;
  bool right;
  int label;
};

struct element store[9000];
int counter[9000];

int find(int a) {
  if (store[a].label == a)
    return a;

  int fa = find(store[a].label);
  store[a].label = fa;
  return fa;
}

void union_elements(int a, int b, int distance) {
  int fa = find(a);
  int fb = find(b);

  if (fa == fb) {
    return false;
  }

  if (counter[fa] <= counter[fb]) {
    counter[fb] += counter[fa];
    store[fa].label = fb;
  } else {
    counter[fa] += counter[fb];
    store[fb].label = fa;
  }

  if (a - distance == b) {
    // handle bottom top;
    store[a].top = false;
    store[b].bottom = false;
  } else if (a + distance == b) {
    // handle top bottom
    store[a].bottom = false;
    store[b].top = false;
  } else if (a + 1 == b) {
    // handle left right
    store[a].right = false;
    store[b].left = false;
  } else if (a - 1 == b) {
    // handle right left
    store[a].left = false;
    store[b].right = false;
  }
}

int make_top(int x, int n) {
  int top = x - n;
  if (top > 0) {
    return top;
  }
  return -1;
}

int make_bottom(int x, int n, int m) {
  int bottom = x + n;
  if (bottom < m) {
    return bottom;
  }
  return -1;
}

int make_right(int x, int n) {
  int right = x + 1;
  if (x / n == right / n) {
    return right;
  }
  return -1;
}

int make_left(int x, int n) {
  int left = x - 1;
  if (x / n == left / n) {
    return left;
  }
  return -1;
}

int random_neighbour(int index, int n, int distance) {
  int top = make_top(index, distance);
  int bottom = make_bottom(index, distance, n);
  int left = make_left(index, distance);
  int right = make_right(index, distance);
  int arr[4] = {top, bottom, left, right};
  int i = random_int(0, 3);
  return arr[i];
}

void render_maze(struct renderer_config *r, int n, int m) {
  SDL_SetRenderDrawColor(r->renderer, 0xFF, 0xFF, 0xFF, 0xFF);
  SDL_RenderClear(r->renderer);
  int x;
  int y;
  int d = r->width / m;
  for (int i = 0; i < n; i++) {
    y = i % m;
    x = i / m;

    SDL_SetRenderDrawColor(r->renderer, rand(), rand(), rand(), rand());
    if (store[i].top) {
      SDL_RenderDrawLine(r->renderer, x * d, y * d, (x + m) * d, y * d);
    }
    if (store[i].bottom) {
      SDL_RenderDrawLine(r->renderer, x * d, (y + m) * d, (x + m) * d,
                         (y + m) * d);
    }
    if (store[i].left) {
      SDL_RenderDrawLine(r->renderer, x * d, y * d, x * d, (y + m) * d);
    }
    if (store[i].right) {
      SDL_RenderDrawLine(r->renderer, (x + m) * d, y * d, (x + m) * d,
                         (y + m) * d);
    }

    SDL_SetRenderDrawColor(r->renderer, 0xFF, 0xFF, 0xFF, 0xFF);
    if (!store[i].top) {
      SDL_RenderDrawLine(r->renderer, x * d, y * d, (x + m) * d, y * d);
    }
    if (!store[i].bottom) {
      SDL_RenderDrawLine(r->renderer, x * d, (y + m) * d, (x + m) * d,
                         (y + m) * d);
    }
    if (!store[i].left) {
      SDL_RenderDrawLine(r->renderer, x * d, y * d, x * d, (y + m) * d);
    }
    if (!store[i].right) {
      SDL_RenderDrawLine(r->renderer, (x + m) * d, y * d, (x + m) * d,
                         (y + m) * d);
    }
  }

  SDL_RenderPresent(r->renderer);
  while (SDL_PollEvent(&(r->event)) != 0) {
    if(r->event.type == SDL_QUIT) {
      destroy_SDL(&r);
    }
  }
  SDL_Delay(10);
}

void kruskal_maze(struct renderer_config *r, int n) {
  int gaps = 0;
  int distance = (int)sqrt(n);
  while (gaps < n - 1) {
    int current = random_int(0, n - 1);
    int neighbour = random_neighbour(current, n, distance);
    if (neighbour > -1) {
      gaps += 1;
      union_elements(current, neighbour, distance);
      render_maze(r, n, distance);
    }
  }
}

int main(int argc, char *args[]) {
  struct renderer_config r;
  r.title = "Maze generator";
  r.width = 512;
  r.height = 512;
  r.window = NULL;
  r.renderer = NULL;

  if (!initialize_SDL(&r)) {
    SDL_Log("SDL failed to initialize: %s", SDL_GetError());
    destroy_SDL(&r);
  }

  int n = 256;
  assert(n <= 9000);
  for (int i = 0; i < n; i++) {
    store[i].top = true;
    store[i].bottom = true;
    store[i].left = true;
    store[i].right = true;
    store[i].label = i;
    counter[i] = 1;
  }

  kruskal_maze(&r, n);
  SDL_Delay(2000);
  destroy_SDL(&r);
  return 0;
}

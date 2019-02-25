#include "SDL.h"

struct renderer_config {
  int width;
  int height;
  char *title;
  SDL_Window *window;
  SDL_Renderer *renderer;
  SDL_Event event;
};

int random_int(int n, int m);
bool initialize_SDL(struct renderer_config *r);
void destroy_SDL(struct renderer_config *r);

int random_int(int n, int m) { return rand() % (m - n + 1) + n; }

bool initialize_SDL(struct renderer_config *r) {
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
    return false;
  }

  r->window = SDL_CreateWindow(r->title, SDL_WINDOWPOS_UNDEFINED,
                               SDL_WINDOWPOS_UNDEFINED, r->width, r->height,
                               SDL_WINDOW_SHOWN);
  if (r->window == NULL) {
    SDL_Log("Unable to create window: %s", SDL_GetError());
    return false;
  }

  r->renderer = SDL_CreateRenderer(r->window, -1, SDL_RENDERER_ACCELERATED);
  if (r->renderer == NULL) {
    SDL_Log("Unable to create renderer: %s", SDL_GetError());
  }

  SDL_SetRenderDrawColor(r->renderer, 0xFF, 0xFF, 0XFF, 0xFF);

  return true;
}

void destroy_SDL(struct renderer_config *r) {
  SDL_DestroyRenderer(r->renderer);
  SDL_DestroyWindow(r->window);
  SDL_Quit();
}

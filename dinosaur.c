// Dino something.
#include "utils.h"
#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

const int WIN_WIDTH = 600;
const int WIN_HEIGHT = 480;
const int HORIZON_FRONT = 0;
const int HORIZON_BACK = 1;
const int CACTUS_LARGE = 2;
const int DINOSAUR = 3;
const int GAME_OBJECTS_CAP = 4;
const int MAX_OBSTACLE_LENGTH = 3;
double TIME_PER_UPDATE = 16.6666f;
int frames = 0;
bool is_jumping = false;
double dino_velocity = -40;
double gravity = 9.8;
double world_velocity = 20.0;

typedef struct texture_t {
  int width;
  int height;
  SDL_Texture *texture;
  SDL_Surface *surface;
} texture_t;

typedef struct gamestate_t {
  int x;
  int y;
  int index;
  SDL_Rect sprite;
} gamestate_t;

bool texture_load(texture_t *t, struct renderer_config *r, const char *path) {
  t->texture = NULL;
  t->surface = IMG_Load(path);
  if (t->surface == NULL) {
    printf("Unable to load image %s. Error: %s\n", path, IMG_GetError());
    return false;
  } else {
    SDL_SetColorKey(t->surface, SDL_TRUE,
                    SDL_MapRGB(t->surface->format, 0, 0, 0));
    t->texture = SDL_CreateTextureFromSurface(r->renderer, t->surface);
    if (t->texture == NULL) {
      printf("Unable to create texture. Error: %s\n", SDL_GetError());
      return false;
    } else {
      t->width = t->surface->w;
      t->height = t->surface->h;
    }
    SDL_FreeSurface(t->surface);
  }
  return true;
}

void texture_free(texture_t *t) {
  if (t->texture != NULL) {
    SDL_DestroyTexture(t->texture);
    t->texture = NULL;
    t->width = 0;
    t->height = 0;
  }
}

void texture_render(texture_t *t, struct renderer_config *r, int x, int y,
                    SDL_Rect *s_rect) {
  SDL_Rect d_rect = {x, y, t->width, t->height};

  if (s_rect != NULL) {
    d_rect.w = s_rect->w;
    d_rect.h = s_rect->h;
  }

  SDL_RenderCopy(r->renderer, t->texture, s_rect, &d_rect);
}

void initialize(texture_t *t, struct renderer_config *r) {
  if (!initialize_SDL(r)) {
    SDL_Log("SDL failed to initialize: %s", SDL_GetError());
    destroy_SDL(r);
  }

  if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
    printf("Error: %s\n", SDL_GetError());
    destroy_SDL(r);
  }

  if (!texture_load(t, r, "./res/dino.png")) {
    SDL_Log("Texture failed to load: %s", SDL_GetError());
    destroy_SDL(r);
  }
}

void update(gamestate_t *game_objects) {
  double dt = 1.0 / 60.0 * 10.0;
  int world_delta = (int)ceil(dt * world_velocity);
  for (int i = 0; i < GAME_OBJECTS_CAP; i++) {
    switch (game_objects[i].index) {
    case HORIZON_FRONT:
      if (game_objects[HORIZON_FRONT].x > -565) {
        game_objects[HORIZON_FRONT].x -= world_delta;
      } else {
        game_objects[HORIZON_FRONT].x = 565;
      }
      break;
    case HORIZON_BACK:
      if (game_objects[HORIZON_BACK].x > -565) {
        game_objects[HORIZON_BACK].x -= world_delta;
      } else {
        game_objects[HORIZON_BACK].x = 565;
      }
      break;
    case CACTUS_LARGE:
      if (game_objects[CACTUS_LARGE].x > 0) {
        game_objects[CACTUS_LARGE].x -= world_delta;
      } else {
        game_objects[CACTUS_LARGE].x = 625;
      }
      break;
    case DINOSAUR:
      if (is_jumping) {
        int value;
        if (dt * dino_velocity < 0) {
          value = (int)floor(dt * dino_velocity);
        } else {
          value = (int)ceil(dt * dino_velocity);
        }
        game_objects[DINOSAUR].y += value;
        dino_velocity += dt * gravity;
        if (game_objects[DINOSAUR].y >= 405) {
          is_jumping = false;
          dino_velocity = -40;
        }
      }
      break;
    }
  }
  frames += 1;
  // animate dinosaur
  if ((frames % 10) == 0) {
    if (game_objects[DINOSAUR].sprite.x < (848 + 3 * 44)) {
      game_objects[DINOSAUR].sprite.x += 44;
    } else {
      game_objects[DINOSAUR].sprite.x = 848;
    }
  }
}

void render(texture_t *t, struct renderer_config *r,
            gamestate_t *game_objects) {
  SDL_SetRenderDrawColor(r->renderer, 0, 0, 0, 0xFF);
  SDL_RenderClear(r->renderer);

  texture_render(t, r, game_objects[HORIZON_FRONT].x,
                 game_objects[HORIZON_FRONT].y,
                 &(game_objects[HORIZON_FRONT].sprite));
  texture_render(t, r, game_objects[HORIZON_BACK].x,
                 game_objects[HORIZON_BACK].y,
                 &(game_objects[HORIZON_BACK].sprite));
  texture_render(t, r, game_objects[CACTUS_LARGE].x,
                 game_objects[CACTUS_LARGE].y,
                 &(game_objects[CACTUS_LARGE].sprite));

  texture_render(t, r, game_objects[DINOSAUR].x, game_objects[DINOSAUR].y,
                 &(game_objects[DINOSAUR].sprite));

  SDL_RenderPresent(r->renderer);
}

int main(int argc, char *args[]) {

  bool is_rendering = true;

  struct renderer_config r = {.title = "Dinosaur game?",
                              .width = WIN_WIDTH,
                              .height = WIN_HEIGHT,
                              .window = NULL,
                              .renderer = NULL};

  texture_t t = {.width = 0, .height = 0, .texture = NULL, .surface = NULL};

  initialize(&t, &r);

  gamestate_t game_objects[] = {
      {.x = 0,
       .y = 448,
       .index = 0,
       .sprite = {.x = 2, .y = 54, .w = 600, .h = 12}},
      {.x = 565,
       .y = 448,
       .index = 1,
       .sprite = {.x = 600, .y = 54, .w = 600, .h = 12}},
      {.x = 700,
       .y = 410,
       .index = 2,
       .sprite = {.x = 332, .y = 2, .w = 25, .h = 50}},
      {.x = 20,
       .y = 405,
       .index = 3,
       .sprite = {.x = 848, .y = 2, .w = 44, .h = 47}}};

  uint64_t current_time = SDL_GetPerformanceCounter();
  uint64_t last_time = 0;
  double dt = 0.0;
  double lag = 0.0;

  while (is_rendering) {

    last_time = current_time;
    current_time = SDL_GetPerformanceCounter();
    dt = (double)((current_time - last_time) * 1000 /
                  (double)SDL_GetPerformanceFrequency());
    lag += dt;

    while (SDL_PollEvent(&(r.event)) != 0) {
      switch (r.event.type) {
      case SDL_QUIT:
        is_rendering = false;
        texture_free(&t);
        destroy_SDL(&r);
      case SDL_KEYDOWN:
        switch (r.event.key.keysym.sym) {
        case SDLK_UP:
        case SDLK_SPACE:
          if (!is_jumping) {
            is_jumping = true;
          }
          SDL_Log("Key up pressed");
        }
      }
    }

    while (lag >= TIME_PER_UPDATE) {
      update(game_objects);
      lag -= TIME_PER_UPDATE;
      if (lag < 0.0) {
        lag = 0.0;
      }
    }
    render(&t, &r, game_objects);
  }

  return 0;
}

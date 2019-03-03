#include <stdbool.h>
#include <stdlib.h>

#include "utils.h"

#define TOP 0
#define BOTTOM 1
#define LEFT 2
#define RIGHT 3
#define CAPACITY 36
#define ROW_LENGTH 6

bool set[CAPACITY];

struct queue {
  int capacity;
  int head;
  int tail;
  int buffer[CAPACITY];
};

void initialize_queue(struct queue *q, int capacity) {
  q->capacity = capacity;
  q->head = 0;
  q->tail = 0;
  for (int i = 0; i < q->capacity; i++) {
    q->buffer[i] = 0;
  }
}

void enqueue(struct queue *q, int x) {
  int tail = q->tail;
  q->buffer[tail] = x;
  if (q->tail == (q->capacity - 1)) {
    q->tail = 0;
  } else {
    q->tail += 1;
  }
}

int dequeue(struct queue *q) {
  int head = q->head;
  int x = q->buffer[head];
  if (q->head == (q->capacity - 1)) {
    q->head = 0;
  } else {
    q->head += 1;
  }
  return x;
}

bool is_empty(struct queue *q) { return q->tail == q->head; }

int neighbour_aux(int current, int direction, int row_length, int capacity) {
  switch (direction) {
  case TOP:
    return (current - row_length) >= 0 ? current - row_length : current;
  case BOTTOM:
    return (current + row_length) < capacity ? current + row_length : current;
  case LEFT:
    return (current - 1) / row_length == current / row_length ? current - 1
                                                              : current;
  case RIGHT:
    return (current + 1) / row_length == current / row_length ? current + 1
                                                              : current;
  default:
    return 0;
  }
}

int neighbour(int current, int direction, int row_length, int capacity) {
  int next = neighbour_aux(current, direction, row_length, capacity);
  return next >= 0 ? next : current;
}

bool is_element(int x, bool set[]) { return x >= 0 && x < CAPACITY && set[x]; }

void render_bfs(struct renderer_config *r, int n) {
  SDL_SetRenderDrawColor(r->renderer, 0xFF, 0xFF, 0xFF, 0xFF);
  SDL_RenderClear(r->renderer);

  int rect_side = r->width / n;
  int color = rand();
  for (int i = 0; i < CAPACITY; i++) {
    if (set[i]) {
      int row = i / n;
      int column = i % n;
      // x, y, w, h
      SDL_Rect fill_rect = {column * rect_side, row * rect_side, rect_side,
                            rect_side};
      SDL_SetRenderDrawColor(r->renderer, color, color, rand(), rand());
      SDL_RenderFillRect(r->renderer, &fill_rect);
    }
  }
  while (SDL_PollEvent(&(r->event)) != 0) {
    if (r->event.type == SDL_QUIT) {
      destroy_SDL(&r);
    }
  }
  SDL_RenderPresent(r->renderer);
  SDL_Delay(50);
}

void breadth_first_search(struct renderer_confgi *r, int start,
                          int queue_capacity) {
  struct queue q;
  initialize_queue(&q, queue_capacity);
  enqueue(&q, start);
  set[start] = true;

  while (!is_empty(&q)) {
    int current = dequeue(&q);

    for (int i = 0; i < 4; i++) {
      int next = neighbour(current, i, ROW_LENGTH, CAPACITY);
      if (!is_element(next, set)) {
        enqueue(&q, next);
        set[next] = true;
      }
    }
    render_bfs(r, ROW_LENGTH);
  }
}

int main(int argc, char *args[]) {
  struct renderer_config r;
  r.title = "BFS pattern generator";
  r.width = 480;
  r.height = 480;
  r.window = NULL;
  r.renderer = NULL;

  if (!initialize_SDL(&r)) {
    SDL_Log("SDL failed to initialize: %s", SDL_GetError());
    destroy_SDL(&r);
  }

  for (int i = 0; i < CAPACITY; i++) {
    set[i] = false;
  }

  breadth_first_search(&r, 6, CAPACITY);
  SDL_Delay(2000);
  destroy_SDL(&r);

  return 0;
}

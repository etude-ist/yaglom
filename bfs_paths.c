#include <stdbool.h>

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

void breadth_first_search(int start, int queue_capacity) {
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
  }
}

int main() {

  for (int i = 0; i < CAPACITY; i++) {
    set[i] = false;
  }

  breadth_first_search(10, CAPACITY);

  return 0;
}

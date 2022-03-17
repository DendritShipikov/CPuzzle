#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include "puzzle.h"

int rects_ctor(Rects_t* rects, int rsize) {
  rects->data = (Rect_t*)malloc(rsize * sizeof(Rect_t));
  if (rects->data == NULL) return 0;
  rects->size = 0;
  rects->rsize = rsize;
  return 1;
}

void rects_move_ctor(Rects_t* rects, Rects_t* other) {
  *rects = *other;
  other->data = NULL;
  other->size = other->rsize = 0;
}

void rects_dtor(Rects_t* rects) { free(rects->data); }

int rects_emplace_back(Rects_t* rects, int a, int b) {
  int size = rects->size;
  int rsize = rects->rsize;
  if (size == rsize) {
    rsize = 2 * rects->rsize + 1;
    Rect_t* data = (Rect_t*)realloc(rects->data, rsize * sizeof(Rect_t));
    if (data == NULL) return 0;
    rects->data = data;
    rects->rsize = rsize;
  }
  Rect_t* rect = &rects->data[size];
  rect->a = a;
  rect->b = b;
  rects->size++;
  return 1;
}

void rects_swap(Rects_t* rects, int i, int j) {
  Rect_t* data = rects->data;
  Rect_t tmp = data[i];
  data[i] = data[j];
  data[j] = tmp;
}

int points_ctor(Points_t* points, int rsize) {
  points->data = (Point_t*)malloc(rsize * sizeof(Point_t));
  if (points->data == NULL) return 0;
  points->size = 0;
  points->rsize = rsize;
  return 1;
}

void points_dtor(Points_t* points) { free(points->data); }

Point_t* points_push(Points_t* points) { return &points->data[points->size++]; }

void points_pop(Points_t* points) { points->size--; }

int evaler_ctor(Evaler_t* evaler, Rects_t* rects, int a, int b) {
  Points_t* points = &evaler->points;
  if (!points_ctor(points, rects->size + 2)) return 0;
  rects_move_ctor(&evaler->rects, rects);
  Point_t* begin = points_push(points);
  Point_t* end = points_push(points);
  begin->x = 0; begin->y = b;
  end->x = a; end->y = 0;
  begin->next = end; begin->prev = NULL;
  end->next = NULL; end->prev = begin;
  evaler->begin = begin;
  evaler->end = end;
  evaler->ilast = 0;
  return 1;
}

void evaler_dtor(Evaler_t* evaler) {
  points_dtor(&evaler->points);
  rects_dtor(&evaler->rects);
}

int evaler_insert(Evaler_t* evaler, Point_t* prev, Point_t* next) {
  Rects_t* rects = &evaler->rects;
  int ilast = evaler->ilast;
  if (ilast == rects->size) return 0;
  Rect_t* rect = &rects->data[ilast];
  int dx = next->x - prev->x;
  int dy = prev->y - next->y;
  int a = rect->a;
  int b = rect->b;
  if (dx < a || dy < b) return 0;
  Points_t* points = &evaler->points;
  Point_t* point = points_push(points);
  point->x = prev->x + a;
  point->y = next->y + b;
  if (dx == a) {
    Point_t* next_next = next->next;
    point->next = next_next;
    if (next_next != NULL) {
      next_next->prev = point;
    } else {
      evaler->end = point;
    }
  } else {
    point->next = next;
    next->prev = point;
  }
  if (dy == b) {
    Point_t* prev_prev = prev->prev;
    point->prev = prev_prev;
    if (prev_prev != NULL) {
      prev_prev->next = point;
    } else {
      evaler->begin = point;
    }
  } else {
    point->prev = prev;
    prev->next = point;
  }
  evaler->ilast++;
  return 1;
}

void evaler_rollback(Evaler_t* evaler, Point_t* prev, Point_t* next) {
  Points_t* points = &evaler->points;
  points_pop(points);
  prev->next = next;
  next->prev = prev;
  Point_t* next_next = next->next;
  Point_t* prev_prev = prev->prev;
  if (next_next != NULL) {
    next_next->prev = next;
  } else {
    evaler->end = next;
  }
  if (prev_prev != NULL) {
    prev_prev->next = prev;
  } else {
    evaler->begin = prev;
  }
  evaler->ilast--;
}

int evaler_eval_step(Evaler_t* evaler) {
  Point_t* prev = evaler->begin;
  Point_t* end = evaler->end;
  while (prev != end) {
    Point_t* next = prev->next;
    if (evaler_insert(evaler, prev, next)) {
      if (evaler->begin == evaler->end) return 1;
      if (evaler_eval(evaler)) return 1;
      evaler_rollback(evaler, prev, next);
    }
    prev = next;
  }
  return 0;
}

int evaler_eval(Evaler_t* evaler) {
  Rects_t* rects = &evaler->rects;
  int ilast = evaler->ilast;
  int size = rects->size;
  for (int i = ilast; i < size; ++i) {
    if (i != ilast) rects_swap(rects, i, ilast);
    Rect_t* rect = &rects->data[ilast];
    if (evaler_eval_step(evaler)) return 1;
    if (rect->a != rect->b) {
      int tmp = rect->a;
      rect->a = rect->b;
      rect->b = tmp;
      if (evaler_eval_step(evaler)) return 1;
    }
  }
  return 0;
}

void evaler_print(Evaler_t* evaler) {
  Rect_t* rects_data = evaler->rects.data;
  Point_t* points_data = evaler->points.data;
  for (int i = 0; i < evaler->ilast; ++i) {
    Rect_t* rect = &rects_data[i];
    Point_t* point = &points_data[i + 2];
    printf("rectangle (%d, %d) -> (%d, %d)\n", rect->a, rect->b, point->x - rect->a, point->y - rect->b);
  }
}
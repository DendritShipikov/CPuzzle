#ifndef PUZZLE_H
#define PUZZLE_H

typedef struct Rect {
  int a;
  int b;
} Rect_t;

typedef struct Rects {
  Rect_t* data;
  int rsize;
  int size;
} Rects_t;

int rects_ctor(Rects_t* rects, int rsize);
void rects_move_ctor(Rects_t* rects, Rects_t* other);
void rects_dtor(Rects_t* rects);
int rects_emplace_back(Rects_t* rects, int a, int b);
void rects_swap(Rects_t* rects, int i, int j);

typedef struct Point {
  int x;
  int y;
  struct Point* prev;
  struct Point* next;
} Point_t;

typedef struct Points {
  Point_t* data;
  int rsize;
  int size;
} Points_t;

int points_ctor(Points_t* points, int rsize);
void points_dtor(Points_t* points);
Point_t* points_push(Points_t* points);
void points_pop(Points_t* points);

typedef struct Evaler {
  Rects_t rects;
  Points_t points;
  Point_t* begin;
  Point_t* end;
  int ilast;
} Evaler_t;

int evaler_ctor(Evaler_t* evaler, Rects_t* rects, int a, int b);
void evaler_dtor(Evaler_t* evaler);
int evaler_insert(Evaler_t* evaler, Point_t* prev, Point_t* next);
void evaler_rollback(Evaler_t* evaler, Point_t* prev, Point_t* next);
int evaler_eval_step(Evaler_t* evaler);
int evaler_eval(Evaler_t* evaler);
void evaler_print(Evaler_t* evaler);

#endif
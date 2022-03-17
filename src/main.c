#include <stdlib.h>
#include <stdio.h>
#include "puzzle.h"

int parse_int(const char* str, int* out) {
  long long result = 0;
  for (const char* c = str; *c != '\0'; ++c) {
    if (*c < '0' || *c > '9') return 0;
    result = 10 * result + (*c - '0');
    if (result > INT_MAX) return 0;
  }
  *out = (int)result;
  return 1;
}

int main(int argc, char** argv) {
#define HELP "Usage: puzzle A B a1 b1 ...\n"
  if (argc < 5 || argc % 2 == 0) {
    printf(HELP);
    return 0;
  }
  Rects_t orects;
  Rects_t* rects = &orects;
  if (!rects_ctor(rects, 8)) {
    printf("Memory is out");
    return 1;
  }
#define TRY_PARSE(i, j) if (!parse_int(argv[i], &a) || !parse_int(argv[j], &b)) { printf(HELP); rects_dtor(rects); return 0; }
  for (int i = 3; i < argc; i += 2) {
    int a, b;
    TRY_PARSE(i, i + 1)
    rects_emplace_back(rects, a, b);
  }
  int a, b;
  TRY_PARSE(1, 2)
  Evaler_t oevaler;
  Evaler_t* evaler = &oevaler;
  if (!evaler_ctor(evaler, rects, a, b)) {
    printf("Memory is out");
    rects_dtor(rects);
    evaler_dtor(evaler);
    return 1;
  }
  if (evaler_eval(evaler)) {
    evaler_print(evaler);
  } else {
    printf("impossible\n");
  }
  evaler_dtor(evaler);
  return 0;
}
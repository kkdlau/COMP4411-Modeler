#pragma once

#include <cstdio>

#ifdef ENABLE_DEBUGGER
#define debugger(...)                                                          \
  printf(__VA_ARGS__);                                                         \
  printf("\n");                                                                \
  fflush(stdout);
#define debugger_vec3(v) debugger("%s: %f %f %f", #v, v[0], v[1], v[2])
#else
#define debugger(...)
#endif
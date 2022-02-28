#pragma once

#include "debug_utils.h"

#if defined(__APPLE__)
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#else
#include <windows.h>

#include <GL/gl.h>
#include <GL/glu.h>
#endif
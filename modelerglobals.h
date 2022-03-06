#ifndef _MODELER_GLOBALS_H
#define _MODELER_GLOBALS_H

#ifndef M_PI
#define M_PI 3.141592653589793238462643383279502
#endif
#include <functional>

using namespace std;

// This is a list of the controls for the SampleModel
// We'll use these constants to access the values
// of the controls from the user interface.
enum SampleModelControls {
  XPOS,
  YPOS,
  ZPOS,
  HEIGHT,
  ROTATE,
  LIGHT_XPOS,
  LIGHT_YPOS,
  LIGHT_ZPOS,
  LIGHT_INTENSITY,
  NUMCONTROLS
};

static inline void
for_each_control(function<void(SampleModelControls)> handler) {
  for (int v = XPOS; v != NUMCONTROLS; v++)
    handler((SampleModelControls)v);
}

// Colors
#define COLOR_RED 1.0f, 0.0f, 0.0f
#define COLOR_GREEN 0.0f, 1.0f, 0.0f
#define COLOR_BLUE 0.0f, 0.0f, 1.0f

// We'll be getting the instance of the application a lot;
// might as well have it as a macro.
#define VAL(x) (float)(ModelerApplication::Instance()->GetControlValue(x))

#endif
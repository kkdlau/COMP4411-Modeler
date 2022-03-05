// The sample model.  You should build a file
// very similar to this for when you make your model.
#include "modelerapp.h"
#include "modelerdraw.h"
#include "modelerview.h"
#include "texturedraw.hpp"

#include <FL/gl.h>
#include <functional>

#include "CatModel.hpp"
#include "modelerglobals.h"

// To make a CatModel, we inherit off of ModelerView
class CatModel : public ModelerView {
public:
  CatModel(int x, int y, int w, int h, char *label)
      : ModelerView(x, y, w, h, label) {}

  virtual void draw();
};

// We need to make a creator function, mostly because of
// nasty API stuff that we'd rather stay away from.
ModelerView *createSampleModel(int x, int y, int w, int h, char *label) {
  return new CatModel(x, y, w, h, label);
}

// We are going to override (is that the right word?) the draw()
// method of ModelerView to draw out CatModel
void CatModel::draw() {
  // This call takes care of a lot of the nasty projection
  // matrix stuff.  Unless you want to fudge directly with the
  // projection matrix, don't bother with this ...
  ModelerView::draw();

  // 1B: dramatic lighting
  GLfloat lightIntensity[]{VAL(LIGHT_INTENSITY), VAL(LIGHT_INTENSITY), VAL(LIGHT_INTENSITY), 1 };
  GLfloat lightPosition2[]{ VAL(LIGHT_XPOS), VAL(LIGHT_YPOS), VAL(LIGHT_ZPOS), 0 };
  GLfloat lightDiffuse2[]{ 1,1,1,1 };
  glEnable(GL_LIGHT2);
  glLightfv(GL_LIGHT2, GL_POSITION, lightPosition2);
  glLightfv(GL_LIGHT2, GL_DIFFUSE, lightDiffuse2);
  //glLightfv(GL_LIGHT1, GL_DIFFUSE, lightIntensity);
  glLightfv(GL_LIGHT2, GL_DIFFUSE, lightIntensity);

  // draw the floor
  setAmbientColor(.1f, .1f, .1f);
  setDiffuseColor(COLOR_RED);
  glPushMatrix();
  {
    glTranslated(-5, 0, -5);
    drawBox(10, 0.01f, 10);
  }
  glPopMatrix();

  // draw the sample model
  setAmbientColor(.1f, .1f, .1f);
  setDiffuseColor(COLOR_GREEN);
  glPushMatrix();
  glTranslated(VAL(XPOS), VAL(YPOS), VAL(ZPOS));

  Cat danny_favourite_cat;
  danny_favourite_cat.render();

  glPopMatrix();
}

int main() {
  // Initialize the controls
  // Constructor is ModelerControl(name, minimumvalue, maximumvalue,
  // stepsize, defaultvalue)
  ModelerControl controls[NUMCONTROLS];
  controls[XPOS] = ModelerControl("X Position", -5, 5, 0.1f, 0); 
  controls[YPOS] = ModelerControl("Y Position", 0, 5, 0.1f, 0);
  controls[ZPOS] = ModelerControl("Z Position", -5, 5, 0.1f, 0);
  controls[HEIGHT] = ModelerControl("Height", 1, 2.5, 0.1f, 1);
  controls[ROTATE] = ModelerControl("Rotate", -135, 135, 1, 0);

  controls[ROTATE_HEAD] = ModelerControl("Rotate Head", 45, 135, 90, 0); // check what the original value should be 90 or 0
  controls[LIGHT_XPOS] = ModelerControl("Light X Position", -10, 10, 0.1f, 0);
  controls[LIGHT_YPOS] = ModelerControl("Light Y Position", 0, 10, 0.1f, 0);
  controls[LIGHT_ZPOS] = ModelerControl("Light Z Position", -10, 10, 0.1f, 0);
  controls[LIGHT_INTENSITY] = ModelerControl("Light Intensity", 0, 1, 0.1f, 0);

  // initialize texture maps 
  initTextureMap();

  ModelerApplication::Instance()->Init(&createSampleModel, controls,
                                       NUMCONTROLS);
  return ModelerApplication::Instance()->Run();
}

// The sample model.  You should build a file
// very similar to this for when you make your model.
#include "modelerapp.h"
#include "modelerdraw.h"
#include "modelerview.h"
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

void draw_leg(float rad = 0.25, float rot = 0.0f) {
  glPushMatrix();
  {
    glRotated(rot, 0.0, 0.0, 0);
    glRotated(-90, 1.0, 0.0, 0.0);
    drawCylinder(1, rad, rad);
  }
  glPopMatrix();
}

void draw_head(float head_width, float head_height) {
  glPushMatrix();
  {
    glTranslated(-head_width / 2, -head_height / 2, -0.8 / 2);
    drawBox(head_width, head_height, 0.8);
#define p1 0, 0 + head_height, 0
#define p2 0.5, 0 + head_height, 0
#define p3 0.25, 0.25 + head_height, 0
    drawTriangle(p1, p2, p3);

#define p1 1.3, 0 + head_height, 0
#define p2 1.3 - 0.5, 0 + head_height, 0
#define p3 1.3 - 0.25, 0.25 + head_height, 0
    drawTriangle(p1, p2, p3);
  }
  glPopMatrix();
}

void draw_tailpart(float r, float scale, float length = 0.8,
                   function<void()> child = nullptr, float radius = 0.2) {
  glPushMatrix();
  {
    glRotated(r, 1.0, 0.0, 0);
    glScaled(scale, scale, scale);
    drawCylinder(length, radius, radius);
    if (child)
      child();
  }
  glPopMatrix();
}

void draw_tail(float r1, float r2, float r3, float component_length) {
  float len = component_length;
  draw_tailpart(r1, 1, len, [&] {
    glTranslated(0, 0, len);
    draw_tailpart(r2, 1, len, [&] {
      glTranslated(0, 0, len);
      draw_tailpart(r3, 1, len);
    });
  });
}

void draw_cat() {
  glPushMatrix();
  {
    glTranslated(-0.5, 2, -2);

    drawBox(2, 1.3, 4);
    glPushMatrix();
    {
      // front_left
      glTranslated(0.2, -1, 0.2);
      draw_leg();
    }
    glPopMatrix();

    glPushMatrix();
    {
      // front_right
      glTranslated(1.8, -1, 0.2);
      draw_leg();
    }
    glPopMatrix();

    glPushMatrix();
    {
      // back_left
      glTranslated(1.8, -1, 3.8);
      draw_leg();
    }
    glPopMatrix();

    glPushMatrix();
    {
      // back_right
      glTranslated(0.2, -1, 3.8);
      draw_leg();
    }
    glPopMatrix();

    glPushMatrix();
    {
      // head
      glTranslated(1, 1.2, -0.1);
      draw_head(1.3, 1.3);
    }
    glPopMatrix();

    glPushMatrix();
    {
      glTranslated(1, 0.5, 4);
      draw_tail(10, 10, 10, 0.5);
    }
    glPopMatrix();
  }
  glPopMatrix();
}

void CatModel::draw() {
  ModelerView::draw();

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

  draw_cat();

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

  ModelerApplication::Instance()->Init(&createSampleModel, controls,
                                       NUMCONTROLS);
  return ModelerApplication::Instance()->Run();
}

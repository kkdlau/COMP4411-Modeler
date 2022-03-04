// The sample model.  You should build a file
// very similar to this for when you make your model.
#include "modelerapp.h"
#include "modelerdraw.h"
#include "modelerview.h"
#include <FL/gl.h>
#include <functional>

#include "modelerglobals.h"

class Base3DObject {
public:
  function<void()> trans; // transformation

  Base3DObject() {}
  virtual void reset_default() {
    trans = [] {};
  }
  virtual void draw() = 0;
};

struct Leg : public Base3DObject {
  float radius;
  float rotate;

  void reset_default() {
    trans = [] {};
    radius = 0.25;
    rotate = 0;
  }

  Leg() { reset_default(); }

  void draw() {
    glPushMatrix();
    {
      trans();
      glRotated(rotate, 0.0, 0.0, 0);
      glRotated(-90, 1.0, 0.0, 0.0);
      drawCylinder(1, radius, radius);
    }
    glPopMatrix();
  }
};

struct Head : public Base3DObject {
  float rotate;

  void reset_default() {
    Base3DObject::reset_default();
    rotate = 0;
  }

  Head() { reset_default(); }

  void draw() {
    glPushMatrix();
    {
      trans();
      drawBox(1.3, 1.3, 0.8);
    }
    glPopMatrix();
  }
};

struct TailPart : public Base3DObject {
  float radius;
  float rotate;
  float length;

  void reset_default() {
    trans = [] {};
    radius = 0.2;
    rotate = 0;
    length = 0.8;
  }

  TailPart() { reset_default(); }

  void draw() {
    glPushMatrix();
    {
      trans();
      glTranslated(0, 0, -length / 2);
      glRotated(rotate, 1.0, 0.0, 0);
      drawCylinder(length, radius, radius);
    }
    glPopMatrix();
  }
};

struct Tail : public Base3DObject {
  TailPart components[3];
  float radius = 0.2;
  float rotate = 10;
  float length = 0.8;

  Tail() { reset_default(); }

  void draw() {
    glPushMatrix();
    {
      trans();
      glPushMatrix();
      for (int i = 0; i < 3; i++) {
        glRotated(rotate, 1.0, 0.0, 0);
        drawCylinder(length, radius, radius);
        glTranslated(0, 0, length);
      }

      glPopMatrix();
    }
    glPopMatrix();
  }
};

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

  glPushMatrix();
  glTranslated(-1.5, 1, -2);
  glScaled(2, 1, 4);
  drawBox(1, 1.3, 1);
  glScaled(1 / 2.0, 1, 1 / 4.0);
  Leg cat_legs[4];
  cat_legs[0].trans = [&] { glTranslated(0.2, -1, 0.2); };
  cat_legs[0].draw();

  cat_legs[1].trans = [&] { glTranslated(1.8, -1, 0.2); };
  cat_legs[1].draw();

  cat_legs[2].trans = [&] { glTranslated(1.8, -1, 3.8); };
  cat_legs[2].draw();

  cat_legs[3].trans = [&] { glTranslated(0.2, -1, 3.8); };
  cat_legs[3].draw();

  Head h;
  h.trans = [&] { glTranslated(0.35, 1.2, -0.1); };

  h.draw();

  Tail t;
  t.trans = [&] { glTranslated(1, 0.5, 4); };
  t.draw();

  glPopMatrix();

  // // draw cannon
  // glPushMatrix();
  // glRotated(VAL(ROTATE), 0.0, 1.0, 0.0);
  // glRotated(-90, 1.0, 0.0, 0.0);
  // drawCylinder(VAL(HEIGHT), 0.1, 0.1);

  // glTranslated(0.0, 0.0, VAL(HEIGHT));
  // drawCylinder(1, 1.0, 0.9);

  // glTranslated(0.0, 0.0, 0.5);
  // glRotated(90, 1.0, 0.0, 0.0);
  // drawCylinder(4, 0.1, 0.2);
  // glPopMatrix();

  glPopMatrix();
}

int main() {
  // Initialize the controls
  // Constructor is ModelerControl(name, minimumvalue, maximumvalue,
  // stepsize, defaultvalue)
  ModelerControl controls[NUMCONTROLS];
  controls[XPOS] = ModelerControl("J Position", -5, 5, 0.1f, 0);
  controls[YPOS] = ModelerControl("Y Position", 0, 5, 0.1f, 0);
  controls[ZPOS] = ModelerControl("Z Position", -5, 5, 0.1f, 0);
  controls[HEIGHT] = ModelerControl("Height", 1, 2.5, 0.1f, 1);
  controls[ROTATE] = ModelerControl("Rotate", -135, 135, 1, 0);

  ModelerApplication::Instance()->Init(&createSampleModel, controls,
                                       NUMCONTROLS);
  return ModelerApplication::Instance()->Run();
}

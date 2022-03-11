// The sample model.  You should build a file
// very similar to this for when you make your model.
#include "modelerapp.h"
#include "modelerdraw.h"
#include "modelerview.h"
#include "texturedraw.hpp"

#include <FL/gl.h>
#include <functional>

#include "CatModel.hpp"
#include "TreeLSystem.hpp"
#include "modelerglobals.h"
#include <math.h>       /* log */

static int32_t ticks = 0;
static const int32_t ANIMATION_MAX_TICKS = 30 * 2; // 5 seconds

static bool enable_animation() { return VAL(ANIMATION); }

static int32_t next_ticks() {
#if 0
    return (ticks + 1) % ANIMATION_MAX_TICKS;
#else
  static bool forward = true;
  ticks += forward ? 1 : -1;
  if (ticks == ANIMATION_MAX_TICKS) {
    ticks = ANIMATION_MAX_TICKS - 2;
    forward = false;
  } else if (ticks == -1) {
    ticks = 1;
    forward = true;
  }
#endif
  // debugger("ticks: %d", ticks);
  return ticks;
}

static float animation_progress() { return (float)ticks / ANIMATION_MAX_TICKS; }

void run_if_animate(function<void()> funcs) {
  if (enable_animation()) {
    // perform any animation pre porcessing
    funcs();
  }
}

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

int prev_individual = 1;

void setIndividual() {
  if (VAL(INDIVIDUAL) == 1 && prev_individual != 1) { // normal orange cat
    ModelerApplication::Instance()->SetControlValue(TAIL_ANGLE, 0);
    ModelerApplication::Instance()->SetControlValue(NUM_TAIL_COMPONENT, 3);
    ModelerApplication::Instance()->SetControlValue(HEAD_WIDTH, 1.3);
    ModelerApplication::Instance()->SetControlValue(HEAD_HEIGHT, 1.3);
    ModelerApplication::Instance()->SetControlValue(LEG_LENGTH, 1.3);
    ModelerApplication::Instance()->SetControlValue(EAR_LENGTH, 0.5);
    ModelerApplication::Instance()->SetControlValue(BODY_DEPTH, 1.3);
    ModelerApplication::Instance()->SetControlValue(BODY_LENGTH, 4);
    ModelerApplication::Instance()->SetControlValue(BODY_WIDTH, 2);
    prev_individual = 1;
  } else if (VAL(INDIVIDUAL) == 2 && prev_individual != 2) { // fat grey cat
    ModelerApplication::Instance()->SetControlValue(TAIL_ANGLE, 30);
    ModelerApplication::Instance()->SetControlValue(NUM_TAIL_COMPONENT, 7);
    ModelerApplication::Instance()->SetControlValue(HEAD_WIDTH, 2);
    ModelerApplication::Instance()->SetControlValue(HEAD_HEIGHT, 1.3);
    ModelerApplication::Instance()->SetControlValue(LEG_LENGTH, 0.5);
    ModelerApplication::Instance()->SetControlValue(EAR_LENGTH, 1);
    ModelerApplication::Instance()->SetControlValue(BODY_DEPTH, 1.3);
    ModelerApplication::Instance()->SetControlValue(BODY_LENGTH, 4);
    ModelerApplication::Instance()->SetControlValue(BODY_WIDTH, 3);
    prev_individual = 2;
  } else if (VAL(INDIVIDUAL) == 3 && prev_individual != 3) { // lean spotty cat
    ModelerApplication::Instance()->SetControlValue(TAIL_ANGLE, 10);
    ModelerApplication::Instance()->SetControlValue(NUM_TAIL_COMPONENT, 7);
    ModelerApplication::Instance()->SetControlValue(HEAD_WIDTH, 1.8);
    ModelerApplication::Instance()->SetControlValue(HEAD_HEIGHT, 2);
    ModelerApplication::Instance()->SetControlValue(LEG_LENGTH, 2.5);
    ModelerApplication::Instance()->SetControlValue(EAR_LENGTH, 2);
    ModelerApplication::Instance()->SetControlValue(BODY_DEPTH, 1.5);
    ModelerApplication::Instance()->SetControlValue(BODY_LENGTH, 6);
    ModelerApplication::Instance()->SetControlValue(BODY_WIDTH, 2);
    prev_individual = 3;
  }
}

void draw_leg(float rad = 0.25, float rot = 0.0f) {
  glPushMatrix();
  {
    glRotated(rot, 0.0, 0.0, 0);
    glRotated(90, 1.0, 0.0, 0.0);
    drawTextureCylinder(VAL(LEG_LENGTH), rad, rad, VAL(INDIVIDUAL));
  }
  glPopMatrix();
}

struct CatEar {
    float length = 1;
    float width = 2;
    float depth = 1;
    int segment = 4;
    void draw() {
        glPushMatrix();
        glTranslated(-width / 2, 0, 0);
        float dw = width / segment;

#define endpoint {width / 2, 0, depth}
        vector<Triangle> triangle{};
        for (int i = 0; i < segment / 2; i++) {
            float x1 = i * dw;
            float x2 = (i + 1) * dw;
            triangle.push_back({
                {x1, interpolate(x1), 0},
                endpoint,
                {x2, interpolate(x2), 0}
                });
            triangle.push_back({
                endpoint,
                {width - x1, interpolate(x1), 0},
                {width - x2, interpolate(x2), 0}
                });
        }

        drawPolygon(triangle);
        glPopMatrix();
    }

    float interpolate(float x) {
        x = x / width * 15;
        float y = log10((double)x + 1) * 10;
        return min(y, 12) / 12;
    }
};

void draw_head(float head_width, float head_height) {
  glPushMatrix();
  {
    glTranslated(-head_width / 2, -head_height / 2, -0.8 / 2);
    drawBox(head_width, head_height, 0.8);
    glTranslated(0, head_height, 0);
    glRotated(VAL(ROTATE), 1, 0, 0);
    glTranslated(0, -head_height, 0);
    const float ear_width = head_width * 0.4;
    CatEar ear = {VAL(EAR_LENGTH), ear_width, head_height * 0.3, VAL(EAR_TRI_SEG)};

    glPushMatrix();
    glTranslated(ear_width / 2, head_height, 0);
    ear.draw();
    glPopMatrix();

    glPushMatrix();
    glTranslated(head_width - ear_width / 2, head_height, 0);
    ear.draw();
    glPopMatrix();
  }
  glPopMatrix();
}

void draw_tailpart(float r, float scale, float length = 0.8,
                   function<void()> child = nullptr, float radius = 0.2) {
  glPushMatrix();
  {
    glRotated(r, 1.0, 0.0, 0);
    glScaled(scale, scale, scale);
    drawTextureCylinder(length, radius, radius, VAL(INDIVIDUAL));
    glTranslated(0, 0, length);
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

void draw_tail_recursive(float a, float len, float count) {
  if (count == 1) {
    draw_tailpart(a, 1, len);
  } else {
    draw_tailpart(a, 1, len,
                  [&] { draw_tail_recursive(a * 1.3, len, count - 1); });
  }
}

struct TreeModel {
  int turn_angle = 20;
  float radius = 0.05;
  float component_length = 1;
  int depth = 3;
  TreeLSystem sys;
  TreeModel() {
    sys.turn_angle = turn_angle;
    sys.radius = radius;
    sys.component_length = component_length;
    sys.generate_sys_string(depth);
  }
  void draw() {
    glPushMatrix();
    { sys.draw(); }
    glPopMatrix();
  }
};

void draw_cat() {
    glPushMatrix();
    {
        glTranslated(VAL(XPOS), VAL(YPOS), VAL(ZPOS));
        glPushMatrix();
        {
            glTranslated(-VAL(BODY_WIDTH) / 2, VAL(LEG_LENGTH),
                -VAL(BODY_LENGTH) / 2);

            drawTextureQuad(VAL(BODY_WIDTH), VAL(BODY_DEPTH), VAL(BODY_LENGTH),
                VAL(INDIVIDUAL));
            glPushMatrix();
            {
                // front_left
                glTranslated(0.2, 0, 0.2);
                draw_leg();
            }
            glPopMatrix();

            glPushMatrix();
            {
                // front_right
                glTranslated(VAL(BODY_WIDTH) - 0.2, 0, 0.2);
                draw_leg();
            }
            glPopMatrix();

            glPushMatrix();
            {
                // back_right
                glTranslated(VAL(BODY_WIDTH) - 0.2, 0, VAL(BODY_LENGTH) - 0.2);
                draw_leg();
            }
            glPopMatrix();

            glPushMatrix();
            {
                // back_left
                glTranslated(0.2, 0, VAL(BODY_LENGTH) - 0.2);
                draw_leg();
            }
            glPopMatrix();

            glPushMatrix();
            {
                // head
                glTranslated(VAL(BODY_WIDTH) / 2, VAL(BODY_DEPTH), -0.1);
                draw_head(VAL(HEAD_WIDTH), VAL(HEAD_HEIGHT));
            }
            glPopMatrix();

            glPushMatrix();
            {
                glTranslated(VAL(BODY_WIDTH) / 2, VAL(BODY_DEPTH) / 2,
                    VAL(BODY_LENGTH));
                float a = VAL(TAIL_ANGLE);
                run_if_animate([&] { a = animation_progress() * 40 - 10; });
                int components = VAL(NUM_TAIL_COMPONENT);
                float tail_part_length = VAL(TAIL_LENGTH) / components;

                draw_tail_recursive(a, tail_part_length, components);
                TreeModel{}.draw();
            }
            glPopMatrix();
        }
        glPopMatrix();
    }
    glPopMatrix();
}

void CatModel::draw() {
  ModelerView::draw();
  /* animation handling starts */
  if (enable_animation()) {
    ticks = next_ticks();
  }
  /* animation handling ends */
  setIndividual();
  // 1B: dramatic lighting
  GLfloat lightIntensity[]{VAL(LIGHT_INTENSITY), 0, 0, 1};
  GLfloat lightPosition2[]{VAL(LIGHT_XPOS), VAL(LIGHT_YPOS), VAL(LIGHT_ZPOS),
                           0};
  GLfloat lightDiffuse2[]{1, 1, 1, 1};
  glEnable(GL_LIGHT2);
  glLightfv(GL_LIGHT2, GL_POSITION, lightPosition2);
  glLightfv(GL_LIGHT2, GL_DIFFUSE, lightDiffuse2);
  // glLightfv(GL_LIGHT1, GL_DIFFUSE, lightIntensity);
  glLightfv(GL_LIGHT2, GL_DIFFUSE, lightIntensity);

  // draw the floor
  setAmbientColor(.1f, .1f, .1f);
  setDiffuseColor(COLOR_BLUE);
  glPushMatrix();
  {
    glTranslated(-5, 0, -5);
    drawBox(10, 0.01f, 10);
  }
  glPopMatrix();

  // draw the sample model
  setAmbientColor(.1f, .1f, .1f);
  setDiffuseColor(1.0, 1.0, 1.0);
  glPushMatrix();

  // glTranslated(1, 1, 1);

  // drawTorus(0.07, 0.15, 16, 20, 0);
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
  controls[ROTATE] = ModelerControl("Rotate", 0, 30, 1, 0);
  controls[TAIL_ANGLE] = ModelerControl("Tail Curvature", -30, 30, 1, 0);
  controls[TAIL_LENGTH] = ModelerControl("Tail Length", 1.5, 3, 0.1, 1.5);
  controls[NUM_TAIL_COMPONENT] =
      ModelerControl("Number of tail components", 3, 10, 1, 3);
  controls[HEAD_WIDTH] = ModelerControl("Head Width", 1.3, 2, 0.1, 1.3);
  controls[HEAD_HEIGHT] = ModelerControl("Head Height", 1.3, 3, 0.1, 1.3);
  controls[LEG_LENGTH] = ModelerControl("Leg Length", 0.5, 3, 0.1, 1.3);
  controls[EAR_LENGTH] = ModelerControl("Ear Length", 0.5, 2, 0.1, 0.5);
  controls[EAR_TRI_SEG] = ModelerControl("Number of Triangle in ear", 4, 8, 2, 4);

  controls[BODY_DEPTH] = ModelerControl("Body Depth", 1.3, 3, 0.1, 1.3);
  controls[BODY_LENGTH] = ModelerControl("Body Length", 4, 6, 0.1, 4);
  controls[BODY_WIDTH] = ModelerControl("Body Width", 2, 4, 0.1, 2);

  controls[LIGHT_XPOS] = ModelerControl("Light X Position", -10, 10, 0.1f, 0);
  controls[LIGHT_YPOS] = ModelerControl("Light Y Position", 0, 10, 0.1f, 0);
  controls[LIGHT_ZPOS] = ModelerControl("Light Z Position", -10, 10, 0.1f, 0);
  controls[LIGHT_INTENSITY] =
      ModelerControl("Red Light Intensity", 0, 1, 0.1f, 0);
  controls[ANIMATION] = ModelerControl("Enable Animation", 0, 1, 1, 0);

  controls[INDIVIDUAL] = ModelerControl("Individual Instances", 1, 3, 1, 1);

  // initialize texture maps
  initTextureMap();

  ModelerApplication::Instance()->Init(&createSampleModel, controls,
                                       NUMCONTROLS);
  return ModelerApplication::Instance()->Run();
}

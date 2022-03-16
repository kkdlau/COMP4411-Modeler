#pragma once
#include "vec.h"
#include "gl_header.h"
#include "debug_utils.h"
//
#include "modelerapp.h"
#include "modelerdraw.h"
#include "modelerview.h"
#include "texturedraw.hpp"
#include <math.h>
#include "modelerglobals.h"

void wrap_angle(float& a, float constraint) {
  const float half = M_PI / 2;
  if (a < half - constraint) {
    a = half - constraint;
  } else if (a > half + constraint) {
    a  = half + constraint;
  }
}

struct PolarVector
{
  float r;
  float lon;
  float lat;
  PolarVector(const Vec3f &v)
  {
    float x = v[0];
    float y = v[1];
    float z = v[2];
    r = sqrt(x * x + y * y + z * z);
    lon = acos(x / sqrt(x * x + y * y)) * (y < 0 ? -1 : 1);
    lat = acos(z / r);
  }

  PolarVector():PolarVector{{0,0,0}} {

  }

  Vec3f as_vector() const
  {
      return {
          r * sin(lat) * cos(lon),
          r * cos(lat),
          r* sin(lat)* sin(lon)
  };
  }
};

class Segment
{
private:
  Vec3f get_end_point()
  {
    PolarVector pv;
    pv.lon = lon;
    pv.lat = lat;
    pv.r = len;
    return start + pv.as_vector();
  }

public:
  float ANGLE_CONSTRAINT = M_PI / 4; // 45 degree
  Vec3f start;
  Vec3f end;
  float len;
  float lon = 0;
  float lat = 0;
  Segment *par = nullptr;
  Segment *child = nullptr;

  Segment(float x, float y, float z, float len_) : start{x, y, z}
  {
    len = len_;
    end = get_end_point();
  }

  Segment(Segment *p, float len_)
  {
    par = p;
    p->child = this;
    start = par->end;
    len = len_;
    end = get_end_point();
  }

  void move_to(Segment* s = nullptr) {
    if (s == nullptr)
      s = child;
    move_to(child->start);
  }

  void move_to(const Vec3f& v) {
    move_to(v[0], v[1], v[2]);
  }

  Vec3f fit_angle_constraint(Vec3f expected_start, Vec3f expected_end) {
    #if 0
      Segment& parent = *par;
      Vec3f diff = expected_end - expected_start;
      PolarVector wrapped{diff};
      wrap_angle(wrapped.lat, Segment::ANGLE_CONSTRAINT);
      wrap_angle(wrapped.lon, Segment::ANGLE_CONSTRAINT);
      Vec3f new_diff = wrapped.as_vector() - diff;
      return expected_start + new_diff;
    #else
      return expected_start;
    #endif
  }

  void move_to(float x, float y, float z) {
    Vec3f tar{x,y,z};
    Vec3f dir = tar - start;
    PolarVector pv{dir};
    lon = pv.lon;
    lat = pv.lat;
    pv.r = -len;
    start = fit_angle_constraint(tar + pv.as_vector(), tar);
    end = tar;
  }

  void draw() {
    end = get_end_point();
    glPushMatrix();
    {
        debugger("start: x y z: %f, %f, %f", start[0], start[1], start[2]);
        debugger("end: x y z: %f, %f, %f", end[0], end[1], end[2]);
      glTranslated(start[0], start[1], start[2]);
      glRotated((M_PI - lon) / M_PI * 180, 0, 1, 0);
      glRotated((2 * M_PI - (M_PI / 2 - lat)) / M_PI * 180, 1, 0, 0);
      drawTextureCylinder(len, 0.1, 0.1, VAL(INDIVIDUAL));
    }
    glPopMatrix();

    if (child) {
        child->start = end;
        child->draw();

    }
  }
};
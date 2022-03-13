#pragma once
#include "vec.h"
#include "gl_header.h"

//
#include "modelerapp.h"
#include "modelerdraw.h"
#include "modelerview.h"
#include "texturedraw.hpp"
#include <math.h>
#include "modelerglobals.h"
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
        r * sin(lat) * sin(lon),
        r * cos(lat)};
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
    return expected_start;
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
    glPushMatrix();
    {
      glTranslated(start[0], start[1], start[2]);
      glRotated(lon / 3.1415 * 180, 1, 0, 0);
      glRotated(lat / 3.1415 * 180, 0, 1, 0);
      drawTextureCylinder(len, 0.03, 0.03, VAL(INDIVIDUAL));
      if (child)
        child->draw();
    }
    glPopMatrix();
  }
};
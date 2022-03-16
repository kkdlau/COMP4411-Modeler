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

template <typename T> int sgn(T val) {
    return (T(0) < val) - (val < T(0));
}

/**
* constraint should be always positive
*/
void wrap_angle(float& a, float based_angle, float constraint) {
    if (a < 0)
        a += 1 * M_PI;

    //if (based_angle < 0)
    //    based_angle += 2 * M_PI;
    debugger("a: %f - base: %f, constraint: %f", a, based_angle, constraint);
    if (fabs(a - based_angle) <= constraint) {
        return;
    }
    debugger("angle limit exceeded: %f > %f", fabs(a - based_angle), constraint);
    if (a > based_angle) {
        a = based_angle + constraint;
    }
    else {
        a = based_angle - constraint;
    }
    debugger("new angle: %f", a);
}

struct PolarVector
{
  float r;
  float lon;
  float lat;
  PolarVector(const Vec3f &v)
  {
    float x = -v[0];
    float y = v[1];
    float z = v[2];
    r = sqrt(x * x + y * y + z * z);
    lon = acos(x / sqrt(x * x + z * z)) * (z < 0 ? -1 : 1);
    lat = acos(y / r);
  }

  PolarVector():PolarVector{{0,0,0}} {

  }

  Vec3f as_vector() const
  {
      return {
          -r * sin(lat) * cos(lon),
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
      // todo: make the constriant become adjustable
      const float constraint = M_PI / 4;
      if (child != nullptr) {
          Vec3f cdir_vector = child->end - child->start;
          Vec3f this_vector = expected_end - expected_start;

          cdir_vector.normalize();
          this_vector.normalize();

          PolarVector c_pv = PolarVector{ cdir_vector };
          PolarVector this_pv = PolarVector{ this_vector };
          wrap_angle(this_pv.lat, c_pv.lat, constraint);
          wrap_angle(this_pv.lon, c_pv.lon, constraint);
          this_pv.r = -len;
          Vec3f new_pos = this_pv.as_vector() + expected_end;
          debugger_vec3(new_pos);
          debugger_vec3(expected_start);
          return new_pos;
      }
      else {
          return expected_start;
      }
  }

  void forward_fit_constraint() {
      const float constraint = M_PI / 4;
      if (par != nullptr) {
          Vec3f cdir_vector = par->end - par->start;
          Vec3f this_vector = end - start;

          cdir_vector.normalize();
          this_vector.normalize();

          PolarVector c_pv = PolarVector{ cdir_vector };
          PolarVector this_pv = PolarVector{ this_vector };
          // wrap_angle(this_pv.lat, c_pv.lat, constraint);
          wrap_angle(this_pv.lon, c_pv.lon, constraint);
      }

      if (child != nullptr)
          child->forward_fit_constraint();
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
    if (par != nullptr) {
        par->move_to(start);
    }
  }

  void draw() {
    glPushMatrix();
    {
      glTranslated(start[2], start[1], start[0]);
      glRotated((M_PI - lon) / M_PI * 180, 0, 1, 0);
      glRotated((2 * M_PI - (M_PI / 2 - lat)) / M_PI * 180, 1, 0, 0);
      drawTextureCylinder(len, 0.1, 0.1, VAL(INDIVIDUAL));
    }
    glPopMatrix();

    if (child) {
        child->start = this->end;
        child->draw();
    }
  }
};
#pragma once
#include "vec.h"
#include "gl_header.h"
#include "debug_utils.h"
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

  Segment(float x, float y, float z, float len_, float constraint) : start{x, y, z}
  {
    ANGLE_CONSTRAINT = constraint / 180 * M_PI;
    len = len_;
    end = get_end_point();
  }

  Segment(Segment *p, float len_, float constraint)
  {
    ANGLE_CONSTRAINT = constraint / 180 * M_PI;
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
      // todo: make the constraint become adjustable
      const float constraint = M_PI / 4;
      if (0) {
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

  void forward_fit_constraint(Vec3f normal) { // assume the normal is in the right orientation
      const float constraint = ANGLE_CONSTRAINT;
      
      Vec3f this_vector = end - start;
      //printf("this vector %f %f %f \t", this_vector[0], this_vector[1], this_vector[2]);
      this_vector.normalize();
      normal.normalize();
      double theta = acos(this_vector * normal);
      //theta = theta / 180 * M_PI;
      if (theta > constraint) {
          printf("theta exceed constraint %f \n", theta);
          float ratio = constraint / theta;
          printf("ratio is %f\n", ratio);
          Vec3f normal_end = start + normal * len;
          Vec3f new_dir = normal_end + (end - normal_end) * ratio;
          new_dir = new_dir - start;
          new_dir.normalize();
          Vec3f new_end = start + new_dir * len;
          move_to2(new_end);

          this_vector = end - start;
          this_vector.normalize();
          theta = acos(this_vector * normal);
          printf("theta after constraint is %f \n", theta);
      }
      
      if (child != nullptr) {
          printf("checking child\n");
          Vec3f dir = end - start;
          child->forward_fit_constraint(dir);
      }
      /*
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
      */
  }

  void move_to(float x, float y, float z) {
    Vec3f tar{x,y,z};
    Vec3f dir = tar - start;
    PolarVector pv{dir};
    lon = pv.lon;
    lat = pv.lat;
    pv.r = -len;
    //start = fit_angle_constraint(tar + pv.as_vector(), tar);
    start = tar + pv.as_vector();
    end = tar;
    if (par != nullptr) {
        par->move_to(start);
    }
  }
  /*
  * Updates my own end
  * Moves child to new start while preserving direction
  * Doesn't touch parent
  */
  void move_to2(Vec3f point) {
      move_to2(point[0], point[1], point[2]);
  }
  void move_to2(float x, float y, float z) { 
      Vec3f tar{ x, y, z };
      Vec3f dir = tar - start;
      PolarVector pv{ dir };
      lon = pv.lon;
      lat = pv.lat;
      end = tar;
      if (child != nullptr) {
          child->move_start(end);
      }
  }
  void move_start(Vec3f p) {
      move_start(p[0], p[1], p[2]);
  }
  void move_start(float x, float y, float z) {
      PolarVector ori_dir = end - start;
      Vec3f dir = ori_dir.as_vector();
      start = { x, y, z };
      end = start + dir;
      if (child != nullptr) {
          child->move_start(end);
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
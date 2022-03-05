#pragma once

#include "BaseModel.hpp"
#include "debug_utils.h"
#include "modelerdraw.h"
#include "texturedraw.hpp"

class CatHead : public BaseModel {
public:
  CatHead() {}
  void draw() {
    glTranslated(-1.3 / 2, -1.3 / 2, -0.8 / 2);
    drawTextureBody(1.3, 1.3, 0.8);
    float y = 1.3;
    drawTriangle(0, y, 0, 0.5, y, 0, 0.25, 0.25 + y, 0);
    drawTriangle(1.3, y, 0, 1.3 - 0.5, y, 0, 1.3 - 0.25, 0.25 + y, 0);
  }
};

class CatLeg : public BaseModel {
public:
  float radius = 0.25;
  float rotate = 0;
  CatLeg() {}
  void draw() {
    glRotated(rotate, 0.0, 0.0, 0);
    glRotated(-90, 1.0, 0.0, 0.0);
    drawCylinder(1, radius, radius);
  }
};

class TailPart : public BaseModel {
public:
  float radius = 0.2;
  float rotate = 0;
  float length = 0.8;
  TailPart(float r = 0, TailPart *another_tail = nullptr) {
    rotate = r;
    child = another_tail;
    pre_draw_transformation = [&](BaseModel *self) {
      glRotated(rotate, 1.0, 0.0, 0);
    };

    post_draw_transformation = [&](BaseModel *self) {
      glTranslated(0, 0, length);
    };
  }

  void draw() { drawCylinder(length, radius, radius); }
};

class CatTail : public BaseModel {
public:
  CatTail(float r1, float r2, float r3) {
    child = new TailPart(r1, new TailPart(r2, new TailPart(r3)));
  }
};

class CatBody : public BaseModel {
public:
  CatLeg *front_left;
  CatLeg *front_right;
  CatLeg *back_left;
  CatLeg *back_right;
  CatHead *h;
  CatTail *t;

  CatBody(float x, float y, float z) { // (x, y, z) position
    pre_draw_transformation = [&](BaseModel *self) {
      glTranslated(x, y, z);
      glScaled(2, 1, 4);
    };
    post_draw_transformation = [&](BaseModel *self) {
      glScaled(1 / 2.0, 1, 1 / 4.0);
    };

    front_left = new CatLeg;
    front_right = new CatLeg;
    back_left = new CatLeg;
    back_right = new CatLeg;

    front_left->pre_draw_transformation = [&](BaseModel *self) {
      glTranslated(0.2, -1, 0.2);
    };

    front_right->pre_draw_transformation = [&](BaseModel *self) {
      glTranslated(1.8, -1, 0.2);
    };
    back_left->pre_draw_transformation = [&](BaseModel *self) {
      glTranslated(1.8, -1, 3.8);
    };
    back_right->pre_draw_transformation = [&](BaseModel *self) {
      glTranslated(0.2, -1, 3.8);
    };

    h = new CatHead;
    h->pre_draw_transformation = [&](BaseModel *self) {
      // glTranslated(0.35, 1.2, -0.1);
    };

    t = new CatTail{10, 10, 10};
    t->pre_draw_transformation = [&](BaseModel *self) {
      glTranslated(1, 0.5, 4);
    };

    children.push_back(front_left);
    children.push_back(front_right);
    children.push_back(back_left);
    children.push_back(back_right);
    children.push_back(h);
    children.push_back(t);
  }

  void draw() { drawTextureBody(1, 1.3, 1); }

  void render() { BaseModel::render(); }
};

class Cat : public BaseModel {
public:
  CatBody *b;
  Cat(float x, float y, float z) { child = b = new CatBody(x, y, z); }
};

#pragma once
#include "gl_header.h"
#include <functional>
#include <vector>
using namespace std;

class BaseModel {
protected:
  BaseModel() : children{} {}
  BaseModel(BaseModel *child) : children{} {
    this->child = child;
    list = false;
  }

  BaseModel(vector<BaseModel *> &&children) : children{} {
    this->children = children;
    list = true;
  }

public:
  function<void(BaseModel *)> pre_draw_transformation = nullptr;
  function<void(BaseModel *)> post_draw_transformation = nullptr;
  BaseModel *child = nullptr;
  vector<BaseModel *> children;
  bool list;

  /**
   * @brief Low-level function for drawing primitive.
   *
   * I guess you only need this if your component invloves some openGL
   * primitive.
   */
  virtual void draw() {}

  /**
   * @brief Renders children, and components self using Hierarchical Modeling.
   *
   */
  virtual void render() {
    glPushMatrix();
    safe_call(pre_draw_transformation);
    draw();
    safe_call(post_draw_transformation);
    if (children.size()) {
      for (auto c : children) {
        c->render();
      }
    }
    if (child) {
      child->render();
    }

    glPopMatrix();
  }

  virtual ~BaseModel() {
    if (children.size()) {
      for (auto c : children) {
        delete c;
      }
    } else if (child) {
      delete child;
    }
  }

private:
  void safe_call(function<void(BaseModel *)> f) {
    if (f != nullptr)
      f(this);
  }
};
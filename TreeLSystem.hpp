#pragma once

#include "LSystem.hpp"
#include "gl_header.h"
#include "modelerapp.h"
#include "modelerdraw.h"
#include "modelerglobals.h"
#include "modelerview.h"
#include "texturedraw.hpp"


class TreeLSystem : public LSystem {
public:
  int turn_angle = 2;
  float radius = 0.5;
  float component_length = 0.7;
  TreeLSystem() { system_string = "[F]"; }

  void generate_sys_string(int depth) {
    for (int i = 0; i < depth; i++) {
      system_string = encode_string(system_string);
    }
  }

  string translate(const char c) {
    switch (c) {
    case 'F':
        // return "F[F[-[F]][F[+[F]]]]";
        return "F[[-[F]]][[[+[F]]]]";
    default:
        return string{ c };
    }
  }

  void perform(const char c) {
    switch (c) {
    case 'F': {
        //glTranslated(0, component_length, 0);

        //glTranslated(0, 0, component_length / 2);
        //glRotated(-90, 1, 0, 0);
        //glTranslated(-radius, -radius, -component_length / 2);
        //drawTextureCylinder(component_length, radius, radius, VAL(INDIVIDUAL));
        glTranslated(-radius, -radius, 0);
        drawTextureCylinder(component_length, radius, radius, VAL(INDIVIDUAL));
        glTranslated(0, 0, component_length);


    } break;
    case '[': {
      glPushMatrix();
    } break;
    case ']': {
      glPopMatrix();
    } break;
    case '+': {
      glRotated(turn_angle, 0.0, 1.0, 0.0);
    } break;
    case '-': {
      glRotated(-turn_angle, 0.0, 1.0, 0.0);
    } break;
    default:
      break;
    }
  }
};

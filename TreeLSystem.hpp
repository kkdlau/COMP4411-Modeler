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
  TreeLSystem() { system_string = "F+F"; }

  void generate_sys_string(int depth) {
    for (int i = 0; i < depth; i++) {
      system_string = encode_string(system_string);
    }
  }

  string translate(const char c) {
    switch (c) {
    case 'X':
      return "-F[+F][---X]+F-F[++++X]-X";
    case 'F':
      return "FF";
    default:
      return "";
    }
  }

  void perform(const char c) {
    switch (c) {
    case 'X': {

    } break;
    case 'F': {
        glPushMatrix();
        {
            //glTranslated(0, -component_length, 0);
            drawTextureCylinder(component_length, radius, radius, VAL(INDIVIDUAL));
        } glPopMatrix();
      glTranslated(0, component_length, 0);
    } break;
    case '[': {
      glPushMatrix();
    } break;
    case ']': {
      glPopMatrix();
    } break;
    case '+': {
      glRotated(45, 0.0, 0.0, 0.0);
    } break;
    case '-': {
      glRotated(-turn_angle, 0.0, 1.0, 0.0);
    } break;
    default:
      break;
    }
  }
};

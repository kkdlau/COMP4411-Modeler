#ifndef TEXTUREDRAW_HPP
#define TEXTUREDRAW_HPP

#include "modelerdraw.h"
#include <FL/gl.h>

#include "bitmap.h"
#include "gl_header.h"
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <math.h>

const int NUM_TEXTURES = 3;
int texImageWidth[NUM_TEXTURES];
int texImageHeight[NUM_TEXTURES];
GLubyte *texImage[NUM_TEXTURES]{nullptr, nullptr, nullptr};
GLuint texImageId[NUM_TEXTURES];
char *texImagePaths[NUM_TEXTURES]{
#ifdef __APPLE__
    "/Users/dannylau/Desktop/COMP4411-Modeler/textures/fur.bmp",
#else
    "textures/fur.bmp",
#endif
#ifdef __APPLE__
    "/Users/dannylau/Desktop/COMP4411-Modeler/textures/grey.bmp",
#else
    "textures/grey.bmp",
#endif
#ifdef __APPLE__
    "/Users/dannylau/Desktop/COMP4411-Modeler/textures/spots.bmp",
#else
    "textures/spots.bmp",
#endif
};
extern void _setupOpenGl();

void initTextureMap() {
  printf("Initializing texture maps\n");
  glPixelStorei(GL_UNPACK_ALIGNMENT, 4);

  for (int i = 0; i < NUM_TEXTURES; ++i) {
    printf("the tex image id is %d\n", texImageId[i]);
    char *path = texImagePaths[i];
    texImage[i] = readBMP(path, texImageWidth[i], texImageHeight[i]);
    if (!texImage[i]) {
      printf("Could not load the texture map\n");
      continue;
    }
    printf("Loaded texture image %d %d\n", texImageWidth[i], texImageHeight[i]);
  }
  glGenTextures(3, texImageId);
}

void drawTextureQuad(double x, double y, double z, int individual) {
  individual = individual - 1; // change from UI value to index
  ModelerDrawState *mds = ModelerDrawState::Instance();

  _setupOpenGl();
  if (mds->m_rayFile) {
    printf("Oops I don't care about the previous version of Modeler\n");
  } else {
    /* remember which matrix mode OpenGL was in. */
    int savemode;
    glGetIntegerv(GL_MATRIX_MODE, &savemode);
    glGetIntegerv(GL_TEXTURE_BINDING_2D, (GLint *)&texImageId[individual]);
    /* switch to the model matrix and scale by x,y,z. */
    glMatrixMode(GL_MODELVIEW);

    glBindTexture(GL_TEXTURE_2D, texImageId[individual]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texImageWidth[individual],
                 texImageHeight[individual], 0, GL_RGB, GL_UNSIGNED_BYTE,
                 texImage[individual]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    glEnable(GL_TEXTURE_2D);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE,
              GL_MODULATE); // default is GL_MODULATE for interacting with light
    glBindTexture(GL_TEXTURE_2D, texImageId[individual]); // repeat?

    glPushMatrix();
    glScaled(x, y, z);

    glBegin(GL_QUADS);

    glNormal3d(0.0, 0.0, -1.0);
    glTexCoord2f(0.0, 0.0);
    glVertex3d(0.0, 0.0, 0.0);
    glTexCoord2f(0.0, 1.0);
    glVertex3d(0.0, 1.0, 0.0);
    glTexCoord2f(1.0, 1.0);
    glVertex3d(1.0, 1.0, 0.0);
    glTexCoord2f(1.0, 0.0);
    glVertex3d(1.0, 0.0, 0.0);

    glNormal3d(0.0, -1.0, 0.0);
    glTexCoord2f(0.0, 0.0);
    glVertex3d(0.0, 0.0, 0.0);
    glTexCoord2f(0.0, 1.0);
    glVertex3d(1.0, 0.0, 0.0);
    glTexCoord2f(1.0, 1.0);
    glVertex3d(1.0, 0.0, 1.0);
    glTexCoord2f(1.0, 0.0);
    glVertex3d(0.0, 0.0, 1.0);

    glNormal3d(-1.0, 0.0, 0.0);
    glTexCoord2f(0.0, 0.0);
    glVertex3d(0.0, 0.0, 0.0);
    glTexCoord2f(0.0, 1.0);
    glVertex3d(0.0, 0.0, 1.0);
    glTexCoord2f(1.0, 1.0);
    glVertex3d(0.0, 1.0, 1.0);
    glTexCoord2f(1.0, 0.0);
    glVertex3d(0.0, 1.0, 0.0);

    glNormal3d(0.0, 0.0, 1.0);
    glTexCoord2f(0.0, 0.0);
    glVertex3d(0.0, 0.0, 1.0);
    glTexCoord2f(0.0, 1.0);
    glVertex3d(1.0, 0.0, 1.0);
    glTexCoord2f(1.0, 1.0);
    glVertex3d(1.0, 1.0, 1.0);
    glTexCoord2f(1.0, 0.0);
    glVertex3d(0.0, 1.0, 1.0);

    glNormal3d(0.0, 1.0, 0.0);
    glTexCoord2f(0.0, 0.0);
    glVertex3d(0.0, 1.0, 0.0);
    glTexCoord2f(0.0, 1.0);
    glVertex3d(0.0, 1.0, 1.0);
    glTexCoord2f(1.0, 1.0);
    glVertex3d(1.0, 1.0, 1.0);
    glTexCoord2f(1.0, 0.0);
    glVertex3d(1.0, 1.0, 0.0);

    glNormal3d(1.0, 0.0, 0.0);
    glTexCoord2f(0.0, 0.0);
    glVertex3d(1.0, 0.0, 0.0);
    glTexCoord2f(0.0, 1.0);
    glVertex3d(1.0, 1.0, 0.0);
    glTexCoord2f(1.0, 1.0);
    glVertex3d(1.0, 1.0, 1.0);
    glTexCoord2f(1.0, 0.0);
    glVertex3d(1.0, 0.0, 1.0);

    glEnd();
    glDisable(GL_TEXTURE_2D);
    /* restore the model matrix stack, and switch back to the matrix
    mode we were in. */
    glPopMatrix();
    glMatrixMode(savemode);
  }
}

void drawTextureCylinder(double h, double r1, double r2, int individual) {
  individual = individual - 1; // change from UI value to index
  ModelerDrawState *mds = ModelerDrawState::Instance();
  _setupOpenGl();
  if (mds->m_rayFile) {
    printf("Oops I don't care about the previous version of Modeler\n");
  } else {
    glGetIntegerv(GL_TEXTURE_BINDING_2D, (GLint *)&texImageId[individual]);
    glBindTexture(GL_TEXTURE_2D, texImageId[individual]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texImageWidth[individual],
                 texImageHeight[individual], 0, GL_RGB, GL_UNSIGNED_BYTE,
                 texImage[individual]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    glEnable(GL_TEXTURE_2D);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE,
              GL_MODULATE); // default is GL_MODULATE for interacting with light
    glBindTexture(GL_TEXTURE_2D, texImageId[individual]); // repeat?

    drawCylinder(h, r1, r2);

    glDisable(GL_TEXTURE_2D);
  }
}
#endif
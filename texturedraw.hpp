#ifndef TEXTUREDRAW_HPP
#define TEXTUREDRAW_HPP 

#include "modelerdraw.h"
#include <FL/gl.h>

#include "gl_header.h"
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <math.h>
#include "bitmap.h"

int texImageWidth;
int texImageHeight;
GLubyte* texImage = nullptr;
GLuint texImageId;

extern void _setupOpenGl();

void initTextureMap() {
    printf("Initializing texture map\n");
    glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
    
    printf("the tex image id is %d\n", texImageId);
    delete texImage;
    texImage = readBMP("fur.bmp", texImageWidth, texImageHeight);
    if (!texImage) {
        printf("Could not load the fur texture map\n");
        return;
    }
    glGenTextures(1, &texImageId);

    printf("Loaded texture image %d %d\n", texImageWidth, texImageHeight);
}

void drawTextureBody(double x, double y, double z) {
    ModelerDrawState* mds = ModelerDrawState::Instance();

    _setupOpenGl();
    if (mds->m_rayFile) {
        printf("Oops I don't care about the previous version of Modeler\n");
    }
    else {
        /* remember which matrix mode OpenGL was in. */
        int savemode;
        glGetIntegerv(GL_MATRIX_MODE, &savemode);
        glGetIntegerv(GL_TEXTURE_BINDING_2D, (GLint*)&texImageId);
        /* switch to the model matrix and scale by x,y,z. */
        glMatrixMode(GL_MODELVIEW);

        glBindTexture(GL_TEXTURE_2D, texImageId);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texImageWidth,
            texImageHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, texImage);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        
        glEnable(GL_TEXTURE_2D);
        glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL); // default is GL_MODULATE for interacting with light
        glBindTexture(GL_TEXTURE_2D, texImageId); // repeat?

        glPushMatrix();
        glScaled(x, y, z);
        
        
        glBegin(GL_QUADS);

        glNormal3d(0.0, 0.0, -1.0);
        glTexCoord2f(0.0, 0.0); glVertex3d(0.0, 0.0, 0.0);
        glTexCoord2f(0.0, 1.0); glVertex3d(0.0, 1.0, 0.0);
        glTexCoord2f(1.0, 1.0); glVertex3d(1.0, 1.0, 0.0);
        glTexCoord2f(1.0, 0.0); glVertex3d(1.0, 0.0, 0.0);

        glNormal3d(0.0, -1.0, 0.0);
        glTexCoord2f(0.0, 0.0); glVertex3d(0.0, 0.0, 0.0);
        glTexCoord2f(0.0, 1.0); glVertex3d(1.0, 0.0, 0.0);
        glTexCoord2f(1.0, 1.0); glVertex3d(1.0, 0.0, 1.0);
        glTexCoord2f(1.0, 0.0); glVertex3d(0.0, 0.0, 1.0);

        glNormal3d(-1.0, 0.0, 0.0);
        glTexCoord2f(0.0, 0.0); glVertex3d(0.0, 0.0, 0.0);
        glTexCoord2f(0.0, 1.0); glVertex3d(0.0, 0.0, 1.0);
        glTexCoord2f(1.0, 1.0); glVertex3d(0.0, 1.0, 1.0);
        glTexCoord2f(1.0, 0.0); glVertex3d(0.0, 1.0, 0.0);

        glNormal3d(0.0, 0.0, 1.0);
        glTexCoord2f(0.0, 0.0); glVertex3d(0.0, 0.0, 1.0);
        glTexCoord2f(0.0, 1.0); glVertex3d(1.0, 0.0, 1.0);
        glTexCoord2f(1.0, 1.0); glVertex3d(1.0, 1.0, 1.0);
        glTexCoord2f(1.0, 0.0); glVertex3d(0.0, 1.0, 1.0);

        glNormal3d(0.0, 1.0, 0.0);
        glTexCoord2f(0.0, 0.0); glVertex3d(0.0, 1.0, 0.0);
        glTexCoord2f(0.0, 1.0); glVertex3d(0.0, 1.0, 1.0);
        glTexCoord2f(1.0, 1.0); glVertex3d(1.0, 1.0, 1.0);
        glTexCoord2f(1.0, 0.0); glVertex3d(1.0, 1.0, 0.0);

        glNormal3d(1.0, 0.0, 0.0);
        glTexCoord2f(0.0, 0.0); glVertex3d(1.0, 0.0, 0.0);
        glTexCoord2f(0.0, 1.0); glVertex3d(1.0, 1.0, 0.0);
        glTexCoord2f(1.0, 1.0); glVertex3d(1.0, 1.0, 1.0);
        glTexCoord2f(1.0, 0.0); glVertex3d(1.0, 0.0, 1.0);

        glEnd();
        glDisable(GL_TEXTURE_2D);
        /* restore the model matrix stack, and switch back to the matrix
        mode we were in. */
        glPopMatrix();
        glMatrixMode(savemode);
    }
    
}
#endif
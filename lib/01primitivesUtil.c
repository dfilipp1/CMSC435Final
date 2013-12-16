
#ifdef __APPLE__
  #include <GLUT/glut.h>
#else
  #include <GL/glut.h>
#endif

#include "01primitivesUtil.h"

/*
void uViewInit(GLfloat ezp,  GLfloat exr, GLfloat eyr)
{
  eyeZPos = ezp;
  eyeXRot = exr;
  eyeYRot = eyr;
}

void uUpdateViewPoint(GLfloat newEyeZPos, GLfloat newEyeXRot, GLfloat newEyeYRot)
{
  glTranslatef(0, 0, -newEyeZPos);
  glRotatef(newEyeXRot, 1, 0, 0);
  glRotatef(newEyeYRot, 0, 1, 0);
}
*/

void uMakeCoordList(GLuint coordIndex)
{
  int i, j;

  draw_coord = 1;
  //GLfloat coord_light[] = {0.5, 0., 0., 1.};

  //glPushAttrib(GL_LIGHTING_BIT);

  glNewList(coordIndex, GL_COMPILE);
  //glMaterialfv(GL_FRONT, GL_AMBIDENT_AND_DIFFUSE, coord_light);
  glBegin(GL_LINES);
    glColor3f(1, 0, 0); glVertex3f(-2, 0, 0); glVertex3f(10, 0, 0); // x
	glColor3f(0, 1, 0); glVertex3f(0, -2, 0); glVertex3f(0, 10, 0); // y
	glColor3f(0, 0, 1); glVertex3f(0, 0, -2); glVertex3f(0, 0, 10); // z
  glEnd();
  glEndList();

  //glPopAttrib();
}

void uMakeGroundList(GLuint groundIndex)
{
  draw_ground = 1;

  glNewList(groundIndex, GL_COMPILE);
  // draw a big grid plane whatever..
  glEndList();
}

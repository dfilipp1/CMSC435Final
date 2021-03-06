/*
 *  Simple trackball-like motion adapted (ripped off) from projtex.c
 *  (written by David Yu and David Blythe).  See the SIGGRAPH '96
 *  Advanced OpenGL course notes.
 */


/* includes */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>
#include "trackball.h"

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

 GLfloat tbx = 0;
 GLfloat tby = 0;
 GLfloat zoom = 5;

/* globals */
static GLuint    tb_lasttime;
static GLfloat   tb_lastposition[3];

static GLfloat   tb_angle = 0.0;
static GLfloat   tb_axis[3];

static GLuint    tb_width;
static GLuint    tb_height;

static GLint     tb_button = -1;
static GLboolean tb_tracking = GL_FALSE;
static GLboolean tb_animate = GL_FALSE;
GLfloat   tb_transform[4][4];


/* functions */
static void
_tbPointToVector(int x, int y, int width, int height, float v[3])
{
  float d, a;

  /* project x, y onto a hemi-sphere centered within width, height. */
  v[0] = (2.0 * x - width) / width;
  v[1] = (height - 2.0 * y) / height;
  d = sqrt(v[0] * v[0] + v[1] * v[1]);
  v[2] = cos((3.14159265 / 2.0) * ((d < 1.0) ? d : 1.0));
  a = 1.0 / sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
  v[0] *= a;
  v[1] *= a;
  v[2] *= a;
}

static void
_tbAnimate(void)
{
  return;
  glutPostRedisplay();
}

void
_tbStartMotion(int x, int y, int button, int time)
{
  assert(tb_button != -1);

  tb_tracking = GL_TRUE;
  tb_lasttime = time;
  _tbPointToVector(x, y, tb_width, tb_height, tb_lastposition);
}

void
_tbStopMotion(int button, unsigned time)
{
  assert(tb_button != -1);

  tb_tracking = GL_FALSE;

  
  if (time == tb_lasttime && tb_animate) {
    glutIdleFunc(_tbAnimate);
  } else {
    tb_angle = 0.0;
    if (tb_animate)
      glutIdleFunc(0);
  }
}

void
tbAnimate(GLboolean animate)
{
  return;
  tb_animate = animate;
}

void
tbInit(GLuint button)
{
  tb_button = button;
  tb_angle = 0.0;

  /* put the identity in the trackball transform */
  glPushMatrix();
  glLoadIdentity();
  glGetFloatv(GL_MODELVIEW_MATRIX, (GLfloat *)tb_transform);
  glPopMatrix();
}

void
tbMatrix()
{
  assert(tb_button != -1);

//printf("tb_angle = %f\n", tb_angle);
  glPushMatrix();
  glLoadIdentity();
  glRotatef(tb_angle, tb_axis[0], tb_axis[1], tb_axis[2]);
  glMultMatrixf((GLfloat *)tb_transform);
  glGetFloatv(GL_MODELVIEW_MATRIX, (GLfloat *)tb_transform);
  glPopMatrix();

  glMultMatrixf((GLfloat *)tb_transform);
}

void
tbReshape(int width, int height)
{
  assert(tb_button != -1);

  tb_width  = width;
  tb_height = height;
}

void
tbMouse(int button, int state, int x, int y)
{
  assert(tb_button != -1);
/*
  if (state == GLUT_DOWN && (button == tb_button || GLUT_RIGHT_BUTTON))
    _tbStartMotion(x, y, button, glutGet(GLUT_ELAPSED_TIME));
  else if (state == GLUT_UP && (button == tb_button || GLUT_RIGHT_BUTTON))
    _tbStopMotion(button, glutGet(GLUT_ELAPSED_TIME));
*/
  if (state == GLUT_DOWN) 
    _tbStartMotion(x, y, button, glutGet(GLUT_ELAPSED_TIME));
  else if (state == GLUT_UP)
    _tbStopMotion(button, glutGet(GLUT_ELAPSED_TIME));

}

void
tbMotion(int x, int y, int button)
{
  GLfloat current_position[3], dx, dy, dz;
  int i, j;

  assert(tb_button != -1);

  if (tb_tracking == GL_FALSE)
    return;

  _tbPointToVector(x, y, tb_width, tb_height, current_position);

  /* calculate the angle to rotate by (directly proportional to the
     length of the mouse movement */
  dx = current_position[0] - tb_lastposition[0];
  dy = current_position[1] - tb_lastposition[1];
  dz = current_position[2] - tb_lastposition[2];

  if(button==GLUT_RIGHT_BUTTON) {
      tbZoom(dy); 
      return;
  }

  if(button==GLUT_LEFT_BUTTON) {
      //Find_Nearest(x, y);
      return;
   }

  tb_angle = 3*90.0 * sqrt(dx * dx + dy * dy + dz * dz);

  /* calculate the axis of rotation (cross product) */
  tb_axis[0] = tb_lastposition[1] * current_position[2] - 
               tb_lastposition[2] * current_position[1];
  tb_axis[1] = tb_lastposition[2] * current_position[0] - 
               tb_lastposition[0] * current_position[2];
  tb_axis[2] = tb_lastposition[0] * current_position[1] - 
               tb_lastposition[1] * current_position[0];

  /* reset for next time */
  tb_lasttime = glutGet(GLUT_ELAPSED_TIME);
  tb_lastposition[0] = current_position[0];
  tb_lastposition[1] = current_position[1];
  tb_lastposition[2] = current_position[2];

  /* remember to draw new position */
  glutPostRedisplay();
}

/*
=======================================================
*/
void
tbZoom(float dy)
{

  zoom += dy;

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

  glTranslatef(tbx, tby, -10.+zoom);

}

void
translate(float dx, float dy) {
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

  tbx+=dx;
  tby+=dy;

  glTranslatef(tbx, tby, -10.+zoom);
}


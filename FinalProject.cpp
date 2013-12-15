//Created by: Zachary Garbarino
//Due on: 12/16/2013
//Demo on: 12/10/2013

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <GL/glui.h>

#ifdef __APPLE__
  #include <GLUT/glut.h>
#else
  #include <GL/glut.h>
#endif

#include <math.h>
#include "trackball.h"
#include <iostream>

#include "CLUSTER_STRUCT.hpp"


vector<POINT> *points;
int clusters;
GLint mouseButton = -1;
char * filename;
int main_window;
int scaleunit = 1;

double * getNewDistant(POINT *p);
double * getRGB(int hue, double sat, double val);

void init(){
  //Getting the clustered points
  glClearColor(0,0,0,0);
  tbInit(GLUT_LEFT_BUTTON);
  points = load_points(filename, clusters);
  
}

void display(void){
  glClear(GL_COLOR_BUFFER_BIT);

  glPushMatrix();
  tbMatrix();
  //Drawing the points
  glPointSize(5);
 
  for (int i = 0; i < points->size(); i++){
    glBegin(GL_POINTS);
    double * coordinates = (*points)[i].coor;

    double *rgb = getRGB(360/clusters * (*points)[i].cluster->ref,1,1);
    glColor3f(rgb[0],rgb[1],rgb[2]);
    free(rgb);

    double * center = getNewDistant(&(*points)[i]);
    //glVertex3f(coordinates[0],coordinates[1],coordinates[2]);
    glVertex3f(center[0]/40.0,center[1]/40.0,center[2]/40.0);
    glEnd();
    
  }
  
 
  glPopMatrix();
  
  glutSwapBuffers();
}

//Get the RGB values each with a range of 0-1. Passing in the hue sat and val
double * getRGB(int hue, double sat, double val){
 
  GLfloat h = fmod(hue/60.0,2);
  GLfloat c = (sat) * (val);
  
  GLfloat x = c * (1 - fabs(h-1));
  GLfloat m = sat - c;

  GLfloat R = c+m;
  GLfloat G = x+m;
  GLfloat B = m;
  
  if (0 <= hue && hue < 60){
    R = c+m;
    B = x+m;
    G = m;
  }
  else if (60 <= hue && hue < 120){
    R = x+m;
    G = c+m;
    B = m;
  }
  else if (120 <= hue && hue < 180){
    R = m;
    G = c+m;
    B = x+m;
  }
  else if (180 <= hue && hue < 240){
    R = m;
    G = x+m;
    B = c+m;
  }
  else if (240 <= hue && hue < 300){
    R = x+m;
    G = m;
    B = c+m;
  }
  else if (300 <= hue && hue < 360){
    R = c+m;
    G = m;
    B = x+m;
  }

  //Store the value in a return array
  double* returnC = new double[3];
  returnC[0] = R;
  returnC[1] = G;
  returnC[2] = B;
  //Return the color array
  return returnC;
}

double * getNewDistant(POINT *p){
  double * center = new double[3];
  double * vecSub = new double[3];
 
  vecSub[0] = p->cluster->center[0] - 0;
  vecSub[1] = p->cluster->center[1] - 0;
  vecSub[2] = p->cluster->center[2] - 0;

  center[0] = p->coor[0] + (.1*scaleunit)*vecSub[0];
  center[1] = p->coor[1] + (.1*scaleunit)*vecSub[1];
  center[2] = p->coor[2] + (.1*scaleunit)*vecSub[2]; 

  return center;
}

void reshape(int w, int h){
  tbReshape(w, h);
  cout<<"Reshape"<<endl;
  glViewport(0, 0, w, h);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(60.0, (GLdouble)w/h, 0.01,1000);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  glTranslatef(0, 0, -10+zoom);
}

void mouse(int button, int state, int x, int y){
  
  mouseButton=button;
  tbMouse(button, state, x,y);
  glutPostRedisplay();
}

void motion(int x, int y){
  tbMotion(x,y,mouseButton);
  glutPostRedisplay();
}

void keyboard(unsigned char key, int x, int y){
  switch (key) {
  case 27:
    exit(0);
    break;
  default:
    break;
  }
  glutPostRedisplay();
}

void special(int key, int x, int y){
  
}

void idlefunction(void){
  if (glutGetWindow() != main_window)
    glutSetWindow(main_window);
  glutPostRedisplay();
}

void control_callback(int control){
  if (control == 100)
    points = load_points(filename, clusters);
  glutPostRedisplay();
}


int main(int argc, char *argv[]){
  //Local Variables	

  //Make sure that there is a command-line argument
  if (argc < 2) {
    std::cout << "usage: FinalProject <datafile> <number of clusters>" << std::endl;
    exit(-1);
  }
  else {
    //The first command-line arg is the name of the file to use
    filename = argv[1];
    //num_of_clusters = atoi(argv[2]);
    clusters = 2; //Just for debugging, the acutal number of clusters is determine after file read
  }
  
  glutInit(&argc, argv);
  glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB);
  glutInitWindowSize (500, 500);
  glutInitWindowPosition (100, 100);
  main_window = glutCreateWindow (argv[0]);
  glutFullScreen();
  
  init();

  glutDisplayFunc(display);
  glutReshapeFunc(reshape);
  glutMouseFunc(mouse);
  glutMotionFunc(motion);
  
  glutKeyboardFunc(keyboard);
  //////////////////////////////// CONTROLS /////////////////
  GLUI_Master.set_glutIdleFunc(idlefunction);

  //Creating a subwindow to the right which house our controls
  GLUI *glui = GLUI_Master.create_glui_subwindow(main_window,GLUI_SUBWINDOW_RIGHT);
  
  //Panel for the controls
  GLUI_Panel *control = new GLUI_Panel(glui,"Controls");
  glui->set_main_gfx_window(main_window);

  GLUI_Spinner *spinner = new GLUI_Spinner(control, "Clusters", GLUI_SPINNER_INT,&clusters,100,control_callback);
  spinner->set_int_limits(2,10,GLUI_LIMIT_CLAMP);
  spinner->set_speed(0);

  GLUI_Spinner *scale_unit = new GLUI_Spinner(control,"Seperate", GLUI_SPINNER_INT,&scaleunit,200,control_callback);
  scale_unit->set_int_limits(1,15,GLUI_LIMIT_CLAMP);
  scale_unit->set_speed(0);

  ///////////////////////////////////////////////////////////
  glutMainLoop();
  return 0;
}

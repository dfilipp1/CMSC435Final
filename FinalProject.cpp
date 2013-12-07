//Created by: Zachary Garbarino
//Due on: 12/16/2013
//Demo on: 12/10/2013

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
GLint mouseButton = -1;

void init(char * filename, int clusters){
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
  glBegin(GL_POINTS);
  for (int i = 0; i < points->size(); i++){
    double * coordinates = (*points)[i].coor;
    if ((*points)[i].cluster->ref == 0)
      glColor3f(1,1,1);
    else if ((*points)[i].cluster->ref == 1)
      glColor3f(1,0,0);
    else if ((*points)[i].cluster->ref == 2)
      glColor3f(0,1,0);
    else if ((*points)[i].cluster->ref == 3)
      glColor3f(0,0,1);
    else if ((*points)[i].cluster->ref == 4)
      glColor3f(0,1,1);
    
    glVertex3f(coordinates[0]/40.0,coordinates[1]/40.0,coordinates[2]/40.0);
    //cout<<coordinates[0]<<" "<<coordinates[1]<<" "<<coordinates[2]<<endl;
  }
  
  glEnd();
  glPopMatrix();
  
  glutSwapBuffers();
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
}

void special(int key, int x, int y){
  
}

int main(int argc, char *argv[]){
  //Local Variables
  char *filename;	
  int num_of_clusters;

  //Make sure that there is a command-line argument
  if (argc < 3) {
    std::cout << "usage: FinalProject <datafile> <number of clusters>" << std::endl;
    exit(-1);
  }
  else {
    //The first command-line arg is the name of the file to use
    filename = argv[1];
    num_of_clusters = atoi(argv[2]);
  }
  
  glutInit(&argc, argv);
  glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB);
  glutInitWindowSize (500, 500);
  glutInitWindowPosition (100, 100);
  glutCreateWindow (argv[0]);
  
  init(filename,num_of_clusters);

  glutDisplayFunc(display);
  glutReshapeFunc(reshape);
  glutMouseFunc(mouse);
  glutMotionFunc(motion);
  
  glutKeyboardFunc(keyboard);
  glutMainLoop();
  return 0;
}

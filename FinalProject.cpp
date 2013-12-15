//Created by: Zachary Garbarino, Dustin Filippi
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
#include "bundling.hpp"

#define SCALE 40.0

vector<POINT> *points;
GLint mouseButton = -1;
int num_of_clusters;

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
    
    glVertex3f(coordinates[0]/SCALE,coordinates[1]/SCALE,coordinates[2]/SCALE);
    //cout<<coordinates[0]<<" "<<coordinates[1]<<" "<<coordinates[2]<<endl;
  }
    
  glEnd();

  glPopMatrix();
  
  double centers[num_of_clusters][4];
  
    for (std::vector<POINT>::iterator it=points->begin(); it<points->end(); it++) {
        centers[(*it).cluster->ref][0]+=(*it).coor[0];
        centers[(*it).cluster->ref][1]+=(*it).coor[1];
        centers[(*it).cluster->ref][2]+=(*it).coor[2];
        centers[(*it).cluster->ref][3]+=1;
    }
    for (int cluster = 0; cluster<num_of_clusters; cluster++) {
        centers[cluster][0]/=centers[cluster][3];
        centers[cluster][1]/=centers[cluster][3];
        centers[cluster][2]/=centers[cluster][3];
    }//Find the center of each cluster


    GLfloat Ctrl[4][3];
    for (std::vector<POINT>::iterator this_cluster=points->begin(); this_cluster<points->end(); this_cluster++) {
        for (std::vector<POINT>::iterator other_cluster=points->begin(); other_cluster<points->end(); other_cluster++) {
            if ((*this_cluster).cluster->ref == (*other_cluster).cluster->ref) {
                continue;
            }
            Ctrl[0][0] = (*this_cluster).coor[0];
            Ctrl[0][1] = (*this_cluster).coor[1];
            Ctrl[0][2] = (*this_cluster).coor[2];
            Ctrl[1][0] = centers[(*this_cluster).cluster->ref][0];
            Ctrl[1][1] = centers[(*this_cluster).cluster->ref][1];
            Ctrl[1][2] = centers[(*this_cluster).cluster->ref][2];
            Ctrl[2][0] = centers[(*other_cluster).cluster->ref][0];
            Ctrl[2][1] = centers[(*other_cluster).cluster->ref][1];
            Ctrl[2][2] = centers[(*other_cluster).cluster->ref][2];
            Ctrl[3][0] = (*other_cluster).coor[0];
            Ctrl[3][1] = (*other_cluster).coor[1];
            Ctrl[3][2] = (*other_cluster).coor[2];
    		draw_curve(Ctrl);
		}
    }//Compute the control points

  for(int this_center = 0; this_center<num_of_clusters; this_center++){
  	for(int next_center = this_center+1; next_center<num_of_clusters; next_center++){
	  glLineWidth(2.5); 
	  glColor3f(1.0, 1.0, 1.0);
	  glBegin(GL_LINES);
	  glVertex3f(centers[this_center][0]/SCALE,centers[this_center][1]/SCALE, centers[this_center][2]/SCALE);
	  glVertex3f(centers[next_center][0]/SCALE,centers[next_center][1]/SCALE, centers[next_center][2]/SCALE);
	  glVertex3f(15, 0, 0);
	  glEnd();
	}
  }

  for(std::vector<POINT>::iterator this_point=points->begin(); this_point<points->end(); this_point++){
  	glLineWidth(2.5); 
	glColor3f(1.0, 1.0, 1.0);
	glBegin(GL_LINES);
	glVertex3f((*this_point).coor[0]/SCALE,(*this_point).coor[1]/SCALE,(*this_point).coor[2]/SCALE);
	glVertex3f(centers[(*this_point).cluster->ref][0]/SCALE,centers[(*this_point).cluster->ref][1]/SCALE,centers[(*this_point).cluster->ref][2]/SCALE);
	glEnd();
  }

  std::cout << "done" << std::endl;

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

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

void init(char * filename, int clusters){
  vector<POINT> *points = load_points(filename, clusters);
  printf("HELLO WORLD\n");
}

void display(void){
  
}

void reshape(int w, int h){

}

void mouse(int button, int state, int x, int y){
  
}

void motion(int x, int y){
  
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
  glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
  glutInitWindowSize (500, 500);
  glutInitWindowPosition (100, 100);
  glutCreateWindow (argv[0]);
  
  init(filename,num_of_clusters);

  glutDisplayFunc(display);
  glutReshapeFunc(reshape);
  glutMouseFunc(mouse);
  glutMotionFunc(motion);
  
  glutKeyboardFunc(keyboard);
  glutSpecialFunc(special);
  
  glutMainLoop();
  return 0;
}

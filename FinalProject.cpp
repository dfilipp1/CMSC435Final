//Created by: Zachary Garbarino, Dustin Filippi Sothiara Em
//Due on: 12/16/2013
//Demo on: 12/10/2013

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <GL/glui.h>
#include <iostream>
#include <sstream>
#include <fstream>

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
vector<CLUSTER> *cluster_ref;
int clusters;
GLint mouseButton = -1;
int main_window;
int scaleunit = 0;

int draw_center = 0;
int toggle_color = 1;

float con_val[74][74];

GLfloat lowerbound = 0.0;
GLfloat upperbound = 1.0;

char* filename;
char* matrix_file;

double * getNewDistant(POINT *p);
double * getRGB(int hue, double sat, double val);
void assignColorToCluster();

GLUI_Spinner *upper_spinner;
GLUI_Spinner *lower_spinner;

CLUSTER_POINT * cluster_point;

void init(){
  //Getting the clustered points
  glClearColor(0,0,0,0);
  tbInit(GLUT_LEFT_BUTTON);
  cluster_point = load_points(filename, clusters);
  points = cluster_point->points;
  cluster_ref = cluster_point->clusters;
  assignColorToCluster();

  //Getting the value from the matrix tables
  ifstream file;
  file.open(matrix_file);
  string line;
  double *value;
  
  int row = 0;
  while (getline(file,line)){
    vector<string> elems;
    split(line,' ',elems);
    for (int i = 0;i < 74; i++){
      con_val[row][i] = atof(elems[i].c_str());
    }
    row++;
  }

  file.close();
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
    
    double *rgb = getRGB((*points)[i].cluster->color[0],1,1);
    glColor3f(rgb[0],rgb[1],rgb[2]);
    free(rgb);
    
    double * center = getNewDistant(&(*points)[i]);
    glVertex3f(center[0]/40.0,center[1]/40.0,center[2]/40.0);
    glEnd();
   
  }
  glEnd();
  
  double centers[clusters][4];
  
  for (std::vector<POINT>::iterator it=points->begin(); it<points->end(); it++) {
    centers[(*it).cluster->ref][0]+=(*it).coor[0];
    centers[(*it).cluster->ref][1]+=(*it).coor[1];
    centers[(*it).cluster->ref][2]+=(*it).coor[2];
    centers[(*it).cluster->ref][3]+=1;
  }
  for (int cluster = 0; cluster<clusters; cluster++) {
    centers[cluster][0]/=centers[cluster][3];
    centers[cluster][1]/=centers[cluster][3];
    centers[cluster][2]/=centers[cluster][3];
  }//Find the center of each cluster
  
  int this_point_count = 0;
  int other_point_count = 0;
  
  GLfloat Ctrl[4][3];
  for (std::vector<POINT>::iterator this_cluster=points->begin(); this_cluster<points->end(); this_cluster++) {
    for (std::vector<POINT>::iterator other_cluster=points->begin(); other_cluster<points->end(); other_cluster++) {
      if ((*this_cluster).cluster->ref == (*other_cluster).cluster->ref) {
	other_point_count++;
	continue;
      }
      if(con_val[this_point_count][other_point_count] >= lowerbound && con_val[this_point_count][other_point_count] <= upperbound){
	double * this_center = getNewDistant(&(*this_cluster));
	double * other_center = getNewDistant(&(*other_cluster));

	Ctrl[0][0] = this_center[0];
	Ctrl[0][1] = this_center[1];
	Ctrl[0][2] = this_center[2];

	Ctrl[3][0] = other_center[0];
	Ctrl[3][1] = other_center[1];
	Ctrl[3][2] = other_center[2];
      
	Ctrl[1][0] = centers[(*this_cluster).cluster->ref][0];
	Ctrl[1][1] = centers[(*this_cluster).cluster->ref][1];
	Ctrl[1][2] = centers[(*this_cluster).cluster->ref][2];
	Ctrl[2][0] = centers[(*other_cluster).cluster->ref][0];
	Ctrl[2][1] = centers[(*other_cluster).cluster->ref][1];
	Ctrl[2][2] = centers[(*other_cluster).cluster->ref][2];

	if (toggle_color == 1){
	  draw_curve(Ctrl,(&(*this_cluster)),(&(*other_cluster)));
	}
	free(this_center);
	free(other_center);
      }
      other_point_count++;
    }
    other_point_count = 0;
    this_point_count++;
  }//Compute the control points
  
  if (draw_center == 1){
      
    for(int this_center = 0; this_center<clusters; this_center++){
      for(int next_center = this_center+1; next_center<clusters; next_center++){
	glColor3f(1.0, 1.0, 1.0);
	glBegin(GL_LINES);
	glVertex3f(centers[this_center][0]/SCALE,centers[this_center][1]/SCALE, centers[this_center][2]/SCALE);
	glVertex3f(centers[next_center][0]/SCALE,centers[next_center][1]/SCALE, centers[next_center][2]/SCALE);
	glVertex3f(15, 0, 0);
	glEnd();
      }
    }
  
    for(std::vector<POINT>::iterator this_point=points->begin(); this_point<points->end(); this_point++){
      glColor3f(1.0, 1.0, 1.0);
      glBegin(GL_LINES);
      glVertex3f((*this_point).coor[0]/SCALE,(*this_point).coor[1]/SCALE,(*this_point).coor[2]/SCALE);
      glVertex3f(centers[(*this_point).cluster->ref][0]/SCALE,centers[(*this_point).cluster->ref][1]/SCALE,centers[(*this_point).cluster->ref][2]/SCALE);
      glEnd();
    }
  }
  glPopMatrix();
   
  glutSwapBuffers();
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
  case 'c':
    if (draw_center == -1)
      draw_center = 1;
    else
      draw_center = 0;
    break;
  case 'v':
    if (toggle_color == 1)
      toggle_color = 0;
    else
      toggle_color = 1;
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
  if (control == 100){
    free(cluster_point->points);
    free(cluster_point->clusters);
    free(cluster_point);
    cluster_point= load_points(filename, clusters);
    points = cluster_point->points;
    cluster_ref = cluster_point->clusters;
    assignColorToCluster();
  }
  else if (control == 400)
    lower_spinner->set_float_limits(0.0,upperbound,GLUI_LIMIT_CLAMP);
  else if (control == 500)
    upper_spinner->set_float_limits(lowerbound,1.0,GLUI_LIMIT_CLAMP);
  glutPostRedisplay();
}

void assignColorToCluster(){
  vector<CLUSTER> * clust = cluster_point->clusters;
  for (int i = 0; i <clusters;i++){
    double *color = new double[3];
    color[0] = (360/clusters) * i;
    color[1] = 1;
    color[2] = 2;
    (*clust)[i].color = color;
  }
}

int main(int argc, char *argv[]){
  //Make sure that there is a command-line argument
  if (argc < 3) {
    std::cout << "usage: FinalProject <datafile> <patient matrix>" << std::endl;
    exit(-1);
  }
  else {
    //The first command-line arg is the name of the file to use
    filename = argv[1];
    matrix_file = argv[2];
    //Initial number of cluser
    clusters = 2; 
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
  scale_unit->set_int_limits(0,15,GLUI_LIMIT_CLAMP);
  scale_unit->set_speed(0);

  upper_spinner = new GLUI_Spinner(control,"Upper bound strength", GLUI_SPINNER_FLOAT,&upperbound,400,control_callback);
  upper_spinner->set_float_limits(lowerbound,1.0,GLUI_LIMIT_CLAMP);
  upper_spinner->set_speed(2);
  
  lower_spinner = new GLUI_Spinner(control,"Lower bound strength", GLUI_SPINNER_FLOAT,&lowerbound,500,control_callback);
  lower_spinner->set_float_limits(0.0,upperbound,GLUI_LIMIT_CLAMP);
  lower_spinner->set_speed(2);
  
  //Adding checkboxs for toggling colors
  glui->add_checkbox_to_panel(control,"Bundle Color",&toggle_color,600,control_callback);
  glui->add_checkbox_to_panel(control,"Cluster center",&draw_center,700,control_callback);

  ///////////////////////////////////////////////////////////
  glutMainLoop();
  return 0;
}

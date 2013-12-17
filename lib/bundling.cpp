//
//  bundling.cpp
//  
//
//  Created by Dustin Filippi on 12/13/13.
//
//

#include "bundling.hpp"
#include <stdio.h>
#include <math.h>
using namespace std;


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

//This is the Bezier matrix for computing bezier curves and surfaces
GLfloat BezMatrix[4][4] = {
    { -1.0,  3.0, -3.0, 1.0},
    {  3.0, -6.0,  3.0, 0.0},
    { -3.0,  3.0,  0.0, 0.0},
    {  1.0,  0.0,  0.0, 0.0}};


///Draw a bezier curve.
void draw_curve(GLfloat CtrlPoints[4][3],POINT *p1, POINT *p2, float con_val, int tensor) {
    //Local Variables
    float u;
    GLfloat xx, yy, zz;
    GLfloat uu[4];
    GLfloat BP[4][3];
    int i, j,k;
    u = 0.0;
    
    for(i=0; i<4; i++)
        for(j=0; j<3; j++) {
            BP[i][j] = 0.0;
            for(k=0; k < 4; k++) {
                BP[i][j] += BezMatrix[i][k] * CtrlPoints[k][j];
            }
        }
    i = 0;
    j = 0;
    
    glBegin(GL_LINES);
    //glPointSize(1.25/40);
    double diff = ((p2->cluster->color)[0] - (p1->cluster->color)[0])/100;
    double accumulate = (p1->cluster->color)[0];
    for(u=0.0; u<=1.0; u+=0.01) {//Make 100 points along curve
        // [1, u, u^2, u^3]
        uu[3] = 1.;
        uu[2] = u;
        uu[1] = uu[2]*u;
        uu[0] = uu[1]*u;
        
        //Compute the X coordinate for this distance along the curve
        xx = 0;
        for(i=0; i < 4; i++)
            xx += uu[i] * BP[i][0];
        
        //Compute the Y coordinate for this distance along the curve
        yy = 0;
        for(i=0; i < 4; i++)
            yy += uu[i] * BP[i][1];
        
        //Compute the Y coordinate for this distance along the curve
        zz = 0;
        for(i=0; i < 4; i++)
	  zz += uu[i] * BP[i][2];
	double *rgb;
	if(tensor){
	  rgb = getRGB(120*con_val,1,1);
	}
	else{
	  rgb = getRGB(accumulate,1,1);
	}
	accumulate+=diff;
	if (accumulate > 360){
	  accumulate-=360;
	}
	else if (accumulate < 0)
	  accumulate+=360;
	glColor3f(rgb[0],rgb[1],rgb[2]);
	free(rgb);
        glVertex3f(xx/40.0, yy/40.0, zz/40.0);
	
    }
    glEnd();
}

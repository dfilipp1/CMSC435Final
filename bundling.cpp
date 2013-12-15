//
//  bundling.cpp
//  
//
//  Created by Dustin Filippi on 12/13/13.
//
//

#include "bundling.hpp"

//This is the Bezier matrix for computing bezier curves and surfaces
GLfloat BezMatrix[4][4] = {
    { -1.0,  3.0, -3.0, 1.0},
    {  3.0, -6.0,  3.0, 0.0},
    { -3.0,  3.0,  0.0, 0.0},
    {  1.0,  0.0,  0.0, 0.0}};


///Draw a bezier curve.
void draw_curve(GLfloat CtrlPoints[4][3]) {
    //Local Variables
    float u;
    GLfloat xx, yy, zz;
    GLfloat uu[4];
    GLfloat BP[4][3];
    int i, j,k;
    u = 0.0;
    
    for(i=0; i<4; i++)
        for(j=0; j<2; j++) {
            BP[i][j] = 0.0;
            for(k=0; k < 4; k++) {
                BP[i][j] += BezMatrix[i][k] * CtrlPoints[k][j];
            }
        }
    i = 0;
    j = 0;
    
    glBegin(GL_POINTS);
    glPointSize(1.25/40);
    glColor3f(1.0, 1.0, 0.0);
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
        
        //Draw it
        glVertex3f(xx/40.0, yy/40.0, zz/40.0);
    }
    glEnd();
}

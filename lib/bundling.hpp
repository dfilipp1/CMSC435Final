//
//  bundling.hpp
//  
//
//  Created by Dustin Filippi on 12/13/13.
//
//

#ifndef ____bundling__
#define ____bundling__

#ifdef __APPLE__
    #include <GLUT/glut.h>
#else
    #include <GL/glut.h>
#endif

#include <iostream>
#include "CLUSTER_STRUCT.hpp"

void draw_curve(GLfloat BP[4][3],POINT *p1, POINT *p2, float con_val, int tensor);

#endif /* defined(____bundling__) */

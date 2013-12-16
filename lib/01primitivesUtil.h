
/* 
 * Utitility routines, 
 * @@ Jian Chen (jichen@umbc.edu)
 * @@ August 2013
 */

#ifndef __01_PRIMTITIVES_UTIL_H__
#define __01_PRIMTITIVES_UTIL_H__


/*-------------------------------------------------------------
 * Public  variables
 */
/* interaction */
extern GLfloat  eyeZPos, eyeXRot, eyeYRot;

/* Geometry */
// on/off (1/0) state for drawing the coordinates (x, y, z): 
// default: 1
extern int draw_coord;
extern GLuint  coordList;

// on/off (1/0) state for drawing the ground plane 
// default: 1
extern int draw_ground;  
extern GLuint  groundList;

/*-------------------------------------------------------------
 * Functions 
 */
/* view transformation function */
//void uViewInit(GLfloat eyeZPos, GLfloat eyeXRot, GLfloat eyeYRot);
//void uUpdateViewPoint(GLfloat newEyeZ, GLfloat newEyeXRot, GLfloat newEyeYRot);


/* create a display list  */
void uMakeCoordList(GLuint coordList);
void uMakeGroundList(GLuint groundList);

#endif

//Author Sothiara Em
#ifndef _CLUSTER_OBJ_H
#define _CLUSTER_OBJ_H

#include <vector>

using namespace std;

/*Struct to define a cluster group*/
typedef struct cluster{
  //Reference number to identify the cluster
  int ref;
  //Double arry to define the x and y of the center
  double *center;
  //For debugging, defining the color for the cluster
  double *color;
}CLUSTER;

vector<string> &split(const string &s, char delim, vector<string> &elems);

/*Struct to define a point that belong to a cluster*/
typedef struct point{
  //Coordinate of the point
  double *coor;
  //Reference to the cluster the points belong in
  CLUSTER *cluster;
}POINT;

typedef struct cluster_point{
  vector<CLUSTER> * clusters;
  vector<POINT> * points;
}CLUSTER_POINT;

/*
 *Takes in a file name and parse the file
 *Cluster the points and return the arrays
 *of point
 */
CLUSTER_POINT *load_points(char * file_name,int clust);


#endif

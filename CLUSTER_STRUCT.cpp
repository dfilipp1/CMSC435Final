/* Author: sothara em
 * Class to handle the clustering of points
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <sstream>
#include <fstream>
#include <string.h>
#include <math.h>
#include "CLUSTER_STRUCT.hpp"

using namespace std;

//Method header
//check distant for center points to each other
int check_distant(double * coordinates, vector<POINT> *points);
void k_mean_clustering(vector<POINT> * points, int cluster_nums);

//Keeping track of the bound for each coordinate components
double max_x = 0;
double max_y = 0;
double max_z = 0;

double min_x = 0;
double min_y = 0;
double min_z = 0;

//Constant of the min distant between initial cluster point
double MIN_INITIAL_CLUSTER_DISTANCE = 7;

/*
 * Split the string by a define delimiter and the
 * store it inside a vector
 */
vector<string> &split(const string &s, char delim, vector<string> &elems) {
  stringstream ss(s);
  string item;
  while (std::getline(ss, item, delim)) {
    elems.push_back(item);
  }
  return elems;
}

/*
 * Pass in the file path and then parse it
 * store in points in vector
 * do k-mean operation on those points
 * takes in the number of clusters we want
 */
vector<POINT> *load_points( char * file_name, int cluster_nums){
  
  vector<POINT> * points = new vector<POINT>();

  //Opening the file
  ifstream file;
  file.open(file_name);
  string line;
  
  //Iterates line by line
  while(getline(file,line)){
    //Vector to split the elems
    vector<string> elems;
    split(line,' ',elems);
    
    double coord[3] = {atof(elems[0].c_str()),atof(elems[1].c_str()),atof(elems[2].c_str())};

    //Keeping tracks on the min and max bound
    if (coord[0] > max_x) max_x = coord[0];
    if (coord[0] < min_x) min_x = coord[0];

    if (coord[1] > max_y) max_y = coord[1];
    if (coord[1] < min_y) min_y = coord[1];
    
    if (coord[2] > max_z) max_z = coord[2];
    if (coord[2] < min_z) min_z = coord[2];
    
    //Initialize a new point
    POINT * p = new POINT();
    p->coor = coord;
    points->push_back(*p);
  }

  k_mean_clustering(points, cluster_nums);
  return points;
}


/*
 * Clustering the points into groups
 */
void k_mean_clustering(vector<POINT> * points, int cluster_nums){
  
  vector<CLUSTER> * clusters = new vector<CLUSTER>();

  //Ininitializing the cluster points to random location on the graph
  for (int i = 0; i < cluster_nums; i++){

    CLUSTER *clus = new CLUSTER();
    clus->ref = i;
    
    double center[3];
    //While the cluster are certain distant apart
    do{
      //Getting the center by picking a number between 0 to (min + max)
      //To account for negative coordinates, we subtract by min
      double randX = fmod(rand(),fabs(max_x)+fabs(min_x))-fabs(min_x);
      if (max_x < 0) randX*=-1;
      double randY = fmod(rand(),fabs(max_y)+fabs(min_y))-fabs(min_y);
      if (max_y < 0) randY*=-1;
      double randZ = fmod(rand(),fabs(max_z)+fabs(min_z))-fabs(min_z);
      if (max_z < 0) randZ*=-1;
      
      center[0] = randX; center[1] = randY; center[2] = randZ;
    }while(check_distant(center, points) == 1);
    clus->center = center;
  }
}

/* 
 * Check to make sure the minimum distant between the mean
 * points is satisfies
 */
int check_distant(double * coordinates, vector<POINT> *points){
  int checked = 0;

  //Loop through all the current center and check the center distant
  for (int i = 0; i < points->size(); i++){
    double * ptDist = (&(*points)[i])->coor;
    
    //calculating the distant between the current center and the new center coordinates
    double distant = sqrt(pow(ptDist[0] - coordinates[0],2) + pow(ptDist[1] - coordinates[1],2) + pow(ptDist[2] - coordinates[2],2));
    if (distant <= MIN_INITIAL_CLUSTER_DISTANCE){
      checked = 1;
      break;
    }
  }
  //return 0 for good 1 for false;
  return checked;
}

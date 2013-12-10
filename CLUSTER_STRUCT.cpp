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
void recenter_mean(vector<POINT> * points,vector<CLUSTER> * clusters);
double get_distant(double*p1, double* p2);

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
  double * coord;
  
  int num_of_points = 0;

  //Iterates line by line
  while(getline(file,line)){
    num_of_points++;
    //Vector to split the elems
    vector<string> elems;
    split(line,' ',elems);
    
    coord = new double(3);
    coord[0] = atof(elems[0].c_str());
    coord[1] = atof(elems[1].c_str());
    coord[2] = atof(elems[2].c_str());

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
    p->cluster = NULL;
    points->push_back(*p);
  }

  file.close();
  
  int num_of_cluster = sqrt(num_of_points/2);

  k_mean_clustering(points, num_of_cluster);
  cout<<"The number of file is: "<<num_of_points<<endl;
  cout<<"The number of cluster is: "<<num_of_cluster<<endl;
  return points;
}


/*
 * Clustering the points into groups
 */
void k_mean_clustering(vector<POINT> * coordPoint, int cluster_nums){
  
  vector<POINT> *points = coordPoint;
  vector<CLUSTER> * clusters = new vector<CLUSTER>();

  //Ininitializing the cluster points to random location on the graph
  for (int i = 0; i < cluster_nums; i++){
    
    CLUSTER *clus = new CLUSTER();
    clus->ref = i;
    
    double * center;
    //While the cluster are certain distant apart
    do{
      center = new double(3);
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
    clusters->push_back(*clus);

  }
  int first_pass = 0;
  int debug = 1;
  //Iterate and begin clustering
  do{
    debug = 1;
    
    //Assign each point to a cluster group
    for (int i = 0;i < points->size(); i ++){
      int min_dist_idx = -1;
      double distant;
      
      //Iterate through all the cluster mean
      for (int j = 0;j < clusters->size(); j++){
	CLUSTER *declus = &(*clusters)[j];	
	double calcDistant = get_distant((*clusters)[j].center,(*points)[i].coor);
	if (min_dist_idx == -1){
	  min_dist_idx = j;
	  distant = calcDistant;
	}
	//Get the minimum distant
	else{
	  if (distant > calcDistant){distant = calcDistant; min_dist_idx = j;}}
      }

      if ((*points)[i].cluster){
	if ((*points)[i].cluster->ref != (*clusters)[min_dist_idx].ref)
	  debug = 0;
      }
      else
	debug = 0;
      (*points)[i].cluster = &(*clusters)[min_dist_idx];
    }//end of the big for loop
    
    if (debug == 0)
      recenter_mean(points,clusters);
  }while(debug == 0);

  /*
  for (int i = 0; i < points->size(); i++){
    cout<<"Points: "<<i<<" Ref:"<<(*points)[i].cluster->ref<<endl;
    }*/
}

/*
 * Get the distant between two 3D points
 */
double get_distant(double*p1, double* p2){
  return sqrt(pow(p1[0] - p2[0],2) + pow(p1[1] - p2[1],2) + pow(p1[2] - p2[2],2));
}

/*
 * Check the mean to see if the centoid need to be recomputed
 */
void recenter_mean(vector<POINT> * points,vector<CLUSTER> * clusters){
  //Initializing the clusters new center
  vector<double*> clusterCenter;
  double * coord;
  for (int i = 0; i < clusters->size(); i ++){
    //[cum_x,cum_y,cum_z,total_added]
    coord =(double*) malloc(4*sizeof(double));
    coord[0]=0;coord[1]=0;coord[2]=0;coord[3]=0;
    clusterCenter.push_back(coord);
  }
  
  //Summing all the x y z of the points belonging to a cluster
  for (int i = 0; i < points->size(); i ++){
    CLUSTER * cluster = (*points)[i].cluster;
    //Coordinate of the points 
    double *coord = (*points)[i].coor; 
    //Get the index of the reference cluster
    double *clsCtr = clusterCenter[cluster->ref];
    //cout<<"Ref: "<<cluster->ref<<endl;
    clsCtr[0]+=coord[0]; clsCtr[1]+=coord[1]; clsCtr[2]+=coord[2]; clsCtr[3]+=1; 
  }

  //Getting the average and also setting the new center
  for (int i = 0; i < clusters->size(); i++){
    ((*clusters)[i].center)[0] = (clusterCenter[i])[0] / (clusterCenter[i])[3];
    ((*clusters)[i].center)[1] = (clusterCenter[i])[1] / (clusterCenter[i])[3];
    ((*clusters)[i].center)[2] = (clusterCenter[i])[2] / (clusterCenter[i])[3];
    //cout<<"New cntr "<<i<<" X:"<<((*clusters)[i].center)[0]<<" Y:"<<((*clusters)[i].center)[1]<<" Z"<<((*clusters)[i].center)[2]<<endl;
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

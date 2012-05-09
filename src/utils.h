#ifndef UTILS_H_
#define UTILS_H_

#include <fstream>
#include <string>
#include <math.h>
#include <sys/types.h>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <dirent.h>
#include <errno.h>
#include <cassert> 
#include <iostream> 
/*
 * calculate de eclidian distance between vec_a and vec_b.
 * THEY MUST HAVE THE SAME SIZE (dim)
 */
double EuclideanDistance(const double *vec_a,const double *vec_b,int dim);
double EuclideanDistance(std::vector<double> vec_a,std::vector<double> vec_b);
/*
 * Calculate the log in any basis.
 */
double u_log(double value,double base);
void file_counter(std::string file_name,unsigned &lines,unsigned &columns);
std::vector<std::string> ls(std::string dir);
bool mkdir(std::string path);
bool rm(std::string file);

  
#endif /* UTILS_H_ */

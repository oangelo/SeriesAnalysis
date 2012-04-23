/*
 * utilities.h
 *
 *  Created on: Feb 23, 2010
 *  Author: gralha and angelo
 *
 *  Description: basic utilities for the time series lib. This modules includes:
 *   --> General math functions;
 *   --> the exception module, for a better encapsulation.
 *
 */

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
#include"lib_exceptions.h" //lib exceptions

/*useful macro for double comparasions, i e, (a<b+EPSILON)*/
#define  EPSILON  0.000001

/*
 * calculate de eclidian distance between vec_a and vec_b.
 * THEY MUST HAVE THE SAME SIZE (dim)
 */
double __dist_eucl(const double *vec_a,const double *vec_b,int dim);

/*
 * Calculate the log in any basis.
 */
double u_log(double value,double base);
void file_counter(std::string file_name,unsigned &lines,unsigned &columns);
std::vector<std::string> ls(std::string dir);
bool mkdir(std::string path);
#endif /* UTILS_H_ */


class ne_pairs{
private:
  std::vector< std::vector<unsigned> > pairs;
public:
  void push_back(unsigned i,unsigned j);
  void push_back(ne_pairs  foo);
  void push_back(std::vector<unsigned> aux);
  bool exist(std::vector<unsigned> aux);
  bool exist(unsigned* aux);
  bool exist(unsigned i, unsigned j);
  std::vector<unsigned> get(unsigned i);
  std::vector<unsigned> take();
  unsigned size();
};
  


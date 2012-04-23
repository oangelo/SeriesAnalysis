/*
 * Attractor.h
 *
 *  Created on: Dec 23, 2009
 *      Author: gralha
 *      Description: Modulus with tools to analyze a time series as
 *      a chaotic data.
 *      reference: [1] Abarbanel et al, Rev Mod Phys, 65(4) 1331 (1993)
 */

#ifndef ATTRACTOR_H_
#define ATTRACTOR_H_

#include "time_series.h"
#include <stdio.h>
#include <vector>
#include <utility>

/*
 * Describe a n-dimensional attractor.
 */
class Attractor{

  friend void file_counter(std::string file_name,unsigned &lines,unsigned &columns);
public:
  
  //creates an attractor from a time series
  Attractor(time_series &ts,unsigned int D_e,unsigned int tau);
  // organized as data[array_index][array_coord]
  Attractor(double **data, unsigned int D_e,unsigned int n_vec);
  Attractor(std::string file_name);
  Attractor( Attractor& att);
  ~Attractor();
  std::vector<unsigned int> Box_counting_D(double epsilon);
  double get(unsigned vec,unsigned dim);
  void get_vec(unsigned vec,double *p_vec);
  double correlation_integral(double length_fraction);
  double get_epsilon();
  unsigned size(){return(__n_vec);};
  unsigned get_dim(){return(__D_e);};
  unsigned get_tau(){return(__tau);};
  
private:

  //check if a point is inside a box between [coord-epsilon/2,coord+epsilon/2]
  int __points_in_box(double *coord,double epsilon);
  /*
   * min(max) is an output array of size D (attractor dimension).
   * Each array element is associated to a coordinate minimum(maximum).
   * Useful to boxcounting.
   */
  void __find(double *min,double *max);
  void __length_side();
  //do n, dynamic, loops by recursion
  void __recursive_for(unsigned dimension,
		       unsigned cont,
		       unsigned* cont_loop,
		       unsigned *walker,
		       unsigned* max_loop,
		       double *min);
  double **__data; // organized as data[array_index][array_coord]
  unsigned __n_vec,__D_e,__tau;
  double __epsilon;
  double __length;
  unsigned int __constructor_flag;
  std::vector<unsigned int> n_box;
};





/*
 * Before reconstruct an attractor we use this functions to find
 * embedding dimension.
 *
 * parameters:
 ******* INPUT ********
 * tau: time delay, normally obtained by the first minimum mutual information
 * d_max: max embedding dimension.
 * Rt: threshold size required to decide if the neighbor is false.
 * SECOND_COND: boolean flag. If true it uses the second condition (If the
 * distance from the nearest neighbor is too far, it is a automatic flaged as
 * false neighbor). Defined at [1] page 1349.
 ******* OUTPUT *******
 * fnn_list: array of size D. Each index is related to a dimension between
 * [0,D], where D= i+1. Each array component have the number of false
 * neighbors for that dimension.
 */
unsigned int false_nearest_nei(time_series& ts,
			       unsigned tau,
			       unsigned int d_max,
			       double Rt,
			       double *fnn_list,
			       bool SECOND_COND);

/*
 * Create a set of lagged array from a time series ts.
 *
 * parameters:
 * ts: imput time series.
 * tau: time delay, normally obtained by the first minimum mutual information
 * dim: dimension
 */
double **create_lagged_array(time_series &ts,
			     const unsigned int tau,
			     const unsigned int dim);

/*
 * Delete the data dynamic allocated multiarray.
 */
void del_lagged_array(double** data,
		      const unsigned int n_vec,
		      const unsigned int dim);
/*
 * returns the index k of the nearest data[k] from data[index]. If we have a tie,
 *returns returns the vector with the higher index.
 */
//$$$$$$$$$$$$$$$$$$$$$$$$$$$warning%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//Memory leak on NN algorithm, need to put it in terms of attractors objects!
unsigned int __find_nearest(Attractor &data,unsigned index);


class recurrence_plot{
  friend class ne_pairs;
public:
  
  recurrence_plot(Attractor &ts_data,unsigned m_points=0,double dist_limit=0);
  recurrence_plot(unsigned** data,unsigned size);
  void generate(double limit);
  ~recurrence_plot();
  
  unsigned size();
  unsigned get(unsigned i, unsigned j);
  
  double RR();
  double DET();
  double L();
  
  unsigned points_in_diagonals();
  ne_pairs burn(unsigned i,unsigned j);
  unsigned diagonal_size(ne_pairs & cluster);
  unsigned diagonals(std::vector<unsigned> & length);
  
private:
  
  unsigned **__data;
  Attractor *__ts_data;
  unsigned __size;
};


#endif /* ATTRACTOR_H_ */

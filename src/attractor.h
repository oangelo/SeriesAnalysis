#ifndef CHAOS_H
#define CHAOS_H

#include "float.h"

#include <stdio.h>
#include <vector>
#include <utility>

#include "time_series.h"
#include "utils.h"
//TODO: 
//strip the correlation integrals and box count from the class

/*
 * Describe a n-dimensional attractor.
 */
class Attractor{

    friend void file_counter(std::string file_name,unsigned &lines,unsigned &columns);

    public:

    //creates an attractor from a time series
    Attractor(const TimeSeries &ts,const unsigned int dimension,const unsigned int delay);
    // organized as data[vector number][vector element]
    Attractor(const double** p_data,const  unsigned int dimension,const unsigned int n_vec);
    Attractor(const std::vector< std::vector<double> > & vec_data);
    Attractor(const std::string file_name);
    Attractor(const  Attractor & att);
    ~Attractor();
    Attractor& operator = (const Attractor &rhs);

    const double get_data(const unsigned vec,const unsigned dim) const;
    const void get_vec(const unsigned vec,double *p_vec) const;
    const std::vector<double>  get_vec(const unsigned vec) const;
    const unsigned get_dimension() const;
    const unsigned get_delay() const;

    const unsigned size() const;
    
    const std::vector<double>  operator[](const unsigned &vec) const;

    private:

    void Allocate(unsigned n_lines,unsigned n_columns);
    void Dealocate(unsigned n_lines,unsigned n_columns);
    unsigned dimension,delay,n_vec;
    double **data; // organized as data[array_index][array_coord]
};
/*
    double CorrelationIntegral(double length_fraction);
    std::vector<unsigned int> BoxCountingDimension(double epsilon);
    double epsilon;
    double length;
    std::vector<unsigned int> n_box;
    //check if a point is inside a box between [coord-epsilon/2,coord+epsilon/2]
    int PointsInBox(double *coord,double epsilon);
    
    //  min(max) is an output array of size D (attractor dimension).
    //  Each array element is associated to a coordinate minimum(maximum).
    //  Useful to boxcounting.
     
    void Find(double *min,double *max);
    void LengthSide();
    //do n, dynamic, loops by recursion
    void RecursiveFor(unsigned dimension,
            unsigned cont,
            unsigned* cont_loop,
            unsigned *walker,
            unsigned* max_loop,
            double *min);
 */



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
unsigned int false_nearest_nei(TimeSeries& ts,
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
void create_lagged_array(const TimeSeries &ts,
        const unsigned int tau,
        const unsigned int dim,
        double** data);

/*
 * returns the index k of the nearest data[k] from data[index]. If we have a tie,
 *returns returns the vector with the higher index.
 */
std::vector<unsigned> FalseNearestNeighbors(const TimeSeries& ts,
        unsigned delay,
        unsigned dimension_max,
        double Rt,
        bool SECOND_COND);

//A function that calculates the mean distance of orbits on the attractor.
//fist, find the mean distance betewn sequential points.
//then, chose a point, and find the point that is close to the chosem point,
//and check if it is not an neighboard on the array.
//This can be done by dividing the distance betwen points by the mean,
//and assures that the distance on the array is bigger than this value
void MeanOrbitDistance(Attractor & attractor,double &  mean, double &  std);



#endif /*CHAOS_H*/

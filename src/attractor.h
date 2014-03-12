#ifndef ATTRACTOR_H 
#define ATTRACTOR_H

#include "float.h"

#include <stdio.h>
#include <vector>
#include <utility>

#include "time_series.h"
#include "utils.h"
#include "statistics/src/statistics.h"
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

    double get_data(const unsigned vec,const unsigned dim) const;
    const std::vector<double>  get_vec(const unsigned vec) const;
    unsigned get_dimension() const;
    unsigned get_delay() const;

    unsigned size() const;
    
    const std::vector<double>  operator[](const unsigned &vec) const;

    private:

    unsigned dimension,delay,n_vec;
    std::vector<std::vector<double>> data; // organized as data[array_index][array_coord]
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
void CreateLaggedArray(const TimeSeries &ts,
        const unsigned int tau,
        std::vector<std::vector<double>>& data);

/*
 * returns the index k of the nearest data[k] from data[index]. If we have a tie,
 *returns returns the vector with the higher index.
 */
std::vector<unsigned> FalseNearestNeighbors(const TimeSeries& ts, unsigned delay,
        unsigned max_dimension, double threshold = 40);

//A function that calculates the mean distance of orbits on the attractor.
//fist, find the mean distance betewn sequential points.
//then, chose a point, and find the point that is close to the chosem point,
//and check if it is not an neighboard on the array.
//This can be done by dividing the distance betwen points by the mean,
//and assures that the distance on the array is bigger than this value
void MeanOrbitDistance(Attractor & attractor,double &  mean, double &  std);
double MeanPointsDistances(Attractor & attractor);
double StdPointsDistances(Attractor & attractor);



#endif /*ATTRACTOR_H*/

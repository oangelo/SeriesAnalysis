/*
 *  time_series.h
 *
 *  Created on: Feb 23, 2010
 *  Author: gralha and angelo
 *
 *  Description: This module encapsulate the time series data.
 *
 */


#ifndef _TIME_SERIES_H
#define	_TIME_SERIES_H
#include <vector>
#include <math.h>
#include <algorithm>
#include <string>
#include <fstream>
#include"lib_exceptions.h"
#include "float.h"
#include "utils.h"

/*
 *  Time series class to work as data structure for nonlinear, statistical and
 *  fractal analysis.
 */
class time_series
{

public:

	/*the memory pointed by data is copiedto an private c-array _data.*/
    time_series(double const *data, size_t size);
    time_series(std::string file_name);
    virtual ~time_series();
    /*
     * access operator
     */
    double& operator[](const int &aux);
    double mean();
    /*
     * data variance
     */
    double var();
    /*
     * data standard deviation
     */
    double std();
    double max();
    double min();
    unsigned int size();
    void print(std::string file_name);

private:
    std::vector<double> _data;

};


#endif	/* _TIME_SERIES_H */

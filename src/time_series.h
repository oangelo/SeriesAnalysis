#ifndef _TIME_SERIES_H
#define	_TIME_SERIES_H

#include <math.h>
#include <float.h>

#include <vector>
#include <algorithm>
#include <string>
#include <fstream>
#include <iostream>
#include <cassert>
#include <exception>

#include "statistics.h"
#include "utils.h"

//TODO Print a header on the file with the séries information, and put every thing inside a name space SERIES.

/*!  
 *  Time series class to work as data structure for nonlinear, statistical and
 *  fractal analysis.
 *  The class is all costant, since the data should not change.
 */
class TimeSeries
{

public:

	/*the memory pointed by data is copied to an vector named data.*/
    TimeSeries(double const * const data, size_t size);
    TimeSeries(const std::vector<double> & data);
    TimeSeries(const std::vector<std::vector<double>> & many_data, unsigned column = 0);
    virtual ~TimeSeries();
    //  access operator
    const double& operator[](const int &aux) const;
    double Mean() const;
     //data variance
    double Var() const;
    // data standard deviation
    double Std() const;
    double Max() const;
    double Min() const;
    unsigned int size() const;
    void Print(std::string file_name) const;

private:
    std::vector<double> data;

};

double Entropy(TimeSeries& ts);
double AutoCorrelation(TimeSeries& ts,unsigned tau);
double CrossCorrelation(TimeSeries& ts1,TimeSeries& ts2);
double MutualInformation(TimeSeries& ts,unsigned  tau, unsigned bins);

#endif	/* _TIME_SERIES_H */

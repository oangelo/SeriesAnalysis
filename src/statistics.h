/* 
 * File:   Histogram.h
 * Author: angelo
 *
 * Created on September 7, 2009, 6:50 PM
 */

#ifndef _STATISTICS_H
#define	_STATISTICS_H

#include "time_series.h"
#include <gsl/gsl_histogram.h>
#include <stdlib.h>
#include <algorithm>



class Histogram {
public:
	/*!Constructor:
	 * n: number of bins
	 * xmin: minimum value
	 * xmax: max value*/
    Histogram(int n,double xmin, double xmax);

	/*!Constructor:
	 * using Scott's choice to binning, see:
	 * DAVID W. SCOTT Biometrika 1979 66(3):605-610;
	 * std: data std
	 * size: number of points
	 * xmin: minimum value
	 * xmax: max value*/

    Histogram(double std, size_t size, double xmin,double xmax);

    Histogram(Histogram& orig);
    virtual ~Histogram();

    /*! Copy histogram to another hostogram */
    double  operator=(const double & aux);
    /*! Returns the probability of a bin */
    double  operator[](const int &aux);
    /*! return the probability of a value */
    double  operator[](const double &aux);
    /*! return the bin of a value */
    int operator()(const double &aux);
    /*! increment the bin on the range of x*/
    void increment(double x);
    /*! return how many times that bin was incremented*/
    double get(const size_t & i);
    /*! set all bins to 0.*/
    void reset(void);
    /*! Returns how many bins we have.*/
    size_t n_bins(void);
    /*! Returns the max value one can input.
     * (The right value of the highest bin) */
    double range_max(void);
    /*! Returns the min value one can input.
    * (The left value of the lowest bin) */
    double range_min(void);
    /**********************
    *Histogram Statistics**
    ***********************
    */
    /*
     *!This function returns the FIRST maximum value found in the contained
     * in the histogram bins.
     */
    double max_val(void);
     /*
     * !This function returns the index of the bin containing the maximum value.
     * In the case where several bins contain the same maximum value the
     * smallest index is returned.
     */
    size_t max_bin(void);
     /*
     * !This function returns the minimum value contained in the histogram bins.
     */
    double min_val(void);
     /*
     * This function returns the index of the bin containing the minimum value.
     * In the case where several bins contain the same maximum value the smallest
     * index is returned.
     */
    size_t min_bin(void);
     /*
     * !This function returns the mean of the histogrammed variable,
     * where the histogram is regarded as a probability distribution.
     * Negative bin values are ignored for the purposes of this calculation.
     * The accuracy of the result is limited by the bin width.
     */
    double mean(void);
     /*
     * This function returns the sum of all bin values. Negative bin values are
     * included in the sum.
     */
    
    double sum(void);
     /*
     * !This function returns the standard deviation of the histogrammed variable,
     * where the histogram is regarded as a probability distribution.
     * Negative bin values are ignored for the purposes of this calculation.
     * The accuracy of the result is limited by the bin width.
     */
    double sigma(void);



















    /*
    *  TO BE INPLEMENTED
     * ***********************
     * Histogram Operations
     * ***********************

     *This function multiplies the contents of the bins of histogram h by the constant scale
     *i.e. h'_1(i) = h_1(i) * scale.

    int scale(double scale);

     *This function shifts the contents of the bins of histogram h by the constant offset,
     *i.e. h'_1(i) = h_1(i) + offset.
     int shift(double offset);
     *
     *
     *
     *
    —: int gsl_histogram_equal_bins_p (const gsl_histogram * h1, const gsl_histogram * h2)
    —: int gsl_histogram_add (gsl_histogram * h1, const gsl_histogram * h2)
    —: int gsl_histogram_sub (gsl_histogram * h1, const gsl_histogram * h2)
    —: int gsl_histogram_mul (gsl_histogram * h1, const gsl_histogram * h2)
    —: int gsl_histogram_div (gsl_histogram * h1, const gsl_histogram * h2)
    —: int gsl_histogram_fprintf (FILE * stream, const gsl_histogram * h, const char * range_format, const char * bin_format)
    —: int gsl_histogram_fscanf (FILE * stream, gsl_histogram * h)

    */
private:
    gsl_histogram * h;
    double _sum_value;
    
};

double entropy(time_series& ts);
double auto_corr_func(time_series& ts,unsigned tau);
double mutual_information(time_series& ts,unsigned  tau);

#endif	/* _STATISTICS_H */


#ifndef _STATISTICS_H
#define	_STATISTICS_H

#include <stdlib.h>

#include <algorithm>
#include <exception>
#include <numeric>
#include <iostream>

#include <gsl/gsl_histogram.h>

#define EPSILON 0.000000000001


class Histogram1D{
    public:

        Histogram1D(const double min,const double max,const unsigned number_of_bins);
        //set values on the histogram
        size_t operator()(const double value);
        //get values from histogram
        unsigned operator[](const size_t index) const;
        double Mean() const;
        double Std() const;
        size_t Max() ;
        size_t Min() ; 
        unsigned Sum() const;
        std::pair<double,double> BinRange(size_t index);
        size_t size() const;

        class IndexErro : public std::exception {
            virtual const char* what() const throw(){return "Index is out of range!";}
        } index_erro;
        class ValueErro : public std::exception {
            virtual const char* what() const throw(){return "Value is wrong!";}
        } value_erro;

    private:
        size_t HashFunction(const double value) const;

        double max,min,bin_width;
        std::vector<unsigned> histogram;
};

#endif	/* _STATISTICS_H */


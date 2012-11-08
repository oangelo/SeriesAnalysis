#ifndef RECURRENCE_PLOT_H
#define RECURRENCE_PLOT_H

#include <set>
#include <map>

#include "chaos.h"

class RecurrencePlot{
    public:
        RecurrencePlot(const TimeSeries &time_series,double threshold);
        RecurrencePlot(const Attractor &attractor,double threshold);
        RecurrencePlot(unsigned** data,unsigned size);
        RecurrencePlot(std::vector<std::vector<unsigned>> data);
        void PrintOnScreen();

        unsigned size() const;
        std::vector<unsigned> & operator[](const unsigned &line);
        std::vector<std::vector<unsigned>> get_data() const;

        typedef std::vector<unsigned> rp_row;
        typedef std::vector<rp_row> rp_data ;

        enum{WHITE_DOT = 0, BLACK_DOT = 1};
        
    private:
        void Generate(double limit, const TimeSeries & time_series);
        void Generate(double limit, const Attractor &  attractor);
        void Allocate(size_t size);

        rp_data data;
};

#endif /*RECURRENCE_PLOT_H*/ 

#ifndef RECURRENCE_PLOT_H
#define RECURRENCE_PLOT_H

#include <set>
#include <map>

#include "chaos.h"

typedef std::set<std::pair<unsigned,unsigned>> PairsList;  

class RecurrencePlot{
    public:

    RecurrencePlot(const TimeSeries &time_series,double threshold,unsigned size=0);
    RecurrencePlot(const Attractor &attractor,double threshold,unsigned size=0);
    RecurrencePlot(unsigned** data,unsigned size);
    RecurrencePlot(const RecurrencePlot & rp);
    void Generate(double limit);
    ~RecurrencePlot();

    const unsigned Size() const;
    const unsigned get_data(unsigned i, unsigned j) const;
    
    RecurrencePlot & operator=(const RecurrencePlot & rp);
    unsigned const* operator[](const unsigned &line) const;

    double RR();
    double DET();
    double L();

    const PairsList Burn(unsigned i,unsigned j) const;
    void Paint(unsigned i,unsigned j, unsigned color);

    private:
     
    void Generate(double limit, const TimeSeries & time_series);
    void Generate(double limit, const Attractor &  attractor);
    void Allocate(unsigned n_lines,unsigned n_columns);
    void Deallocate(unsigned n_lines,unsigned n_columns);
 
    unsigned size;
    unsigned** data;
};

unsigned diagonal_size(PairsList  cluster);

/*
    unsigned points_in_diagonals();
    unsigned diagonals(std::vector<unsigned> & length);
*/

#endif /*RECURRENCE_PLOT_H*/ 

#ifndef RECURRENCE_PLOT_H
#define RECURRENCE_PLOT_H

#include <set>
#include <map>

#include "chaos.h"

typedef std::set<std::pair<unsigned,unsigned> > PairsList;  

class RecurrencePlot{
    public:

    RecurrencePlot(const TimeSeries &time_series,double threshold,unsigned size=0);
    RecurrencePlot(const Attractor &attractor,double threshold,unsigned size=0);
    RecurrencePlot(unsigned** data,unsigned size);
    RecurrencePlot(const RecurrencePlot & rp);
    void Generate(double limit);
    ~RecurrencePlot();

    unsigned Size() const;
    unsigned get_data(unsigned i, unsigned j) const;
    
    RecurrencePlot & operator=(const RecurrencePlot & rp);
    unsigned const* operator[](const unsigned &line) const;

    double RR();
    double DET();
    double L();

    PairsList Burn(unsigned i,unsigned j) const;
    PairsList Paint(unsigned i,unsigned j, unsigned color);
    std::vector<unsigned> Diagonals();

    enum{WHITE_DOT = 0, BLACK_DOT = 1};
    private:
     
    void Generate(double limit, const TimeSeries & time_series);
    void Generate(double limit, const Attractor &  attractor);
    void Allocate(unsigned n_lines,unsigned n_columns);
    void Deallocate(unsigned n_lines,unsigned n_columns);
    unsigned CountBlackDots();
 
    unsigned size;
    unsigned** data;
    std::vector<unsigned> diagonals;
    unsigned n_black_dots;
};

unsigned DiagonalSize(PairsList  cluster);

/*
    unsigned diagonals(std::vector<unsigned> & length);
*/

#endif /*RECURRENCE_PLOT_H*/ 

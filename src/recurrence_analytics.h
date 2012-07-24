#ifndef RECURRENCE_ANALYTICS_H
#define RECURRENCE_ANALYTICS_H 

#include <set>
#include <map>
#include <vector>

#include "recurrence_plot.h"

typedef std::set<std::pair<unsigned,unsigned> > PairsList; 

unsigned NumberOfBlackDots(RecurrencePlot data);

PairsList Burn(RecurrencePlot data, unsigned i,unsigned j);
PairsList Paint(RecurrencePlot & data, unsigned i,unsigned j,unsigned color);

unsigned DiagonalSize(PairsList  cluster);
std::vector<unsigned> Diagonals(RecurrencePlot data);

unsigned VerticalSize(PairsList  cluster);
std::vector<unsigned> Verticals(RecurrencePlot data);

class RecurrenceAnalytics{
    public:
        RecurrenceAnalytics(RecurrencePlot data);
        double RR();
        double DET();
        double LAM();
        double RATIO();
        double L();
        double TT();
        double LMax();
        double VMax();
        double DIV();
        double ENTR();
        double TREND();
    private:
        std::vector<unsigned> verticals;
        std::vector<unsigned> diagonals;
        unsigned n_black_dots;
};

#endif /* RECURRENCE_ANALYTICS_H */

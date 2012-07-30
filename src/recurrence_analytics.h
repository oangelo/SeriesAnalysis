#ifndef RECURRENCE_ANALYTICS_H
#define RECURRENCE_ANALYTICS_H 

#include <set>
#include <map>
#include <vector>
#include <numeric>
#include <algorithm>

#include "recurrence_plot.h"
#include "statistics.h"

typedef std::set<std::pair<unsigned,unsigned> > PairsList; 

unsigned NumberOfBlackDots(RecurrencePlot data);

PairsList Burn(RecurrencePlot data, unsigned i,unsigned j);
PairsList Paint(RecurrencePlot & data, unsigned i,unsigned j,unsigned color);

unsigned DiagonalSize(PairsList  cluster);
std::vector<unsigned> Diagonals(RecurrencePlot data);
std::vector<int> DiagonalsDistances(RecurrencePlot data);

unsigned VerticalSize(PairsList  cluster);
std::vector<unsigned> Verticals(RecurrencePlot data);

class RecurrenceAnalytics{
    public:
        RecurrenceAnalytics(const RecurrencePlot & data);
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
        std::vector<int> diagonals_distances;
        unsigned n_black_dots;
        size_t size;
        unsigned points_in_diagonals;
        unsigned points_in_verticals;
};

#endif /* RECURRENCE_ANALYTICS_H */

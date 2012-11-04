#ifndef RECURRENCE_ANALYTICS_H
#define RECURRENCE_ANALYTICS_H 


#include "rp.h"
#include "statistics.h"
#include "burn.h"


unsigned NumberOfBlackDots(RecurrencePlot data);



unsigned DiagonalLength(PairsList  cluster);
unsigned DiagonalLenghtOrthogonal(PairsList  cluster);
//points that form diagonals lines
std::vector<unsigned> PointsInDiagonal(RecurrencePlot data);
std::vector<int> PointsInDiagonalDistances(RecurrencePlot data);

unsigned VerticalLength(PairsList  cluster);
unsigned HorizontalLength(PairsList  cluster);
//points that form vertical lines
std::vector<unsigned> PointsInVertical(RecurrencePlot data);

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

        unsigned NumberOfDiagonals();
        unsigned NumberOfVerticals();
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

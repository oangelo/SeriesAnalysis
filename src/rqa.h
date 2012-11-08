#ifndef RECURRENCE_ANALYTICS_H
#define RECURRENCE_ANALYTICS_H 

#include "rp.h"
#include "statistics.h"
#include "patterns/patterns.h"


unsigned NumberOfBlackDots(RecurrencePlot data);

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
        unsigned NumberOfHorizontals();
        unsigned NumberOfUnknown();
        double HitPercentage();
    private:
        unsigned n_black_dots;
        unsigned size;
        patterns::Patterns<RecurrencePlot> pattern;
};

#endif /* RECURRENCE_ANALYTICS_H */

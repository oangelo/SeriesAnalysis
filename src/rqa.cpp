#include "rqa.h"

unsigned NumberOfBlackDots(RecurrencePlot data){
    unsigned count = 0;
    for(unsigned j = 0; j < data.size(); j++)
        for(unsigned i = 0; i < data.size(); i++)
            if(data[i][j] == burn::BLACK_DOT )
                count++;
    return(count);
}





RecurrenceAnalytics::RecurrenceAnalytics(const RecurrencePlot & data)
:n_black_dots(NumberOfBlackDots(data)), size(data.size()), pattern(data, 2)
{
    std::cerr << " >> Unknown features: " << pattern.unknown_cluster.size() << std::endl;
}

double RecurrenceAnalytics::RR(){
    return(static_cast<double>(n_black_dots) / (size*size));
}

double RecurrenceAnalytics::DET(){
    unsigned sum(0);
    for(auto &iten: pattern.secondary_diagonal_cluster)
        sum += iten.size();
    return static_cast<double>(sum) / n_black_dots;
}

double RecurrenceAnalytics::LAM(){
    unsigned sum(0);
    for(auto &iten: pattern.vertical_cluster)
        sum += iten.size();
    return static_cast<double>(sum) / n_black_dots;
}

double RecurrenceAnalytics::RATIO(){
    return DET() / RR();
}

double RecurrenceAnalytics::L(){
    unsigned sum(0);
    for(auto &iten: pattern.secondary_diagonal_length)
        sum += iten;
    return static_cast<double>(sum) / pattern.secondary_diagonal_length.size();
}

double RecurrenceAnalytics::TT(){
    unsigned sum(0);
    for(auto &iten: pattern.vertical_length)
        sum += iten;
    return static_cast<double>(sum) / pattern.vertical_length.size();
}

double RecurrenceAnalytics::LMax(){
}
double RecurrenceAnalytics::VMax(){
}
double RecurrenceAnalytics::DIV(){
}
double RecurrenceAnalytics::ENTR(){
/*    Histogram1D histogram(1, LMax()+1,sqrt(diagonals.size()));
    for(auto item: diagonals)
        histogram(item);
    double entropy = 0, sum = histogram.Sum();
    for (size_t i = 0; i < histogram.size(); ++i){
       double p = histogram[i] / sum; 
       entropy += p * log(p); 
    }
    return entropy;
*/
}

    /* Needing implementation
double RecurrenceAnalytics::TREND(){
    unsigned n_til = 0, counter = 0;
    double RR = 0;
    for (size_t i = 0; i < diagonals_distances.size(); ++i)
        if( diagonals_distances[i] > 0){
            n_til += diagonals_distances[i];
            ++counter;
        }
    return 1;

}
        */
unsigned RecurrenceAnalytics::NumberOfDiagonals() {
    return pattern.secondary_diagonal_length.size();
}

unsigned RecurrenceAnalytics::NumberOfVerticals() {
    return pattern.vertical_length.size();
}

unsigned RecurrenceAnalytics::NumberOfHorizontals() {
    return pattern.horizontal_length.size();
}

unsigned RecurrenceAnalytics::NumberOfUnknown(){
    return pattern.unknown_cluster.size();
}

double RecurrenceAnalytics::HitPercentage(){
    return static_cast<double>(NumberOfVerticals() + NumberOfDiagonals()) / NumberOfUnknown();
}

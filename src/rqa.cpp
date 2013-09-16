#include "rqa.h"

unsigned NumberOfBlackDots(RecurrencePlot data){
    unsigned count = 0;
    for(unsigned j = 0; j < data.size(); j++)
        for(unsigned i = 0; i < data.size(); i++)
            if(data[i][j] == RecurrencePlot::BLACK_DOT )
                count++;
    return(count);
}

void RecurrenceAnalytics::RemoveLOI(RecurrencePlot& data){
    for (size_t i = 0; i < data.size(); ++i)
            data[i][i] = RecurrencePlot::WHITE_DOT;
}

RecurrenceAnalytics::RecurrenceAnalytics(RecurrencePlot data)
:n_black_dots(NumberOfBlackDots(data)), size(data.size()), vertical(), diagonal()
{
    RemoveLOI(data);
    vertical = FindVerticalLines(data); 
    diagonal = FindDiagonalLines(data);
}


std::vector<unsigned> FindVerticalLines(RecurrencePlot& data){
    std::vector<unsigned> vertical;
    bool flag = false;
    unsigned count = 0;
    for (size_t i = 1; i < data.size(); ++i) {
        count = 0;
        flag = false;
        for (size_t j = 0; j < data.size(); ++j) {
            if(data[j][i] == 1) {
                flag = true;
            }else{
                flag = false;
            }
            if(flag == true)
                ++count;
            if(flag == false and count >= 2) {
                vertical.push_back(count);
                count = 0;
            }
            if(flag == false and count < 2) {
                count = 0;
            }
            //end
        }
        if(count > 2)
            vertical.push_back(count);

    }
    return vertical;
}

std::vector<unsigned> FindDiagonalLines(RecurrencePlot& data){
    std::vector<unsigned> diagonal;
    bool flag = false;
    unsigned count = 0;
    for (size_t i = 1; i < data.size(); ++i) {
        count = 0;
        flag = false;
        for (size_t j = 0; j < data.size() - i; ++j) {
            if(data[j][j + i] == 1) {
                flag = true;
            }else{
                flag = false;
            }
            if(flag == true)
                ++count;
            if(flag == false) {
                if(count >= 2){
                    diagonal.push_back(count);
                    count = 0;
                }else{
                    count = 0;
                }
            }
        }
        //end of the loop
        if(count > 2)
            diagonal.push_back(count);
    }
    return diagonal;
}

double RecurrenceAnalytics::RR(){
    return(static_cast<double>(n_black_dots) / (size*size));
}

double RecurrenceAnalytics::DET(){
    unsigned sum(0);
    for(auto &iten: diagonal)
        sum += iten;
    return static_cast<double>(2 * sum) / n_black_dots;
}

double RecurrenceAnalytics::LAM(){
    unsigned sum(0);
    for(auto &iten: vertical)
        sum += iten;
    return static_cast<double>(sum) / n_black_dots;
}

double RecurrenceAnalytics::RATIO(){
    return DET() / RR();
}

double RecurrenceAnalytics::L(){
    unsigned sum(0);
    for(auto &iten: diagonal)
        sum += iten;
    if(diagonal.size() > 0)
        return static_cast<double>(sum) / diagonal.size();
    else
        return 0;
}

double RecurrenceAnalytics::TT(){
    unsigned sum(0);
    if(vertical.size() > 0){
        for(auto &iten: vertical)
            sum += iten;
        return static_cast<double>(sum) / vertical.size();
    }else{
        return 0; 
    }
}

unsigned RecurrenceAnalytics::LMax(){
    auto max(std::max_element(diagonal.begin(), diagonal.end()));
    auto end(diagonal.end());
    if(max != end)
        return *max; 
    else
        return(0);
}

unsigned RecurrenceAnalytics::VMax(){
    auto max(std::max_element(vertical.begin(), vertical.end()));
    auto end(vertical.end());
    if(max != end)
        return *max; 
    else
        return(0);
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
    return diagonal.size();
}

unsigned RecurrenceAnalytics::NumberOfVerticals() {
    return vertical.size();
}

unsigned RecurrenceAnalytics::NumberOfHorizontals() {
    return 0;
}

unsigned RecurrenceAnalytics::NumberOfUnknown(){
    return 0;
}

unsigned RecurrenceAnalytics::NumberOfRecurrence(){
    return n_black_dots;
}

double RecurrenceAnalytics::HitPercentage(){
    return static_cast<double>(NumberOfVerticals() + NumberOfDiagonals()) / NumberOfUnknown();
}

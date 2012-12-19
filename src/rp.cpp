#include "rp.h"

//######################################################################################
//#################################***Public Methods***#################################
//######################################################################################

RecurrencePlot::RecurrencePlot(const TimeSeries  &time_series, double threshold):data(){
    Allocate(time_series.size());
    Generate(threshold,time_series);
}

RecurrencePlot::RecurrencePlot(const Attractor &attractor, double threshold):data(){
    Allocate(attractor.size());
    Generate(threshold,attractor);
}

RecurrencePlot::RecurrencePlot(unsigned** data, unsigned size):data(){
    Allocate(size);
    for(unsigned int i=0;i<size;i++)
        for(unsigned int j=0;j<size;j++)
            this->data[i][j] = data[i][j];
}

RecurrencePlot::RecurrencePlot(std::vector<std::vector<unsigned>> data):data(){
    this->data = data;
}

unsigned RecurrencePlot::size() const{ 
    return(data.size());
}

std::vector<unsigned> & RecurrencePlot::operator[](const unsigned &line){
    return(data[line]);
}

std::vector<std::vector<unsigned>> RecurrencePlot::get_data() const{
    return(data);
}

void RecurrencePlot::PrintOnScreen(){
    for (size_t i = 0; i < data.size(); i++) {
        for (size_t j = 0; j < data[i].size(); ++j) {
                std::cout << data[i][j] << " "; 
        }
        std::cout << std::endl;
    }
}

//######################################################################################
//#################################***Private Methods***################################
//######################################################################################

void RecurrencePlot::Generate(double limit, const TimeSeries & time_series){
    for(unsigned j = 0; j < time_series.size(); j++)
        for(unsigned i = 0; i < time_series.size(); i++){
            double dist = fabs(time_series[i]-time_series[j]);  
            if(dist < limit){
                data[i][j] = BLACK_DOT;
            }else{
                data[i][j] = WHITE_DOT;
            }
        }
}

void RecurrencePlot::Generate(double limit, const Attractor & attractor){
    std::vector<double> vec_i(attractor.get_dimension()),vec_j(attractor.get_dimension());
    for(unsigned j = 0; j < attractor.size(); j++)
        for(unsigned i = 0; i < attractor.size(); i++){
            vec_i = attractor[i];
            vec_j = attractor[j];
            double dist = EuclideanDistance(&vec_i[0],&vec_j[0],attractor.get_dimension());
            if(dist < limit){
                data[i][j]= BLACK_DOT;
            }else{
                data[i][j]= WHITE_DOT;
            }
        }
}

inline
void RecurrencePlot::Allocate(size_t size){
    for (size_t i = 0; i < size; ++i)
    {
       data.push_back(rp_row(size, WHITE_DOT)); 
    }
}

double RR(RecurrencePlot rp) {
    double sum = 0, rr;
    for(auto i: rp.get_data())
        for(auto j: i)
            sum += j;
    rr = 100.0 * sum / (pow(rp.size(),2));
    return rr;
}

double FindThreshold(Attractor& att, double percentage, double tolerance, double hint) {
    if(hint == 0){
        hint = MeanPointsDistances(att);
    }
    double epsilon;
    double rr = 0;
    double init = 0, end = hint; 
    unsigned count = 0, max = 1000;
    while(fabs(RR(RecurrencePlot(att, epsilon)) - percentage) > tolerance && count < max) {
        ++count;
        double half_epsilon((end - init) / 2.0);
        double left_weight(fabs(RR(RecurrencePlot(att, (init + half_epsilon * 0.5))) - percentage));
        double right_weight(fabs(RR(RecurrencePlot(att, end -  half_epsilon * 0.5)) - percentage));
        if(left_weight < right_weight) {
            end -=  half_epsilon; 
            epsilon = end;
        }else {
            init += half_epsilon;
            epsilon = init;
        }
        if(init == end) {
            std::cerr << "Bad hint to find the RP threshold" << std::endl;
            return 0;
        }

    }
    if(count == max) {
       std::cerr << "Bad hint to find the RP threshold" << std::endl;
       return 0;
     }

    return epsilon;
}


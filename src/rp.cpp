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
    double mean_points_distance, std_poinsts_distance;
    if(hint < 0.000000001){
        mean_points_distance = MeanPointsDistances(att);
        std_poinsts_distance = StdPointsDistances(att);
        hint = mean_points_distance + 2 * std_poinsts_distance;
    }
    double epsilon;
    double init = 0, end = hint; 
    unsigned count = 0, max = 60;
    double half_epsilon = 0;
    double rr = 0, old_rr;
    while(fabs(rr - percentage) > tolerance && count < max) {

        double right_weight(fabs(RR(RecurrencePlot(att, end - half_epsilon)) - percentage));
        double left_weight(fabs(RR(RecurrencePlot(att, init + half_epsilon)) - percentage));
        double half_epsilon((end - init) / 2.0);

        if(left_weight < right_weight) {
            end -=  half_epsilon; 
            epsilon = end;
        }else {
            init += half_epsilon;
            epsilon = init;
        }

        old_rr = rr;
        rr = RR(RecurrencePlot(att, epsilon));
        //std::cerr << "rr = " << rr << " old =" << old_rr << " count " << count << std::endl;

        if(fabs(old_rr - rr) < 0.0000001){
            if(rr < percentage){
                end = init + half_epsilon;
                init = init + half_epsilon + 2 * std_poinsts_distance;
            }else{
                end = 0;
                init = init + half_epsilon;
            }
            half_epsilon = 0;
        }
        ++count;
    }
    if(count == max) {
       std::cerr << "RP threshold not converged (max iterantions reached) " << epsilon << std::endl;
       return std::numeric_limits<double>::quiet_NaN();
    }

    return epsilon;
}


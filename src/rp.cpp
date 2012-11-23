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
    double vec_i[attractor.get_dimension()],vec_j[attractor.get_dimension()];
    for(unsigned j = 0; j < attractor.size(); j++)
        for(unsigned i = 0; i < attractor.size(); i++){
            attractor.get_vec(i,vec_i);
            attractor.get_vec(j,vec_j);
            double dist = EuclideanDistance(vec_i,vec_j,attractor.get_dimension());
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




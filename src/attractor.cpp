#include "attractor.h"


Attractor::Attractor(const TimeSeries &ts,const unsigned int dimension,const unsigned int delay) : 
dimension(dimension),delay(delay), n_vec(ts.size() - delay*(dimension-1)), data(n_vec, std::vector<double>(dimension)) 
{
    CreateLaggedArray(ts, delay, data);
    //LengthSide();
}

Attractor::Attractor(const double** p_data,const  unsigned int dimension,const  unsigned int n_vec):
dimension(dimension),delay(0), n_vec(n_vec), data(n_vec, std::vector<double>(dimension))  
{
    for(unsigned int v = 0; v < n_vec; v++){
        for(unsigned int d = 0; d < dimension; d++){
            this->data[v][d] = p_data[v][d];
        }
    }
    //LengthSide();
}

Attractor::Attractor(const std::vector< std::vector<double> > & vec_data):
dimension(vec_data[0].size()), delay(0), n_vec(vec_data.size()), data(n_vec, std::vector<double>(dimension))
{
    for(size_t v = 0; v < n_vec; v++) {
        for(size_t d = 0; d < dimension; d++) {
            this->data[v][d] = vec_data[v][d];
        }
    }
}


double Attractor::get_data(unsigned vec, unsigned dim) const{
    return (data[vec][dim]);
}

inline
const std::vector<double>  Attractor::get_vec(const unsigned vec) const{
    std::vector<double> out;
    for (unsigned i = 0; i < dimension; ++i)
    {
       out.push_back(data[vec][i]); 
    }
    return out;
}
unsigned Attractor::get_dimension() const{return(dimension);}
unsigned Attractor::get_delay() const{return(delay);}
unsigned Attractor::size() const{return(data.size());}


const std::vector<double> Attractor::operator[](const unsigned &vec) const{
    return(this->get_vec(vec));
}
inline

void CreateLaggedArray(const TimeSeries &ts,
        const unsigned int delay,
        std::vector<std::vector<double>>& data)
{
    for(size_t i = 0; i < data.size(); i++){
        for(size_t j = 0; j < data[i].size(); j++){
            data[i][j] = ts[ i + j * delay];
        }
    }
}




/**************************************************************************/
/*Warning, box counting has problems of convergence************************/
/**************************************************************************/
/*
std::vector<unsigned int> Attractor::Box_counting_D(double epsilon) {

    //unsigned cont;
    n_box.clear();

    //Searching the max an min of the atractor
    double max[dimension];
    double min[dimension];
    __find(min, max);
    //attractor larger side
    epsilon =__length*epsilon;
    //Creating Auxiliary vectors to __recursive_for
    unsigned cont_loop[dimension]; //This help us to know which loop is going on the moment.
    unsigned walker[dimension]; //This vector will assume all the possible values
    for (unsigned i = 0; i < dimension; i++) { //initializing arrays
        cont_loop[i] = 0; walker[i] = 0;
    }
    unsigned int max_loop[dimension]; //Shape of the matrix one would like to span
    for (unsigned i = 0; i < dimension; i++) {
        max_loop[i] = ceil((max[i] - min[i]) / epsilon)+1;
    }
    //Calling the function __recursive_for that will span the space
    __recursive_for(dimension, 0, cont_loop, walker, max_loop, min);

    return(n_box);


}
*/
/******************************************************************************/
/*This function generates in a recursive manner many "for" inside "for"********/
/******************************************************************************/
/*
void Attractor::__recursive_for(unsigned dimension, unsigned cont,
        unsigned* cont_loop, unsigned *walker,
        unsigned* max_loop, double *min) {

    for (unsigned i = 0; i < max_loop[cont]; i++) {
        //This "if" stacks the loops
        if (cont < dimension - 1) {
            cont_loop[cont]++;
            walker[cont] = cont_loop[cont] % max_loop[cont];
            __recursive_for(dimension, cont + 1, cont_loop, walker, max_loop, min);
        } else {
            //Do the last loop on the stack
            double vet[dimension];
            for (unsigned j = 0; j < dimension - 1; j++) {
                vet[j] = min[j] + epsilon * walker[j];
            }
            vet[dimension - 1] = min[dimension - 1] + epsilon*i;
            int n_points = __points_in_box(vet, epsilon);
            if(n_points!=0){
                n_box.push_back(n_points);}
        }
    }
}
*/
/******************************************************************************/
/*Counting how many points there are in the boxes******************************/
/******************************************************************************/
/*
int Attractor::__points_in_box(double *coord, double epsilon) {
    unsigned d = 0, v = 0;
    unsigned count_d = 0;
    int count = 0;
    //box boundaries
    double max[dimension];
    double min[dimension];
    for (d = 0; d < dimension; d++) { //init box boundaries with the target box
        max[d] = coord[d] + epsilon / 2;
        min[d] = coord[d] - epsilon / 2;
    }
    for (v = 0; v < n_vec; v++) {//running in every attractors point
        count_d = 0;
        for (d = 0; d < dimension; d++) {
            if ((data[v][d] < max[d]) && (data[v][d] >= min[d])) {
                count_d++;
            }
        }
        if (count_d == dimension) {
            count++;
        }
    }
    return (count);
}
*/
/******************************************************************************/
/*Searching for the max and min values of the atracor**************************/
/******************************************************************************/
/*
void Attractor::__find(double *min, double *max) {
    unsigned d = 0, v = 0;
    for (d = 0; d < dimension; d++) {
        max[d] = data[0][d];
        min[d] = data[0][d];
    }
    for (d = 0; d < dimension; d++) {
        for (v = 1; v < n_vec; v++) {
            if (max[d] < data[v][d]) {
                max[d] = data[v][d];
            } else {
                if (min[d] > data[v][d]) {
                    min[d] = data[v][d];
                }
            }
        }
    }
}

void Attractor::__length_side(){


    double max[dimension];
    double min[dimension];
    //Searching the max an min of the atractor
    __find(min, max);
    for(unsigned i=1;i<dimension;i++){
        if((max[i]-min[i]) > __length){
            __length=max[i]-min[i];
        }
    }

}

double Attractor::correlation_integral(double length_fraction){

    if(length_fraction>=1 || length_fraction<0){
        throw Value_error("epsilon must be between 1 and 0");
    }
    // box side as a fraction of largest object side
    epsilon =__length*length_fraction;

    int count = 0;
    for (unsigned i=0; i < n_vec; i++){
        for (unsigned j=0; j < n_vec; j++){
            double dist = __dist_eucl(data[i],data[j],dimension);
            if(dist  < epsilon) count++;
        }
    }

    return ((double)count/(n_vec*n_vec) );
}

*/

unsigned int FindNearest(const Attractor & data, const size_t index)
{
    double dist(FLT_MAX);
    size_t nn(0);
    for(size_t i(0); i < data.size(); i++){
        double aux(EuclideanDistance(data[i], data[index]));
        if(i != index && dist > aux){
            dist = aux;
            nn = i;
        }
    }
    return(nn);
}

std::vector<unsigned> FalseNearestNeighbors(const TimeSeries& ts, unsigned delay,
        unsigned max_dimension, double threshold)
{
    unsigned int dimension(1), false_neighbors(1);
    double std(ts.Std());
    std::vector<unsigned> fnn_list;
    while(dimension <= max_dimension && false_neighbors !=0){
        Attractor attractor(ts, dimension, delay);
        false_neighbors = 0;
        for(size_t i(0); i < attractor.size(); i++){
            size_t j(FindNearest(attractor, i));
            double dist(EuclideanDistance(attractor[i], attractor[j]));
            double check(fabs(ts[i + delay * dimension] - ts[j + delay * dimension]));
            if((check / dist) > threshold || (sqrt(dist * dist + check*check)) / std >= 2 ) 
                false_neighbors++;
        }
        fnn_list.push_back(false_neighbors);
        dimension++;
    }
    return(fnn_list);
}

void MeanOrbitDistance(Attractor & attractor,double & mean, double & std){
    double mean_sequential,std_sequential=0;
    for (unsigned i = 0; i < attractor.size()-1; i++)
    {
        double distance = EuclideanDistance(attractor.get_vec(i + 1),attractor.get_vec(i)); 
        mean_sequential = (distance + i*mean_sequential)/(i + 1);
        std_sequential = std_sequential + (double(i)/(i + 1.0 )) * pow(distance - mean_sequential,2);
    }
    std_sequential = sqrt(std_sequential / (attractor.size() - 1));
    
    mean = 0; std = 0;
    unsigned counter=0;
    std::vector<unsigned> neigboards(attractor.size());
    for (unsigned j = 0; j < attractor.size(); j++){
        double distance_min = DBL_MAX; 
        neigboards[j]=j;//case there is no near geometric neigboard that is not neighboard on the array, the neighboard is the same point.
        for (unsigned i = 0; i < attractor.size(); i++)
        {
            double distance = EuclideanDistance(attractor.get_vec(j),attractor.get_vec(i)); 

            if(distance < distance_min && abs(i-j) > ceil(distance / (mean_sequential-std_sequential)))
            {
                distance_min = distance;
                neigboards[j]=i;
            }
            
        }
        if(neigboards[j] != j){
            double distance = EuclideanDistance(attractor.get_vec(j),attractor.get_vec(neigboards[j])); 
                mean = (distance + counter*mean)/(counter + 1);
                std = std + (double(counter)/(counter + 1.0 )) * pow(distance - mean,2);
                counter++;
            }
    }
    std = sqrt(std / (attractor.size() - 1));
}


double MeanPointsDistances(Attractor & attractor) {
    pstatistics::Mean mean;
    for (size_t i = 0; i < attractor.size(); ++i)
    {
        for (size_t j = i + 1; j < attractor.size(); ++j) {
            double distance = EuclideanDistance(attractor[i], attractor[j]);
            mean(distance);
        }
    }
    return mean;
}

double StdPointsDistances(Attractor & attractor) {
    pstatistics::StandardDeviation std;
    for (size_t i = 0; i < attractor.size(); ++i)
    {
        for (size_t j = i + 1; j < attractor.size(); ++j) {
            double distance = EuclideanDistance(attractor[i], attractor[j]);
            std(distance);
        }
    }
    return std;
}

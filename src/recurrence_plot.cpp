#include "recurrence_plot.h"

//m_poist is the size of one side of the mxm matrix
RecurrencePlot::RecurrencePlot(const TimeSeries  &time_series,double threshold,unsigned size): size(size),data(){

    //deciding the steps and allocating memory
    unsigned step = 1;
    if(size < time_series.Size() && size >0){
        step = time_series.Size() / size;
    }else{
        this->size = time_series.Size();
    }
    Allocate(this->size,this->size);

    Generate(threshold,time_series);
}

RecurrencePlot::RecurrencePlot(const Attractor &attractor,double threshold,unsigned size): size(size),data(){

    //deciding the steps and allocating memory
    unsigned step = 1;
    if(size < attractor.Size() && size >0){
        step = attractor.Size() / size;
    }else{
        this->size = attractor.Size();
    }
    Allocate(this->size,this->size);

    Generate(threshold,attractor);
    
}
void RecurrencePlot::Generate(double limit, const TimeSeries & time_series){
    double step = time_series.Size()/size;
    for(unsigned j = 0; j < size; j++)
        for(unsigned i = 0; i < size; i++){
            double dist = fabs(time_series[i]-time_series[j]);  
            if(dist < limit){
                data[i][j]=1;
            }else{
                data[i][j]=0;
            }
        }
}
void RecurrencePlot::Generate(double limit, const Attractor & attractor){
    double vec_i[attractor.get_dimension()],vec_j[attractor.get_dimension()];
    double step = attractor.Size()/size;
    for(unsigned j = 0; j < size; j++)
        for(unsigned i = 0; i < size; i++){
            attractor.get_vec(i*step,vec_i);
            attractor.get_vec(j*step,vec_j);
            double dist = EuclideanDistance(vec_i,vec_j,attractor.get_dimension());
            if(dist < limit){
                data[i][j]=1;
            }else{
                data[i][j]=0;
            }
        }
}

RecurrencePlot::RecurrencePlot(unsigned** data,unsigned size):size(size),data(){

    this->size = size; 
    Allocate(size,size);
    for(unsigned int i=0;i<size;i++)
        for(unsigned int j=0;j<size;j++)
            this->data[i][j]=data[i][j];

}

RecurrencePlot::RecurrencePlot(const RecurrencePlot & rp) : size(rp.Size()),data()
{
    Allocate(size,size);
    for (unsigned i = 0; i < size; ++i)
    {
       for (unsigned j = 0; j < size; ++j)
       {
          data[i][j] = rp[i][j]; 
       } 
    }
}    
RecurrencePlot::~RecurrencePlot(){
    Deallocate(size,size);
}
RecurrencePlot & RecurrencePlot::operator=(const RecurrencePlot & rp){
    if(&rp != this){
        Deallocate(size,size);
        size=rp.Size();
        Allocate(size,size);
        for (unsigned i = 0; i < size; ++i)
        {
            for (unsigned j = 0;  j < size; ++j)
            {
                data[i][j] = rp[i][j]; 
            } 
        }
    }
    return *this;
}
unsigned const* RecurrencePlot::operator[](const unsigned &line) const{
    return(data[line]);
}
const unsigned RecurrencePlot::get_data(unsigned i, unsigned j) const{
    return(data[i][j]);
}
const unsigned RecurrencePlot::Size() const{ 
    return(size);
}

const NePairs RecurrencePlot::Burn(unsigned i,unsigned j) const{
    assert(i < size  && j < size );
    //store the position of burned points
    NePairs cluster;
    //store points to be burned, and that needs to check neigboards
    NePairs neigboards;
    if(data[i][j] == 1)  
    neigboards.push_back(i,j);
    while(neigboards.Size() != 0){
        unsigned x=(neigboards.get_pair(0))[0];
        unsigned y=(neigboards.get_pair(0))[1];
        neigboards.Take();
        //search for neigboards
        //bulk
        if(x<size-1)
            if(data[x+1][y] == 1 && cluster.push_back(x+1,y) )
                neigboards.push_back(x+1,y);
        if(y<size-1)
            if(data[x][y+1] == 1 && cluster.push_back(x,y+1) )
                neigboards.push_back(x,y+1);
        if(x<size-1 && y<size-1)
            if(data[x+1][y+1] == 1 && cluster.push_back(x+1,y+1) )
                neigboards.push_back(x+1,y+1);

        if(x>0)
            if(data[x-1][y] == 1 && cluster.push_back(x-1,y) )
                neigboards.push_back(x-1,y);
        if(y>0)
            if(data[x][y-1] == 1 && cluster.push_back(x,y-1) )
                neigboards.push_back(x,y-1);
        if(x>0 && y>0)
            if(data[x-1][y-1] == 1 && cluster.push_back(x-1,y-1) )
                neigboards.push_back(x-1,y-1);

        if(x>0 && y<size-1)
            if(data[x-1][y+1] == 1 && cluster.push_back(x-1,y+1) )
                neigboards.push_back(x-1,y+1);
        if(x<size-1 && y>0)
            if(data[x+1][y-1] == 1 && cluster.push_back(x+1,y-1) ) 
                neigboards.push_back(x+1,y-1);  
    }

    return(cluster);
}
void RecurrencePlot::Paint(unsigned i,unsigned j,unsigned color){ 
    NePairs cluster=Burn(i,j);
    for (unsigned i = 0; i < cluster.Size(); ++i)
    {
       data[cluster.get_pair(i)[0]][cluster.get_pair(i)[1]]=color; 
    }
    
}
/*
unsigned RecurrencePlot::points_in_diagonals(){
    std::vector<unsigned>  length;
    diagonals(length);
    unsigned sum=0;
    for(unsigned j = 0; j < length.size(); j++)
        if(length[j]>3)
            sum+=length[j];
    return(sum);
}

double RecurrencePlot::L(){
    std::vector<unsigned>  length;
    diagonals(length);
    unsigned sum=0,counter=0;
    for(unsigned j = 0; j < length.size(); j++)
        if(length[j]>3){
            sum+=length[j];
            counter++;
        }
    return(((double)sum)/counter);
}

unsigned RecurrencePlot::diagonals(std::vector<unsigned> & length){
    ne_pairs burnt;
    for(unsigned j = 1; j < size-1; j++)
        for(unsigned i = 1; i < size-1; i++)
            if(data[i][j]==1)
                if(!burnt.exist(i,j)){
                    ne_pairs cluster(this->burn(i,j));
                    burnt.push_back(cluster);  
                    unsigned diag_length=diagonal_size(cluster);
                    if(diag_length>1)
                        length.push_back(diag_length);  
                }
    return(length.size());
}

unsigned RecurrencePlot::diagonal_size(ne_pairs & cluster){
    unsigned max=0;
    for(unsigned counter=0;counter<cluster.size();counter++){
        std::vector< unsigned > aux(cluster.get(counter));
        unsigned length=1;
        bool exist=1;
        while(exist){
            exist=cluster.exist(aux[0]+length,aux[1]+length);
            if(exist)
                length++;
        }
        if(length>max)
            max=length;
    }
    return(max);
}

double RecurrencePlot::DET(){
    unsigned count=0;
    for(unsigned j = 0; j < size; j++)
        for(unsigned i = 0; i < size; i++)
            if(data[i][j]==1)
                count++;

    return(((double)points_in_diagonals())/count);

}

double RecurrencePlot::RR(){
    unsigned count=0;
    for(unsigned j = 0; j < size; j++)
        for(unsigned i = 0; i < size; i++)
            if(data[i][j]==1)
                count++;
    return(((double)count)/(size*size));

}

*/
inline
void RecurrencePlot::Allocate(unsigned n_lines,unsigned n_columns){
    this->data = new unsigned* [n_lines];
    for(unsigned i=0;i < n_lines;i++) this->data[i] = NULL;
    for(unsigned i=0;i<n_lines;i++) this->data[i] = new unsigned[n_columns];
}
inline
void RecurrencePlot::Deallocate(unsigned n_lines,unsigned n_columns){
    for(unsigned i=0;i<n_lines;i++){
        delete [] this->data[i];
    }
    delete [] this->data;   
}



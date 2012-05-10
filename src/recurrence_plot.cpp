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

PairsList RecurrencePlot::Burn(unsigned i,unsigned j) const{
    assert(i < size  && j < size );
    //store the position of burned points
    PairsList cluster;
    //store points to be burned, and that needs to check neigboards
    PairsList neigboards;

    if(data[i][j] == 1)  
        neigboards.insert({i,j});
    while(neigboards.size() != 0){
        unsigned x = neigboards.begin()->first;
        unsigned y = neigboards.begin()->second;
        neigboards.erase(neigboards.begin());
        //search for neigboards
        //bulk
        if(x<size-1)
            if(data[x+1][y] == 1 && cluster.insert({x+1,y}).second )
                neigboards.insert({x+1,y});
        if(y<size-1)
            if(data[x][y+1] == 1 && cluster.insert({x,y+1}).second )
                neigboards.insert({x,y+1});
        if(x<size-1 && y<size-1)
            if(data[x+1][y+1] == 1 && cluster.insert({x+1,y+1}).second )
                neigboards.insert({x+1,y+1});

        if(x>0)
            if(data[x-1][y] == 1 && cluster.insert({x-1,y}).second )
                neigboards.insert({x-1,y});
        if(y>0)
            if(data[x][y-1] == 1 && cluster.insert({x,y-1}).second )
                neigboards.insert({x,y-1});
        if(x>0 && y>0)
            if(data[x-1][y-1] == 1 && cluster.insert({x-1,y-1}).second )
                neigboards.insert({x-1,y-1});

        if(x>0 && y<size-1)
            if(data[x-1][y+1] == 1 && cluster.insert({x-1,y+1}).second )
                neigboards.insert({x-1,y+1});
        if(x<size-1 && y>0)
            if(data[x+1][y-1] == 1 && cluster.insert({x+1,y-1}).second ) 
                neigboards.insert({x+1,y-1});  
    }

    return(cluster);
}
PairsList RecurrencePlot::Paint(unsigned i,unsigned j,unsigned color){ 
    PairsList cluster(Burn(i,j));
    for (auto i: cluster)
    {
        data[i.first][i.second]=color; 
    }
    return cluster;
}

unsigned DiagonalSize(PairsList  cluster){
    unsigned max=0;
    unsigned length;
    for(auto counter: cluster ){
        length=1;
        bool exist=1;
        while(exist){
            exist=cluster.count({counter.first+length,counter.second+length});
            if(exist)
                length++;
        }
        if(length>max)
            max=length;
    }
    if(max > 1)
        return(max);
    else
        return(0);
}

std::vector<unsigned> RecurrencePlot::Diagonals(){
    unsigned color = 10;
    std::vector<unsigned> length;
    for(unsigned j = 1; j < size-1; j++)
        for(unsigned i = 1; i < size-1; i++)
            if(data[i][j] == 1){
                    unsigned len = DiagonalSize(Paint(i,j,color));  
                    if( len > 0 )
                        length.push_back(len);  
            }
    for(unsigned j = 1; j < size-1; j++)
        for(unsigned i = 1; i < size-1; i++)
            if(data[i][j] == color)
                data[i][j] = 1;
     return(length);
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



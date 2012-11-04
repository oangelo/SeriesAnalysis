#include "recurrence_analytics.h"

unsigned NumberOfBlackDots(RecurrencePlot data){
    unsigned count = 0;
    for(unsigned j = 0; j < data.size(); j++)
        for(unsigned i = 0; i < data.size(); i++)
            if(data[i][j] == BLACK_DOT )
                count++;
    return(count);
}

PairsList Burn(RecurrencePlot data, unsigned i,unsigned j){

    unsigned size = data.size();
    assert(i < size  && j < size );

    //store the position of burned points
    PairsList cluster;
    //store points to be burned, and that needs to check neigboards
    PairsList neigboards;

    if(data[i][j] == BLACK_DOT)  
        neigboards.insert({i,j});
    while(neigboards.size() != 0){
        unsigned x = neigboards.begin()->first;
        unsigned y = neigboards.begin()->second;
        neigboards.erase(neigboards.begin());
        //search for neigboards
        //bulk
        if(x<size-1)
            if(data[x+1][y] == BLACK_DOT && cluster.insert({x+1,y}).second )
                neigboards.insert({x+1,y});
        if(y<size-1)
            if(data[x][y+1] == BLACK_DOT  && cluster.insert({x,y+1}).second )
                neigboards.insert({x,y+1});
        if(x<size-1 && y<size-1)
            if(data[x+1][y+1] == BLACK_DOT   && cluster.insert({x+1,y+1}).second )
                neigboards.insert({x+1,y+1});

        if(x>0)
            if(data[x-1][y] == BLACK_DOT   && cluster.insert({x-1,y}).second )
                neigboards.insert({x-1,y});
        if(y>0)
            if(data[x][y-1] == BLACK_DOT   && cluster.insert({x,y-1}).second )
                neigboards.insert({x,y-1});
        if(x>0 && y>0)
            if(data[x-1][y-1] == BLACK_DOT   && cluster.insert({x-1,y-1}).second )
                neigboards.insert({x-1,y-1});

        if(x>0 && y<size-1)
            if(data[x-1][y+1] == BLACK_DOT   && cluster.insert({x-1,y+1}).second )
                neigboards.insert({x-1,y+1});
        if(x<size-1 && y>0)
            if(data[x+1][y-1] == BLACK_DOT   && cluster.insert({x+1,y-1}).second ) 
                neigboards.insert({x+1,y-1});  
    }

    return(cluster);
}

PairsList Paint(RecurrencePlot & data, unsigned i,unsigned j,unsigned color){ 
    PairsList cluster(Burn(data, i, j));
    for (auto i: cluster)
    {
        data[i.first][i.second] = color; 
    }
    return cluster;
}

unsigned DiagonalLength(PairsList  cluster){
    unsigned max = 0;
    unsigned length;
    for(auto counter: cluster ){
        length = 1;
        bool exist = true;
        while(exist){
            exist = cluster.count({counter.first + length, counter.second + length});
            if(exist)
                length++;
        }
        if(length > 1 && length > max)
            max = length;
    }
    return(max);
}

unsigned DiagonalLengthOrthogonal(PairsList  cluster){
    unsigned max = 0;
    unsigned length;
    for(auto counter: cluster ){
        length = 1;
        bool exist = true;
        while(exist){
            exist = cluster.count({counter.first + length, counter.second - length});
            if(exist)
                length++;
        }
        if(length > 1 && length > max)
            max = length;
    }
    return(max);
}

std::vector<unsigned> PointsInDiagonal(RecurrencePlot data){
    unsigned color = 10;
    RecurrencePlot data_aux(data);
    std::vector<unsigned> length;
    for(unsigned j = 0; j < data.size(); j++)
        for(unsigned i = 0; i < data.size(); i++)
            if(data[i][j] == BLACK_DOT){
                    PairsList  cluster(Paint(data, i, j, color));
                    unsigned len = DiagonalLength(cluster);  
                    unsigned len_orth = DiagonalLengthOrthogonal(cluster);  
                    //std::cout << len << " " << len_orth << std::endl;
                    if( len > 1.5 * (len_orth + 1))
                        length.push_back(cluster.size());  
            }
     return(length);
}

std::vector<int> PointsInDiagonalDistances(RecurrencePlot data){
    unsigned color = 10;
    std::vector<int> length;
    for(unsigned j = 0; j < data.size(); j++)
        for(unsigned i = 0; i < data.size(); i++)
            if(data[i][j] == BLACK_DOT){
                    unsigned len = DiagonalLength(Paint(data, i, j, color));  
                    if( len > 0 )
                        length.push_back(i-j);  
            }
     return(length);
}

unsigned VerticalLength(PairsList  cluster){
    unsigned max = 0;
    unsigned length;
    for(auto counter: cluster ){
        length = 1;
        bool exist = true;
        while(exist){
            exist = cluster.count({counter.first + length, counter.second});
            if(exist)
                length++;
        }
        if(length > 1 && length > max)
            max = length;
    }
    return(max);
}

unsigned HorizontalLength(PairsList  cluster){
    unsigned max = 0;
    unsigned length;
    for(auto counter: cluster ){
        length = 1;
        bool exist = true;
        while(exist){
            exist = cluster.count({counter.first, counter.second + length});
            if(exist)
                length++;
        }
        if(length > 1 && length > max)
            max = length;
    }
    return(max);
}

std::vector<unsigned> PointsInVertical(RecurrencePlot data){
    unsigned color = 10;
    std::vector<unsigned> length;
    for(unsigned j = 0; j < data.size(); j++)
        for(unsigned i = 0; i < data.size(); i++)
            if(data[i][j] == BLACK_DOT){
                    PairsList  cluster(Paint(data, i, j, color));
                    unsigned len_vertical = VerticalLength(cluster);  
                    unsigned len_horizontal = HorizontalLength(cluster);  
                    if( len_vertical > 1.5 * (len_horizontal + 1))
                        length.push_back(cluster.size());  
            }
     return(length);
}

RecurrenceAnalytics::RecurrenceAnalytics(const RecurrencePlot & data)
:verticals(PointsInVertical(data)), diagonals(PointsInDiagonal(data)), 
diagonals_distances(PointsInDiagonalDistances(data)), n_black_dots(NumberOfBlackDots(data)),
size(data.size()),
points_in_diagonals(std::accumulate(diagonals.begin(), diagonals.end(), 0.0)),
points_in_verticals(std::accumulate(verticals.begin(), verticals.end(), 0.0)) {
}

double RecurrenceAnalytics::RR(){
    return(static_cast<double>(n_black_dots) / (size*size));
}

double RecurrenceAnalytics::DET(){
    return(static_cast<double>(points_in_diagonals) / n_black_dots);
}

double RecurrenceAnalytics::LAM(){
    return(static_cast<double>(points_in_verticals) / n_black_dots);
}
double RecurrenceAnalytics::RATIO(){
    return DET() / RR();
}
double RecurrenceAnalytics::L(){
    return(static_cast<double>(points_in_diagonals) / diagonals.size());
}
double RecurrenceAnalytics::TT(){
    return(static_cast<double>(points_in_verticals) / verticals.size());
}
double RecurrenceAnalytics::LMax(){
    return *std::max_element(diagonals.begin(), diagonals.end()); 
}
double RecurrenceAnalytics::VMax(){
    return *std::max_element(verticals.begin(), verticals.end()); 
}
double RecurrenceAnalytics::DIV(){
    return 1 / LMax();
}
double RecurrenceAnalytics::ENTR(){
    Histogram1D histogram(1, LMax()+1,sqrt(diagonals.size()));
    for(auto item: diagonals)
        histogram(item);
    double entropy = 0, sum = histogram.Sum();
    for (size_t i = 0; i < histogram.size(); ++i){
       double p = histogram[i] / sum; 
       entropy += p * log(p); 
    }
    return entropy;
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
    return diagonals.size();
}

unsigned RecurrenceAnalytics::NumberOfVerticals() {
    return verticals.size();
}

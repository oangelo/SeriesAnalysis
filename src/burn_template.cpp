#include "burn.h"

template<class Type>
PairsList Burn(Type data, unsigned i,unsigned j){

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

template<class Type>
PairsList Paint(Type& data, unsigned i,unsigned j,unsigned color){ 
    PairsList cluster(Burn(data, i, j));
    for (auto i: cluster)
    {
        data[i.first][i.second] = color; 
    }
    return cluster;
}

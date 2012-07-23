PairsList RecurrencePlot::Burn(unsigned i,unsigned j) const{
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
PairsList RecurrencePlot::Paint(unsigned i,unsigned j,unsigned color){ 
    PairsList cluster(Burn(i,j));
    for (auto i: cluster)
    {
        data[i.first][i.second]=color; 
    }
    return cluster;
}

unsigned Diagonalsize(PairsList  cluster){
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



double RecurrencePlot::L(){
    unsigned sum = 0;
    for(auto item : diagonals)
            sum += item;
    return(double(sum) / diagonals.size());
}

double RecurrencePlot::DET(){
    unsigned points_in_diagonals = 0;
    for(auto item : diagonals)
        points_in_diagonals += item;
    return(double(points_in_diagonals) / n_black_dots);
}

double RecurrencePlot::RR(){
    unsigned size = data.size();
    return(double(n_black_dots) / (size*size));
}


std::vector<unsigned> RecurrencePlot::Diagonals(){
    unsigned size = data.size();
    unsigned color = 10;
    std::vector<unsigned> length;
    for(unsigned j = 0; j < size; j++)
        for(unsigned i = 0; i < size; i++)
            if(data[i][j] == BLACK_DOT){
                    unsigned len = Diagonalsize(Paint(i,j,color));  
                    if( len > 0 )
                        length.push_back(len);  
            }
    for(unsigned j = 0; j < size; j++)
        for(unsigned i = 0; i < size; i++)
            if(data[i][j] == color)
                data[i][j] = BLACK_DOT;
     return(length);
}

unsigned RecurrencePlot::CountBlackDots(){
    unsigned size = data.size();
    unsigned count=0;
    for(unsigned j = 0; j < size; j++)
        for(unsigned i = 0; i < size; i++)
            if(data[i][j] == BLACK_DOT )
                count++;
    return(count);
}


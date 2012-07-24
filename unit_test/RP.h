#include "math.h"
#include "../src/recurrence_analytics.h"

TEST(RecurrencePlot,ConstructorTimeSeries){
    double data[3] = { 1.0, 2.0, 3.0 };
    TimeSeries teste(data, 3); 
    RecurrencePlot rp(teste,0.5);
     for(unsigned j = 0; j < 3; j++){
        for(unsigned i = 0; i < 3; i++){
            if(i==j){
                EXPECT_EQ(rp[i][j],1);
            }else{
                EXPECT_EQ(rp[i][j],0);
            } 
        }
    }
    RecurrencePlot rp1(teste,1.5); 
    ASSERT_DOUBLE_EQ(rp1[0][2],0.0);
    ASSERT_DOUBLE_EQ(rp1[2][0],0.0);
}

TEST(RecurrencePlot,ConstructorAttractor){
    double data[3] = { 1.0, 2.0, 3.0 };
    TimeSeries teste(data, 3); 
    Attractor at_teste(teste,1,1);
    RecurrencePlot rp(at_teste,0.5);
    for(unsigned j = 0; j < rp.size(); j++){
        for(unsigned i = 0; i < rp.size(); i++){
            if(i==j){
                EXPECT_EQ(rp[i][j],1);
            }else{
                EXPECT_EQ(rp[i][j],0);
            } 
        }
    }
    RecurrencePlot rp1(teste,1.5); 
    ASSERT_DOUBLE_EQ(rp1[0][2],0.0);
    ASSERT_DOUBLE_EQ(rp1[2][0],0.0);
}

TEST(RecurrencePlot,ConstructorMatrix){
    std::vector<unsigned> v1({ 1, 0, 0 });
    std::vector<unsigned> v2({ 0, 1, 0 });
    std::vector<unsigned> v3({ 0, 0, 1 });
    std::vector<std::vector<unsigned>> matrix;
    matrix.push_back(v1);
    matrix.push_back(v2);
    matrix.push_back(v3);
    RecurrencePlot rp(matrix);
    for(unsigned j = 0; j < rp.size(); j++){
        for(unsigned i = 0; i < rp.size(); i++){
            if(i==j){
                EXPECT_EQ(rp[i][j],1);
            }else{
                EXPECT_EQ(rp[i][j],0);
            } 
        }
    }
}


class RecurrenceAnalyticsTests: public ::testing::Test {
    public:
        vector<vector<unsigned>> data;
        RecurrenceAnalyticsTests(){
            size_t size = 40;
            for (size_t j = 0; j < 40; ++j)
                data.push_back(std::vector<unsigned>(size,0));
            for (size_t i = 0; i < size; ++i){
                    data[(int)fabs(19-19*sin((3.14*double(i))/size))][i]=1;
                    data[(int)fabs(20-19*sin((3.14*double(i))/size))][i]=1;
                    data[i][(int)fabs(19-9*sin((3.14*double(i))/size))]=1;
                    data[i][(int)fabs(20-15*sin((3.14*double(i))/size))]=1;
                    data[i][i]=1;
                    data[20][i]=1;
                }
            data[19][39]=1;

        }

};

TEST_F(RecurrenceAnalyticsTests, Burn)
{ 
    RecurrencePlot rp(data);
    //NePairs burn(rp.burn(0,20));
    Paint(rp, 1,20,2);
    for(unsigned j = 0; j < data.size(); j++){
        for(unsigned i = 0; i < data.size(); i++){
            if(data[i][j] == 1)
                EXPECT_EQ(rp[i][j], 2);
            //std::cout <<  rp[i][j]<< " ";
        }
        //std::cout << std::endl;
    }

}

TEST_F(RecurrenceAnalyticsTests, Diagonals)
{ 
    size_t size = data.size();
    for (size_t i = 0; i < size; ++i){

        for (size_t j = 0; j < size; ++j){
            data[i][j]=0;
        }
    } 
    for (size_t j = 4; j < size / 2; ++j){
        data[j][j]=1;
        data[j][j+1]=1;
        data[j+17][size/2 - j]=1;
        data[j][25]=1;
        data[2][j+10]=1;
    }
    for (size_t j = 10; j < size / 3; ++j)
        data[j][j+20]=1;
    data[12][31]=1;
    data[11][32]=1;
    data[11][30]=1;
    data[10][31]=1;
    data[20][0]=1;
 
    RecurrencePlot rp(data);

    EXPECT_EQ( DiagonalSize(Burn(rp, 4, 4)) , 16);
    EXPECT_EQ( DiagonalSize(Burn(rp, 10, 30)), 3);
    EXPECT_EQ( DiagonalSize(Burn(rp, 14, (size / 2) - 4)), 0);
    EXPECT_EQ( DiagonalSize(Burn(rp, 20, 0)), 0);
    EXPECT_EQ( DiagonalSize(Burn(rp, 5, 25)), 0);
    EXPECT_EQ( DiagonalSize(Burn(rp, 2, 14)), 0);
    EXPECT_EQ( Diagonals(rp).size(), 2);
    EXPECT_EQ( Diagonals(rp)[0], 16);
    EXPECT_EQ( Diagonals(rp)[1], 3);

/*    for(unsigned j = 0; j < rp.size(); j++){
        for(unsigned i = 0; i < rp.size(); i++){
            std::cout <<  rp[size-1-i][j]<< " ";
        }
        std::cout << std::endl;
    }
//*/
}

TEST_F(RecurrenceAnalyticsTests, Verticals)
{ 
    size_t size = data.size();
    for (size_t i = 0; i < size; ++i){

        for (size_t j = 0; j < size; ++j){
            data[i][j]=0;
        }
    } 
    for (size_t j = 4; j < size / 2; ++j){
        data[j][j]=1;
        data[j][j+1]=1;
        data[j+17][size/2 - j]=1;
        data[j][25]=1;
        data[2][j+10]=1;
    }
    for (size_t j = 10; j < size / 3; ++j)
        data[j][j+20]=1;
    data[12][31]=1;
    data[11][32]=1;
    data[11][30]=1;
    data[10][31]=1;
    data[20][0]=1;
 
    RecurrencePlot rp(data);

    EXPECT_EQ( VerticalSize(Burn(rp, 4, 4)) , 2);
    EXPECT_EQ( VerticalSize(Burn(rp, 10, 30)), 3);
    EXPECT_EQ( VerticalSize(Burn(rp, 14, (size / 2) - 4)), 0);
    EXPECT_EQ( VerticalSize(Burn(rp, 20, 0)), 0);
    EXPECT_EQ( VerticalSize(Burn(rp, 5, 25)), 0);
    EXPECT_EQ( VerticalSize(Burn(rp, 2, 14)), 16);
    EXPECT_EQ( Verticals(rp).size(), 3);
    EXPECT_EQ( Verticals(rp)[0], 2);
    EXPECT_EQ( Verticals(rp)[1], 16);
    EXPECT_EQ( Verticals(rp)[2], 3);
/*
    for(unsigned j = 0; j < rp.size(); j++){
        for(unsigned i = 0; i < rp.size(); i++){
            std::cout <<  rp[size-1-i][j]<< " ";
        }
        std::cout << std::endl;
    }
//*/
}

TEST_F(RecurrenceAnalyticsTests, BlackDots)
{ 
    for (size_t i = 0; i < data.size(); ++i){

        for (size_t j = 0; j < data.size(); ++j){
            data[i][j]=0;
        }
    } 
    for (size_t j = 4; j < data.size() / 2; ++j){
        data[j][j]=1;
        data[j][j+1]=1;
        data[j+17][data.size() / 2 - j]=1;
        data[j][25]=1;
        data[2][j+10]=1;
    }
 
    RecurrencePlot rp(data);
    EXPECT_EQ( NumberOfBlackDots(rp), 80);
/*
    for(unsigned j = 0; j < rp.size(); j++){
        for(unsigned i = 0; i < rp.size(); i++){
            std::cout <<  rp[data.size()-1-i][j]<< " ";
        }
        std::cout << std::endl;
    }
//*/
}



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
        vector<vector<unsigned>> data2;
        vector<vector<unsigned>> data_rp;
        RecurrenceAnalytics analytics;

        RecurrenceAnalyticsTests():data(), data_rp(), analytics(RecurrencePlot(data)){
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

            vector<unsigned> v7({0, 0, 0, 1, 0, 0, 1});
            vector<unsigned> v6({0, 0, 1, 0, 0, 0, 1});
            vector<unsigned> v5({0, 1, 0, 0, 0, 0, 1});
            vector<unsigned> v4({1, 0, 0, 0, 0, 0, 1});
            vector<unsigned> v3({0, 0, 0, 0, 1, 0, 0});
            vector<unsigned> v2({1, 1, 1, 0, 0, 1, 0});
            vector<unsigned> v1({0, 0, 0, 0, 0, 0, 1});

            data_rp.push_back(v1);
            data_rp.push_back(v2);
            data_rp.push_back(v3);
            data_rp.push_back(v4);
            data_rp.push_back(v5);
            data_rp.push_back(v6);
            data_rp.push_back(v7);

            analytics = RecurrenceAnalytics(RecurrencePlot(data_rp));
            
            data2.push_back({0, 1, 1, 1, 0, 0, 0});
            data2.push_back({0, 0, 1, 1, 1, 0, 0});
            data2.push_back({0, 0, 0, 1, 1, 1, 0});
            data2.push_back({0, 0, 0, 0, 1, 1, 0});
            data2.push_back({0, 1, 1, 0, 0, 1, 0});
            data2.push_back({0, 1, 1, 0, 0, 0, 0});
            data2.push_back({0, 0, 0, 0, 0, 0, 0});

           /*
            for (int i = data2.size() - 1; i >= 0 ; i--)
            {
                for (size_t j = 0; j < data2.size(); ++j)
                    std::cout << data2[i][j] << " ";
                std::cout << std::endl;
            }
            //*/
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

TEST_F(RecurrenceAnalyticsTests, PointsInDiagonal)
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
//*
    EXPECT_EQ( DiagonalLength(Burn(rp, 4, 4)) , 16);
    EXPECT_EQ( DiagonalLength(Burn(rp, 10, 30)), 3);
    EXPECT_EQ( DiagonalLength(Burn(rp, 14, (size / 2) - 4)), 0);
    EXPECT_EQ( DiagonalLength(Burn(rp, 20, 0)), 0);
    EXPECT_EQ( DiagonalLength(Burn(rp, 5, 25)), 0);
    EXPECT_EQ( DiagonalLength(Burn(rp, 2, 14)), 0);
    EXPECT_EQ( PointsInDiagonal(rp).size(), 1);
    EXPECT_EQ( PointsInDiagonal(rp)[0], 32);

    EXPECT_EQ( PointsInDiagonal(RecurrencePlot(data_rp)).size(), 1);
    EXPECT_EQ( PointsInDiagonal(RecurrencePlot(data_rp))[0], 4);
    EXPECT_EQ( PointsInDiagonalDistances(RecurrencePlot(data_rp))[0], 3);
//*/

    

    /*
    for (size_t i = 0; i < data.size(); ++i)
    {
        for (int j = data.size() - 1; j >= 0 ; j--)
            std::cout << data[i][j] << " ";
        std::cout << std::endl;
    }
    //*/
    EXPECT_EQ( PointsInDiagonal(RecurrencePlot(data2)).size(), 1);
    EXPECT_EQ( PointsInDiagonal(RecurrencePlot(data2))[0], 12);

   
}

TEST_F(RecurrenceAnalyticsTests, PointsInVertical)
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
    /*
    for (size_t i = 0; i < data.size(); ++i)
    {
        for (int j = data.size() - 1; j >= 0 ; j--)
            std::cout << data[i][j] << " ";
        std::cout << std::endl;
    }
    //*/
    EXPECT_EQ( VerticalLength(Burn(rp, 4, 4)) , 2);
    EXPECT_EQ( VerticalLength(Burn(rp, 10, 30)), 3);
    EXPECT_EQ( VerticalLength(Burn(rp, 14, (size / 2) - 4)), 0);
    EXPECT_EQ( VerticalLength(Burn(rp, 20, 0)), 0);
    EXPECT_EQ( VerticalLength(Burn(rp, 5, 25)), 16);
    EXPECT_EQ( VerticalLength(Burn(rp, 2, 14)), 0);
    EXPECT_EQ( PointsInVertical(rp).size(), 1);
    EXPECT_EQ( PointsInVertical(rp)[0], 16);

    EXPECT_EQ( PointsInVertical(RecurrencePlot(data_rp)).size(), 1);
    EXPECT_EQ( PointsInVertical(RecurrencePlot(data_rp))[0], 4);

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
    EXPECT_EQ( NumberOfBlackDots(RecurrencePlot(data_rp)), 14);
    /*
       for(unsigned j = 0; j < rp.size(); j++){
       for(unsigned i = 0; i < rp.size(); i++){
       std::cout <<  rp[data.size()-1-i][j]<< " ";
       }
       std::cout << std::endl;
       }
    //*/
}

TEST_F(RecurrenceAnalyticsTests, RR){

    EXPECT_NEAR( analytics.RR(), 14.0/(7.0 * 7.0), 0.0001);
}
TEST_F(RecurrenceAnalyticsTests, DET){
    EXPECT_NEAR( analytics.DET(), 4.0/(14.0), 0.0001);
}
TEST_F(RecurrenceAnalyticsTests, LAM){
    EXPECT_NEAR( analytics.LAM(), 4.0/(14.0), 0.0001);
}
TEST_F(RecurrenceAnalyticsTests, RATIO){
    EXPECT_NEAR( analytics.RATIO(), 4.0*(7.0 * 7.0) / (14.0 * 14.0), 0.0001);
}
TEST_F(RecurrenceAnalyticsTests, L){
    EXPECT_NEAR( analytics.L(), 4.0, 0.0001);
}
TEST_F(RecurrenceAnalyticsTests, TT){
    EXPECT_NEAR( analytics.TT(), 4.0, 0.0001);
}
TEST_F(RecurrenceAnalyticsTests, LMax){
    EXPECT_NEAR( analytics.LMax(), 4.0, 0.0001);
}
TEST_F(RecurrenceAnalyticsTests, VMax){
    EXPECT_NEAR( analytics.VMax(), 4.0, 0.0001);
}
TEST_F(RecurrenceAnalyticsTests, DIV){
    EXPECT_NEAR( analytics.DIV(), 1 / 4.0, 0.0001);
}
TEST_F(RecurrenceAnalyticsTests, ENTR){
}
TEST_F(RecurrenceAnalyticsTests, TREND){
}

  

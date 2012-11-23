#include "math.h"
#include "../src/rqa.h"


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
/*
TEST(RecurrenceAnalyticsTests, RR){

    EXPECT_NEAR( analytics.RR(), 14.0/(7.0 * 7.0), 0.0001);
}
TEST(RecurrenceAnalyticsTests, DET){
    EXPECT_NEAR( analytics.DET(), 4.0/(14.0), 0.0001);
}
TEST(RecurrenceAnalyticsTests, LAM){
    EXPECT_NEAR( analytics.LAM(), 4.0/(14.0), 0.0001);
}
TEST(RecurrenceAnalyticsTests, RATIO){
    EXPECT_NEAR( analytics.RATIO(), 4.0*(7.0 * 7.0) / (14.0 * 14.0), 0.0001);
}
TEST(RecurrenceAnalyticsTests, L){
    EXPECT_NEAR( analytics.L(), 4.0, 0.0001);
}
TEST(RecurrenceAnalyticsTests, TT){
    EXPECT_NEAR( analytics.TT(), 4.0, 0.0001);
}
TEST(RecurrenceAnalyticsTests, LMax){
    EXPECT_NEAR( analytics.LMax(), 4.0, 0.0001);
}
TEST(RecurrenceAnalyticsTests, VMax){
    EXPECT_NEAR( analytics.VMax(), 4.0, 0.0001);
}
TEST(RecurrenceAnalyticsTests, DIV){
    EXPECT_NEAR( analytics.DIV(), 1 / 4.0, 0.0001);
}
TEST(RecurrenceAnalyticsTests, ENTR){
}
TEST(RecurrenceAnalyticsTests, TREND){
}

 */ 

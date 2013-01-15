#include <map> 
#include "../src/chaos.h"

class AttractorTest : public ::testing::Test {
    public:
        TimeSeries *teste;
        Attractor *att;
        ~AttractorTest(){
            delete teste;
            delete att;
        }


        AttractorTest(){
            double data[10] = { 1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0, 10.0 };
            teste = new TimeSeries(data,10);
            att = new Attractor(*teste,2,1);
        }

};

TEST_F(AttractorTest,values) {
    EXPECT_EQ((*att).size(),9);
    EXPECT_EQ((*att).get_dimension(),2);
    EXPECT_EQ((*att).get_delay(),1);
    for (unsigned i = 0; i < att->size(); ++i)
    {
        ASSERT_DOUBLE_EQ(att->get_data(i,0),i+1);
    }
    for (unsigned i = 0; i < att->size(); i++)
    {
        ASSERT_DOUBLE_EQ(att->get_data(i,1),i+2);
    }
}

TEST_F(AttractorTest,vec){
    double data[10] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
    TimeSeries teste(data, 10);
    Attractor att_test(teste,2,1);
    for(unsigned i=0;i<9;i++){
        std::vector<double> vec1(att_test[i]);
        EXPECT_EQ(vec1[0],data[i]);
        EXPECT_EQ(vec1[1],data[i+1]);
    }
    for(unsigned i=0;i<9;i++){
        std::vector<double> vec1;
        vec1 = att_test[i];
        EXPECT_EQ(vec1[0],data[i]);
        EXPECT_EQ(vec1[1],data[i+1]);
        EXPECT_EQ(att_test[i][0],data[i]);
        EXPECT_EQ(att_test[i][1],data[i+1]);

    }
}
TEST_F(AttractorTest,MeanOrbitDistanceTest){
    std::vector< std::vector< double > > att;
    std::vector< double > vet(2);
    for (unsigned i = 0; i < 10; i++)
    {
        vet[0]=i;
        vet[1]=0;
        att.push_back(vet);
    }
    for (unsigned i = 0; i < 10; i++)
    {
        vet[0]=9-i;
        vet[1]=1;
        att.push_back(vet);
    }
    Attractor attractor(att);     
    double mean,std;
    MeanOrbitDistance(attractor,mean,std);
    ASSERT_DOUBLE_EQ(mean,1.0);
    ASSERT_DOUBLE_EQ(std,0.0);
}

TEST(CHAOS, PoincareMap){
    std::vector<double> point1({0, -1});
    std::vector<double> point2({0,  1});
    std::map<unsigned, double> surface;
    surface[1]=0;
    auto result(PointOfSurfaceIntersect(point1, point2, surface));
    for(auto i: result)
        std::cout << i << " ";
        std::cout << std::endl;

}

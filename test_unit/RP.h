#include "../src/recurrence_plot.h"

//#include "../src/chaos.h"

class RecurrencePlotTeste: public ::testing::Test {
    public:
        TimeSeries *teste;
        Attractor *att;
        RecurrencePlot *rp;


        ~RecurrencePlotTeste(){
            delete teste;
            delete att;
            delete rp;
        }


        RecurrencePlotTeste(){
            double data[10] = { 1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0, 10.0 };
            teste = new TimeSeries(data,10);
            att = new Attractor(*teste,2,1);
            rp = new RecurrencePlot(*att);
        }

};

TEST_F(RecurrencePlotTeste,constructor){
    double data[3] = { 1.0, 2.0, 3.0 };
    TimeSeries teste(data, 3); //inicia um histograma com 10 bins com range de 1 até 10
    Attractor at_teste(teste,1,1);
    RecurrencePlot rp(at_teste,3,0.5);
     for(unsigned j = 0; j < 3; j++){
        for(unsigned i = 0; i < 3; i++){
            if(i==j){
                ASSERT_DOUBLE_EQ(rp.get_data(i,j),1.0);
            }else{
                ASSERT_DOUBLE_EQ(rp.get_data(i,j),0.0);
            } 
            //std::cout << rp.get_pair_data(i,j) << " ";
        }
        //std::cout << std::endl;
    }
    
}

TEST(Test_rp_burn,burn)
{ 

    unsigned length=25;
    double data1[25] = { 1.0, 2.0, 3.0,4.0,3.0,5.0,6.0,20,8,9,6,20,12,6,14,15,20,17,20,17,20,20,22,23,25};
    TimeSeries teste1(data1, length); //inicia um histograma com 10 bins com range de 1 até 10
    Attractor at_teste1(teste1,1,1);
    RecurrencePlot rp(at_teste1,length,0.5);
    //NePairs burn(rp.burn(4,4));
    rp.Paint(4,4);
    for(unsigned j = 0; j < length; j++){
        for(unsigned i = 0; i < length; i++){
            std::cout << rp.get_data(i,j) << " ";
        }
        std::cout << std::endl;
    }/*
    EXPECT_EQ((burn.get_pair(0))[0],4);EXPECT_EQ((burn.get_pair(0))[1],4);
    EXPECT_EQ((burn.get_pair(1))[0],5);EXPECT_EQ((burn.get_pair(1))[1],5);
    EXPECT_EQ((burn.get_pair(2))[0],3);EXPECT_EQ((burn.get_pair(2))[1],3);
    EXPECT_EQ((burn.get_pair(3))[0],6);EXPECT_EQ((burn.get_pair(3))[1],6);
    EXPECT_EQ((burn.get_pair(4))[0],2);EXPECT_EQ((burn.get_pair(4))[1],3);
    EXPECT_EQ((burn.get_pair(5))[0],3);EXPECT_EQ((burn.get_pair(5))[1],2);
    EXPECT_EQ((burn.get_pair(6))[0],2);EXPECT_EQ((burn.get_pair(6))[1],2);
    EXPECT_EQ((burn.get_pair(7))[0],1);EXPECT_EQ((burn.get_pair(7))[1],2);
    EXPECT_EQ((burn.get_pair(8))[0],1);EXPECT_EQ((burn.get_pair(8))[1],1);
    EXPECT_EQ((burn.get_pair(9))[0],0);EXPECT_EQ((burn.get_pair(9))[1],0);
*/
//    EXPECT_EQ(rp.diagonal_size(burn),7);
}
/*


TEST(Test_rp_RR)
{

    double data[3] = { 1.0, 1.0, 1.0 };
    time_series teste(data, 3); //inicia um histograma com 10 bins com range de 1 até 10
    Attractor at_teste(teste,1,1);
    recurrence_plot rp(at_teste,3,0.5);
    CHECK_CLOSE(rp.RR(),1,0.0001);

    double data1[3] = { 1.0, 2.0, 3.0 };
    time_series teste1(data1, 3); //inicia um histograma com 10 bins com range de 1 até 10
    Attractor at_teste1(teste1,1,1);
    recurrence_plot rp1(at_teste1,3,0.5);
    CHECK_CLOSE(rp1.RR(),0.33333,0.0001);

}



TEST(Test_rp_diagonals)
{  
    double data1[10] = { 1.0, 2.0, 3.0,3.0,5.0,6.0,7.0,2,3,20};
    time_series teste1(data1, 10); //inicia um histograma com 10 bins com range de 1 até 10
    Attractor at_teste1(teste1,1,1);
    recurrence_plot rp(at_teste1,10,0.5);
    ne_pairs burn(rp.burn(7,1));
    std::vector<unsigned> length;
    EXPECT_EQ(rp.diagonals(length),3);
    EXPECT_EQ(rp.points_in_diagonals(),14);
    CHECK_CLOSE(rp.DET(),14.0/18.0,0.00001);
}
*/

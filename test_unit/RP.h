
#include "math.h"
#include "../src/recurrence_plot.h"

//#include "../src/chaos.h"

class RecurrencePlotTeste: public ::testing::Test {
    public:
        TimeSeries *teste;
        Attractor *att;
        RecurrencePlot *rp;
        unsigned** data;
        unsigned size;            

        ~RecurrencePlotTeste(){
            delete teste;
            delete att;
            delete rp;
            for (size_t i = 0; i < size; ++i)
                delete[] data[i];
            delete data;
        }


        RecurrencePlotTeste(){
            double data_ts[10] = { 1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0, 10.0 };
            teste = new TimeSeries(data_ts,10);
            att = new Attractor(*teste,2,1);
            rp = new RecurrencePlot(*att,1);
            size = 40;
            data = new unsigned* [size];
            for (size_t i = 0; i < size; ++i)
                data[i]=NULL;
            for (size_t i = 0; i < size; ++i)
                data[i]=new unsigned[size];
            for (size_t i = 0; i < size; ++i)
                for (size_t j = 0; j < size; ++j){
                    data[i][j]=0;
                }
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

TEST(RecurrencePlot,constructor){
    double data[3] = { 1.0, 2.0, 3.0 };
    TimeSeries teste(data, 3); //inicia um histograma com 10 bins com range de 1 até 10
    //Attractor at_teste(teste,1,1);
    RecurrencePlot rp(teste,0.5);
     for(unsigned j = 0; j < 3; j++){
        for(unsigned i = 0; i < 3; i++){
            if(i==j){
                ASSERT_DOUBLE_EQ(rp.get_data(i,j),1.0);
            }else{
                ASSERT_DOUBLE_EQ(rp.get_data(i,j),0.0);
            } 
          //std::cout << rp.get_data(i,j) << " ";
        }
        //std::cout << std::endl;
    }
    RecurrencePlot rp1(teste,1.5); 
    ASSERT_DOUBLE_EQ(rp1.get_data(0,2),0.0);
    ASSERT_DOUBLE_EQ(rp1.get_data(2,0),0.0);
}

TEST_F(RecurrencePlotTeste,burn)
{ 

    unsigned length=size;
    RecurrencePlot rp(data,size);
    //NePairs burn(rp.burn(0,20));
    rp.Paint(1,20,2);
    for(unsigned j = 0; j < length; j++){
        for(unsigned i = 0; i < length; i++){
            if(data[i][j]==1)
                EXPECT_EQ(rp[i][j],2);
            //std::cout <<  rp[i][j]<< " ";
        }
        //std::cout << std::endl;
    }

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

//TODO Make tests to test all the Time Series Methods

class TimeSeriesTest : public ::testing::Test {
    public:
        TimeSeries *teste;
        TimeSeries *teste_alt;
        ~TimeSeriesTest(){
            delete teste;
            delete teste_alt;
        }


        TimeSeriesTest(){
            double data[10] = { 1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0, 10.0 };
            teste = new TimeSeries(data,10);
            double data1[10] = { 1.0, 0.0, 1.0, 0.0, 1.0, 0.0, 1.0, 0.0, 1.0, 0.0 };
            teste_alt = new TimeSeries(data1,10); 
        }

};

TEST_F(TimeSeriesTest,values) {
    EXPECT_EQ((*teste).Size(),10);
    for (int i = 0; i < 10; i++)
    {
        ASSERT_DOUBLE_EQ((*teste)[i],i+1);
    }

}

TEST_F(TimeSeriesTest,mean) {
        ASSERT_DOUBLE_EQ((*teste_alt).Mean(),0.5);
}

TEST_F(TimeSeriesTest,max_min) {
    ASSERT_DOUBLE_EQ((*teste).Max(),10.0);
    ASSERT_DOUBLE_EQ((*teste).Min(),1.0);
}
  
TEST_F(TimeSeriesTest,std) {
        ASSERT_DOUBLE_EQ((*teste_alt).Std(),0.5);
}
/*
   TEST(Test_binary_entropy)
   {
   double data[10] = { 1.0, 0.0, 1.0, 0.0, 1.0, 0.0, 1.0, 0.0, 1.0, 0.0 };
   TimeSeries teste(data, 10);
   CHECK_EQUAL(entropy(teste),1);
   }

   TEST(Test_auto_correlation_function)
   {
   double data[10] = { 0.0, 1.0, 0.0, 1.0, 0.0, 1.0, 0.0, 1.0, 0.0, 1.0 };
   TimeSeries teste(data, 10); //inicia um histograma com 10 bins com range de 1 até 10
   CHECK_CLOSE(auto_corr_func(teste,2),1.0,0.01);
   }

   TEST(Test_auto_correlation_function_almost_same)
   {
   double data[10] = { 1.1, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0 };
   TimeSeries teste(data, 10); //inicia um histograma com 10 bins com range de 1 até 10
   CHECK_CLOSE(auto_corr_func(teste,2),-0.02,0.01);
   }
   */

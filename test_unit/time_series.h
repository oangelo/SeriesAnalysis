    TEST(Test_binary_entropy)
    {
        double data[10] = { 1.0, 0.0, 1.0, 0.0, 1.0, 0.0, 1.0, 0.0, 1.0, 0.0 };
        time_series teste(data, 10);
        CHECK_EQUAL(entropy(teste),1);
    }
    
    TEST(Test_auto_correlation_function)
    {
        double data[10] = { 0.0, 1.0, 0.0, 1.0, 0.0, 1.0, 0.0, 1.0, 0.0, 1.0 };
        time_series teste(data, 10); //inicia um histograma com 10 bins com range de 1 até 10
        CHECK_CLOSE(auto_corr_func(teste,2),1.0,0.01);
    }
    
    TEST(Test_auto_correlation_function_almost_same)
    {
        double data[10] = { 1.1, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0 };
        time_series teste(data, 10); //inicia um histograma com 10 bins com range de 1 até 10
        CHECK_CLOSE(auto_corr_func(teste,2),-0.02,0.01);
    }

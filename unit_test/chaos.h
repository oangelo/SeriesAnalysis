#include "../src/attractor.h"

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
    for (unsigned i = 0; i < att->size(); i++)
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
        double vec1[2];
        att_test.get_vec(i,vec1);
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
TEST_F(AttractorTest,AttractorFromFile){
    std::ofstream test_file;
    test_file.open("test.txt");
    for (size_t i = 0; i < 10; ++i)
    {
       test_file << i << " " << i + 1 << std::endl; 
    }
    test_file.close();
    Attractor attractor_file("test.txt");
    for (size_t i = 0; i < 10; ++i)
    {
      ASSERT_DOUBLE_EQ(attractor_file[i][0],i); 
      ASSERT_DOUBLE_EQ(attractor_file[i][1],i + 1); 
    }
    rm("test.txt");
}
/*

    TEST(Test_attractor_from_file)
    {
        //Make a better test! write to a file, read attractor then delete the file  
        std::string file="to_analyse/dp_s/dp_s_series_1_.out";
        Attractor at_teste(file);
        //for(unsigned i =0;i<20;i++)
        //std::cout << at_teste.get(i,0) << " " << at_teste.get(i,1) << " " << at_teste.get(i,2) << " " << at_teste.get(i,3) << std::endl;  
    }
    

SUITE(Attractor_class_test)
{
        TEST(Test_constructor_from_ts)
        {
                double data[10] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
                time_series teste(data, 10);
                Attractor att_test(teste,2,1);
                CHECK_EQUAL(att_test.size(),9);
                CHECK_EQUAL(att_test.get_dim(),2);
                for(unsigned i=0;i<9;i++){
                        CHECK_EQUAL(att_test.get(i,0),data[i]);
                        CHECK_EQUAL(att_test.get(i,1),data[i+1]);
                }
        }

        TEST(Test_constructor_get_vec)
        {
               }
        }

}

    
    TEST(Test__lagged_array)
    {
        
        double data[7] = { 1, 2, 3, 4, 5, 6, 7 };
        unsigned tau = 3, dim = 2;
        time_series ts(data, 7);
        double **foo;
        foo = create_lagged_array(ts, tau, dim);
        for (int i = 0; i < 4; i++) {
            CHECK_CLOSE(foo[i][0],ts[i],EPSILON);
            CHECK_CLOSE(foo[i][1],ts[i+tau],EPSILON);
        }
        del_lagged_array(foo, 4, dim);
        
    }
    
    TEST(Test_find_nn)
    {
        double data[7] = { 0, 2, 3, 4, 5, 6, 7 };
        unsigned tau = 3, dim = 2;
        time_series ts(data, 7);
        Attractor foo(ts,dim,tau);
        unsigned i = __find_nearest(foo, 1);
        CHECK_CLOSE(foo.get(i,0),0,EPSILON);
        CHECK_CLOSE(foo.get(i,1),4,EPSILON);
    }
    
    
    TEST(Test_lorenz_mutual_info)
    {
        ofstream dados;
        int N = 8192;
        int TAU_MAX = 50;
        dados.open("test/mutual_info_lorenz.dat");
        double initial_cond[3] = { 10.1, 10.1, 10.1 };
        double integration_step = 0.01;
        Generator aux;
        //parameters for caos
        double* datax = aux.lorenz_flux('x',
                                        16.0,
                                        45.92,
                                        4.0,
                                        initial_cond,
                                        integration_step,
                                        N);
        
        time_series teste(datax, N);
        
        for (int i = 0; i < TAU_MAX; i++) {
            dados << i << " " << mutual_information(teste, i) << endl;
        }
        #ifdef FULL_TEST
        system("echo \"plot \\\"mutual_info_lorentz.dat\\\" w l;  pause 1 \" | gnuplot");
        #endif
        //This graphic should look like the graph in Fig. 8 (page 1345)
        //of The analysis of chaotic data in physical systems, by Abarbanel.
        dados.close();
        delete[] datax;
    }
    
    SUITE(Time_Series_boxcounting_test)
{
    
    TEST(Test_Attractor_ts)
    {
        double data[10] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
        unsigned tau = 1, dim = 2;
        time_series ts(data, 10);
        Attractor atrator(ts, dim, tau);
        std::vector<unsigned int> n_box;
        n_box = atrator.Box_counting_D(0.01);
        CHECK_EQUAL(n_box.size(),9);
    }
    
    TEST(Test_Attractor_count_points_in_box)
    {
        unsigned dim = 3, n_vec = 27;
        unsigned int i, x, y, z, cont;
        //Alocating memory for data
        double **data = new double*[n_vec];
        for (i = 0; i < n_vec; i++)
            data[i] = NULL;
        for (i = 0; i < n_vec; i++)
            data[i] = new double[dim];
        //feeding data with vectors of a box
            cont = 0;
            for (x = 1; x < dim + 1; x++) {
                for (y = 1; y < dim + 1; y++) {
                    for (z = 1; z < dim + 1; z++) {
                        data[cont][0] = x;
                        data[cont][1] = y;
                        data[cont][2] = z;
                        cont++;
                    }
                }
            }
            Attractor atrator(data, dim, cont);
            //   unsigned int total_points;
            std::vector<unsigned int> n_box;
            n_box = atrator.Box_counting_D(0.5);
            for (unsigned i = 0; i < n_vec; i++) {
                delete[] data[i];
            }
            delete[] data;
            CHECK_EQUAL(n_box.size(),27);
    }
    
    TEST(Test_Attractor_dim_frac_box)
    {
        unsigned int dim = 3, n_vec = 0;
        unsigned int i, cont = 0;
        double x, y, z;
        double max = 1, e = 0.05;
        
        n_vec = ceil(max / e);
        n_vec *= n_vec * n_vec;
        
        double **data = new double*[n_vec];
        for (i = 0; i < n_vec; i++)
            data[i] = NULL;
        for (i = 0; i < n_vec; i++)
            data[i] = new double[dim];
        
        cont = 0;
        for (x = 0; x < max; x = x + e) {
            for (y = 0; y < max; y = y + e) {
                for (z = 0; z < max; z = z + e) {
                    data[cont][0] = x;
                    data[cont][1] = y;
                    data[cont][2] = z;
                    cont++;
                }
            }
        }
        
        Attractor atrator2(data, dim, n_vec);
        for (unsigned i = 0; i < n_vec; i++) {
            delete[] data[i];
        }
        delete[] data;
        
        std::vector<unsigned int> n_box;
        n_box = atrator2.Box_counting_D(e);
        
        int total = 0;
        for (unsigned i = 0; i < n_box.size(); i++)
            total += n_box[i];
        
        CHECK_EQUAL(n_vec,total);
        CHECK_EQUAL(n_vec,n_box.size());
    }
} 

SUITE(Time_Series_Fractal_dimensions_test)
{
    //#define TEST_DIMENSION_HENON_D2
    //#define TEST_DIMENSION_HENON_D1
    //#define TEST_DIMENSION_HENON_D0
    //#define TEST_DIMENSION_LORENZ_D2
    

    TEST(Test_counting_box_henon_D_2)
    {
        ofstream dados;
        ofstream dados2;
        dados.open("test/henon_d2.dat");
        Generator aux;
        int size = 30000;
        int dim = 2, i;
        
        double* datax = aux.henon_map('x', 1.4, 0.3, 0.1, 0.1, size);
        double* datay = aux.henon_map('y', 1.4, 0.3, 0.1, 0.1, size);
        //Allocating memory for data
        double **data = new double*[size];
        for (i = 0; i < size; i++)
            data[i] = NULL;
        for (i = 0; i < size; i++)
            data[i] = new double[dim];
        
        //feeding data with henon values
            for (i = 0; i < size; i++) {
                data[i][0] = datax[i];
                data[i][1] = datay[i];
            }
            
            //creating the object attractor
            Attractor atrator(data, dim, size);
            
            //deallocating memory
            for (int i = 0; i < size; i++) {
                delete[] data[i];
            }
            delete[] data;
            //making the box counting
            double e;
            for (e = 0.4; e > 0.00005; e *= 0.9) {
                double C_2 = 0;
                C_2 = atrator.correlation_integral(e);
                dados << u_log(atrator.get_epsilon(), 2) << " " << u_log(C_2, 2)
                << std::endl;
            }
            dados.close();
    }

    TEST(Test_frac_dim_henon_attractor_D_1)
    {
        ofstream dados;
        dados.open("test/henon_d1.dat");
        Generator aux;
        int size = 1000000;
        int dim = 2, i;
        
        double* datax = aux.henon_map('x', 1.4, 0.3, 0.1, 0.1, size);
        double* datay = aux.henon_map('y', 1.4, 0.3, 0.1, 0.1, size);
        //Allocating memory for data
        double **data = new double*[size];
        for (i = 0; i < size; i++)
            data[i] = NULL;
        for (i = 0; i < size; i++)
            data[i] = new double[dim];
        
        //feeding data with lorenz values
            for (i = 0; i < size; i++) {
                data[i][0] = datax[i];
                data[i][1] = datay[i];
            }
            
            //creating the object attractor
            Attractor atrator(data, dim, size);
            
            //deallocating memory
            for (int i = 0; i < size; i++) {
                delete[] data[i];
            }
            delete[] data;
            
            //making the box counting
            double e = 0.1;
            
            for (e = 0.1; e > 0.0001; e /= 2) {
                std::vector<unsigned int> n_box;
                n_box = atrator.Box_counting_D(e);
                double entropy = 0.0;
                
                for (int i = 0; i < (int) n_box.size(); i++) {
                    entropy += ((double) n_box[i] / size) * log((double) n_box[i]
                    / size);
                }
                dados << log(atrator.get_epsilon()) << " " << entropy  << std::endl;
            }
            dados.close();
    }

    TEST(Test_frac_dim_henon_attractor_D_0)
    {
        ofstream dados;
        dados.open("test/henon_d0.dat");
        Generator aux;
        int size = 80000;
        int dim = 2, i;
        
        double* datax = aux.henon_map('x', 1.4, 0.3, 0.1, 0.1, size);
        double* datay = aux.henon_map('y', 1.4, 0.3, 0.1, 0.1, size);
        //Allocating memory for data
        double **data = new double*[size];
        for (i = 0; i < size; i++)
            data[i] = NULL;
        for (i = 0; i < size; i++)
            data[i] = new double[dim];
        
        //feeding data with henon map values
            for (i = 0; i < size; i++) {
                data[i][0] = datax[i];
                data[i][1] = datay[i];
            }
            
            //creating the object attractor
            Attractor atrator(data, dim, size);
            
            //deallocating memory
            for (int i = 0; i < size; i++) {
                delete[] data[i];
            }
            delete[] data;
            
            //making the box counting
            double e = 0.1;
            
            for (e = 0.1; e > 0.0001; e /= 2) {
                std::vector<unsigned int> n_box;
                n_box = atrator.Box_counting_D(e);
                dados << -log(atrator.get_epsilon()) << " " << log(n_box.size())  << std::endl;
            }
            dados.close();
    }

    TEST(Test_frac_dim_lorenz_attractor_D_2)
    {
        Generator aux;
        int size = 8192;
        int dim = 3; //lorenz dimension
        
        int i;
        double r0[3] = { 0.1, 0.1, 0.1 };
        double* datax = aux.lorenz_flux('x', 16.0, 45.92, 4.0, r0, 0.01, size);
        double* datay = aux.lorenz_flux('y', 16.0, 45.92, 4.0, r0, 0.01, size);
        double* dataz = aux.lorenz_flux('z', 16.0, 45.92, 4.0, r0, 0.01, size);
        
        //Allocating memory for data
        double **data = new double*[size];
        for (i = 0; i < size; i++)
            data[i] = NULL;
        for (i = 0; i < size; i++)
            data[i] = new double[dim];
        
        //feeding data with lorenz values
            for (i = 0; i < size; i++) {
                data[i][0] = datax[i];
                data[i][1] = datay[i];
                data[i][2] = dataz[i];
            }
            
            //creating the object attractor
            Attractor atrator(data, dim, size);
            
            //deallocating memory
            for (int i = 0; i < size; i++) {
                delete[] data[i];
            }
            delete[] data;
            
            //making the box counting
            
            double e = 0.3;
            ofstream dados;
            dados.open("test/lorenz_d2.dat");
            
            for (e = 0.8; e > 0.0003; e /= 2) {
                double c2 = atrator.correlation_integral(e);
                dados << u_log(atrator.get_epsilon(), 2) << " " << u_log(c2, 2)
                << std::endl;
            }
            dados.close();
            
    }
    
}


SUITE(Time_Series_Generator_test)
{
    
    TEST(Test_generator_logistic_map_periodic_two)
    {
        
        Generator aux;
        double* data = aux.logistic_map(3.4, 0.7, 10);
        time_series teste(data, 10);
        
        for (int i = 0; i < 10; i++) {
            //cout << (*teste)[i] << endl;
            if (i % 2) {
                CHECK_CLOSE(teste[i],0.842154,EPSILON);
            } else {
                CHECK_CLOSE(teste[i],0.451963,EPSILON);
            }
        }
        
        delete[] data;
    }
    
    
     //plot the x coord from lorenz attractor for visual inspection
     //data is saved: ./test/lorenz_x_coord.dat
     
    TEST(Test_lorenz_x_coord_gen)
    {
        ofstream dados;
        dados.open("test/lorenz_x_coord.dat");
        Generator aux;
        int size = 8192;
        double r0[3] = { 0.1, 0.1, 0.1 };
        double* data = aux.lorenz_flux('x', 16.0, 45.92, 4.0, r0,0.01, size);
        time_series teste(data, size);
        for (int i = 0; i < size; i++)
            dados << teste[i] << std::endl;
        dados.close();
        #ifdef FULL_TEST
        system("echo \"plot \\\"Lorenz_gen.dat\\\" w l, \\\"Lorenz.dat\\\" u :1 w l;  pause 1 \" | gnuplot");
        #endif
        delete[] data;
    }
    
    
    //plot the hennon map for visual inspection
    //data is saved: ./test/henon_map.dat
     
    TEST(Test_henon_atractor_gen)
    {
        ofstream dados;
        dados.open("test/henon_map.dat");
        Generator aux;
        int size = 7000;
        double* datax = aux.henon_map('x', 1.4, 0.3, 0.1, 0.1, size);
        double* datay = aux.henon_map('y', 1.4, 0.3, 0.1, 0.1, size);
        for (int i = 0; i < size; i++)
            dados << datax[i] << " " << datay[i] << std::endl;
        dados.close();
        #ifdef FULL_TEST
        system("echo \"plot \\\"henon_gen.dat\\\" w d;  pause 1 \" | gnuplot");
        #endif
        delete[] datax;
        delete[] datay;
    }
    
    
    //plot the lorenz attractor for visual inspection
    //data is saved: ./test/lorenz_attractor.dat
     
    TEST(Test_lorenz_visual_plot_attractor)
    {
        ofstream dados;
        dados.open("test/lorenz_attractor.dat");
        int size = 300;
        double initial_cond[3] = { 10.1, 10.1, 10.1 };
        double integration_step = 0.01;
        Generator aux;
        //parameters for caos
        double* datax = aux.lorenz_flux('x',
                                        16.0,
                                        45.92,
                                        4.0,
                                        initial_cond,
                                        integration_step,
                                        size);
        
        double* datay = aux.lorenz_flux('y',
                                        16.0,
                                        45.92,
                                        4.0,
                                        initial_cond,
                                        integration_step,
                                        size);
        
        double* dataz = aux.lorenz_flux('z',
                                        16.0,
                                        45.92,
                                        4.0,
                                        initial_cond,
                                        integration_step,
                                        size);
        
        for (int i = 0; i < size; i++) {
            dados << datax[i] << " ";
            dados << datay[i] << " ";
            dados << dataz[i] << "\n";
        }
        dados.close();
        #ifdef FULL_TEST
        //Chamando o gnuplot para plotar o atrator de lorenz
        //(Veja se parece uma borboleta!)
        system("echo \"splot \\\"Lorenz.dat\\\" w l;  pause 0.5 \" | gnuplot");
        #endif
        //free memory
        delete [] datax; delete [] datay; delete [] dataz;
    }
    
    
}
*/




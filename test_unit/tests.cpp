#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <UnitTest++.h>


#include "../src/statistics.h"
#include "../src/time_series.h"
#include "../src/chaos.h"
#include "../src/numerical_integration.h"
#include "../src/models.h"

#include "RP.h"


#define TRUE 1
#define FALSE 0

using namespace std;


/*Using this in several tests to fill a test histogram*/
void fill(Histogram &teste, size_t n_bins) {
	for (int i = 0; i < (int) n_bins; i++)
		teste.increment(i);
}

SUITE(Histogram_test)
{

	TEST(Test_Constructor)
	{
		Histogram teste(11, 0, 10);
	}

	TEST(Test_Constructor_wrong)
	{
		CHECK_THROW(Histogram teste(11, 10, 10), Value_error);
	}

	TEST(Test_Constructor_scott)
	{
		Histogram teste(28.87, 100, 0, 100);
		fill(teste, 100);
		CHECK_EQUAL(teste.n_bins(),5);
	}

	TEST(Test_Constructor_scott_wrong)
	{
		CHECK_THROW(Histogram teste(28.87, 100, 100, 100), Value_error);
	}

	TEST(Test_Increment)
	{
		Histogram teste(10, 0, 10);
		for (unsigned int i = 0; i < 10; i++)
			teste.increment(i);
		for (int i = 0; i < 10; i++)
			CHECK_EQUAL(teste[i],0.1);
	}

	TEST(Test_Increment_wrong)
	{
		Histogram teste(10, 0, 10);
		for (int i = 0; i < 10; i++)
			teste.increment(i);
		CHECK_THROW(teste.increment(15), Value_error);
	}

	/*Testing boundary conditions for the smallest(2 bins) histogram*/
	TEST(Test_Increment_smallest)
	{
		Histogram teste(2, 0, 2);
		for (int i = 0; i < 2; i++)
			teste.increment(i);
		CHECK_EQUAL(teste[1],0.5);
		CHECK_EQUAL(teste[0],0.5);
	}

	TEST(Test_Increment_repeat)
	{
		Histogram teste(2, 0, 10);
		double data[10] = { 1, 1, 1, 1, 2, 2, 2, 3, 3, 4 };
		for (int i = 0; i < 10; i++)
			teste.increment(data[i]);
		CHECK_EQUAL(teste[1],0);
		CHECK_CLOSE(teste[0],1.0,EPSILON);
	}

	TEST(Test_mean)
	{
		Histogram teste(10, 0, 10);
		for (int i = 0; i < 10; i++)
			teste.increment(i);
		CHECK_CLOSE(5.0, teste.mean(), 0.01);
	}

	TEST(Test_Operator_accessing_int)
	{
		Histogram teste(10, 0, 10);
		for (int i = 0; i < 10; i++)
			teste.increment(i);

		for (int i = 0; i < 10; i++)
			CHECK_CLOSE(0.1, teste[i], 0.01);
	}

	TEST(Test_Operator_accessing_double)
	{
		Histogram teste(10, 0, 10);
		for (int i = 0; i < 10; i++)
			teste.increment(9.9);
		CHECK_CLOSE(1, teste[9.9], 0.01);
		CHECK_CLOSE(0.0, teste[0], 0.01);
	}

	TEST(Test_Operator_accessing_double_wrong)
	{
		Histogram teste(10, 0, 10);
		for (int i = 0; i < 10; i++)
			teste.increment(9.9);
		CHECK_THROW(teste[11.1], Value_error);
	}

	TEST(Test_Operator_accessing_int_wrong)
	{
		Histogram teste(10, 0, 10);
		for (int i = 0; i < 10; i++)
			teste.increment(i);
		CHECK_THROW(teste[12], Index_error);
	}

	TEST(Test_Operator_call_double)
	{
		Histogram teste(10, 0, 10);
		for (int i = 0; i < 10; i++)
			teste.increment(i);

		CHECK_EQUAL(9, teste(9.9));
	}

	TEST(Test_Operator_call_double_wrong)
	{
		Histogram teste(10, 0, 10);
		for (int i = 0; i < 10; i++)
			teste.increment(i);
		CHECK_THROW(teste(11.1), Value_error);
	}

	TEST(Test_n_bins)
	{
		Histogram teste(10, 0, 10);
		for (int i = 0; i < 10; i++)
			teste.increment(i);
		CHECK_EQUAL(teste.n_bins(), 10);
	}

	TEST(Test_range_max)
	{
		Histogram teste(10, 0, 10);
		for (int i = 0; i < 10; i++)
			teste.increment(i);
		CHECK_EQUAL(teste.range_max(), 10);
	}

	TEST(Test_reset)
	{
		Histogram teste(10, 0, 10);
		fill(teste, 10);
		teste.reset();
		for (int i = 0; i < 10; i++)
			CHECK_EQUAL(teste[i], 0.0);
	}

	TEST(Test_sum)
	{
		Histogram teste(10, 0, 10);
		fill(teste, 10);
		CHECK_EQUAL(teste.sum(), 10.0);
	}

	TEST(Test_sigma)
	{
		Histogram teste(10, 0, 10);
		fill(teste, 10);
		CHECK_CLOSE(2.87, teste.sigma(), 0.01); // succeeds 2.87
	}

	TEST(Test_max_val)
	{
		Histogram teste(10, 0, 10);
		fill(teste, 10);
		teste.increment(0.2);
		CHECK_EQUAL(teste.max_val(), 2.0);
	}

	TEST(Test_min_val)
	{
		Histogram teste(10, 0, 10);
		fill(teste, 9);
		fill(teste, 10);
		CHECK_EQUAL(teste.min_val(), 1.0);
	}

	TEST(Test_max_bin)
	{
		Histogram teste(10, 0, 10);
		fill(teste, 10);
		teste.increment(0.2);
		CHECK_EQUAL(teste.max_bin(), 0);
	}

	TEST(Test_min_bin)
	{
		Histogram teste(10, 0, 10);
		fill(teste, 10);
		fill(teste, 9);
		CHECK_EQUAL(teste.min_bin(), 9);
	}

	TEST(Test_Copy_Constructor)
	{
		Histogram teste(10, 1, 10);

		//Colocando valores dentro dos bins para teste
		teste.increment(3.2);
		teste.increment(4.2);
		teste.increment(3.5);
		teste.increment(7);
		teste.increment(7.1);
		teste.increment(7.2);
		teste.increment(7.2);

		Histogram teste2 = teste;

	}

}// END TEST SUITE

SUITE(utils_h_functions_test)
{
	TEST(Test__dist_eucl)
	{
		double vec_a[4] = { 1.0, 0, 0, 0 };
		double vec_b[4] = { 0, 1.0, 0, 0 };
		CHECK_CLOSE(__dist_eucl(vec_a,vec_b,4),1.41,0.01);
	}
}

SUITE(Time_Series_class_test)
{

	TEST(Test_operator_access)
	{
		double data[10] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
		time_series teste(data, 10);
		CHECK_CLOSE(teste[5],6,0.01);
	}

	TEST(Test_operator_assign)
	{
		double data[10] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
		time_series teste(data, 10);
		teste[0] = 42;
		CHECK_CLOSE(teste[0],42,0.01);
	}

	TEST(Test_operator_assign_wrong)
	{
		double data[10] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
		time_series teste(data, 10);
		CHECK_THROW(teste[12], Index_error);
	}
	TEST(Test_mean)
	{
		double data[10] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
		time_series teste(data, 10);
		CHECK_CLOSE(teste.mean(),5.5,0.01);
	}

	TEST(Test_var)
	{
		double data[10] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
		time_series teste(data, 10);
		CHECK_CLOSE(teste.var(),8.25,0.01);
	}
	TEST(Test_std)
	{
		double data[10] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
		time_series teste(data, 10);
		CHECK_CLOSE(teste.std(),2.872,0.01);
	}

	TEST(Test_size)
	{
		double data[10] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
		time_series teste(data, 10);
		CHECK_EQUAL(teste.size(),10);
	}
	TEST(Test_max)
	{
		double data[10] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
		time_series teste(data, 10);
		CHECK_EQUAL(teste.max(),10);
	}

	TEST(Test_min)
	{
		double data[10] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
		time_series teste(data, 10);
		CHECK_EQUAL(teste.min(),1);
	}
	
	TEST(Test_print_and_read_file)
	{
	        double data[10] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
		time_series teste1(data, 10);
		teste1.print("test/read_file.txt");
		time_series teste2("test/read_file.txt");
		for(double i=1;i<11;i++)
		  CHECK_EQUAL(teste2[i-1],i);
	}

}// END TEST SUITE

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
		double data[10] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
		time_series teste(data, 10);
		Attractor att_test(teste,2,1);
		double vec1[2];
		for(unsigned i=0;i<9;i++){
		  att_test.get_vec(i,vec1);
		  CHECK_EQUAL(vec1[0],data[i]);
		  CHECK_EQUAL(vec1[1],data[i+1]);
		}
	}
	
}// END TEST SUITE

SUITE(Time_Series_Statistics_Functions_test)
{
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
}// END SUITE

#define TIME_SERIES
#ifdef TIME_SERIES
#define TIME_SERIES_GENERATOR
#ifdef TIME_SERIES_GENERATOR
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

	/*
	 * plot the x coord from lorenz attractor for visual inspection
	 * data is saved: ./test/lorenz_x_coord.dat
	 */
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

	/*
	 * plot the hennon map for visual inspection
	 * data is saved: ./test/henon_map.dat
	 */
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

	/*
	 * plot the lorenz attractor for visual inspection
	 * data is saved: ./test/lorenz_attractor.dat
	 */
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


}//END SUITE
#endif
#define TIME_SERIES_CHAOS
#ifdef TIME_SERIES_CHAOS
SUITE(Time_Series_chaos_function_test)
{
  
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
  
  TEST(Test_attractor_from_file)
  {
    //Make a better test! write to a file, read attractor then delete the file  
    std::string file="to_analyse/dp_s/dp_s_series_1_.out";
    Attractor at_teste(file);
    //for(unsigned i =0;i<20;i++)
    //std::cout << at_teste.get(i,0) << " " << at_teste.get(i,1) << " " << at_teste.get(i,2) << " " << at_teste.get(i,3) << std::endl;  
  }

  

  TEST(Test_ne_pairs)
  {
    ne_pairs ne;
    ne.push_back(1,2);
    ne.push_back(1,2);
    ne.push_back(1,2);
    CHECK_EQUAL(ne.size(),1);
    ne_pairs ne2;
    ne2.push_back(1,3);
    ne2.push_back(ne);
    ne.take();
    CHECK_EQUAL(ne.size(),0);
    CHECK_EQUAL(ne2.size(),2);
    CHECK_EQUAL((ne2.get(0))[0],1);
    CHECK_EQUAL((ne2.get(0))[1],3);
    CHECK_EQUAL((ne2.get(1))[0],1);
    CHECK_EQUAL((ne2.get(1))[1],2);
    ne_pairs ne0;
    ne0.push_back(1,2);
    ne0.push_back(1,3);
    ne0.push_back(1,4);
    ne0.push_back(1,5);
    ne0.push_back(1,6);
    ne0.push_back(1,7);
    ne0.push_back(1,8);
    ne0.push_back(1,9);
    ne2.push_back(ne0);
    CHECK_EQUAL(ne2.size(),8);
  }
  

  
}//END SUITE
#endif
//#define TEST_NN
//#define TEST_NN
#ifdef TEST_NN
//$$$$$$$$$$$$$$$$$$$$$$$$$$$warning%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//Memory leak on NN algorithm, need to put it in terms of attractors objects!
SUITE(Time_Series_chaos_nn_test)
{
TEST(Test_henon_NN)
{

		ofstream dados;
		Generator aux;
		int size = 7000;
		double* data = aux.henon_map('x',1.4,0.3,0.1,0.1,size);
		int tau = 1;
		int dmax = 4;
		double R_t = 12.0;
		double fnn_list[dmax];
		time_series ts(data,size);
		delete[] data;
		CHECK_EQUAL(false_nearest_nei(ts,tau,dmax,R_t,fnn_list,FALSE),2);
	}
///*
TEST(Test_lorenz_NN)
{
  ofstream dados;
  dados.open("test/lorenz_NN");
  int N = 7000;
  int tau = 10;
  int dmax = 4;
  double R_t = 12;
  double fnn_list[dmax]; // number of false neigh for each dimension
  double initial_cond[3] = { 1.1, 1.1, 1.1 };
  double integration_step = 0.01;
  Generator aux;
  
  for (int i=0;i<dmax;i++) fnn_list[i] = 0.0;
  //parameters for caos
  double* datax = aux.lorenz_flux('x',
				  16.0,
				  45.92,
				  4.0,
				  initial_cond,
				  integration_step,
				  N);
  time_series teste(datax,N);
  //This graphic should look like the graph in Fig. 14 (page 1349)
  //of The analysis of chaotic data in physical systems, by Abarbanel.
  
  CHECK_EQUAL(false_nearest_nei(teste,tau,dmax,R_t,fnn_list,TRUE),4);
  for(int i=0;i<dmax;i++)
    data << i+1 <<"  "<<  fnn_list[i] << std::endl;
  dados.close();
  delete[] datax;
}
	//*/
}//END SUITE
#endif
#endif

#ifdef TIME_SERIES_BOX_C
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
} // END SUITE
#endif
#ifdef TIME_SERIES_FRACTAL
SUITE(Time_Series_Fractal_dimensions_test)
{
//#define TEST_DIMENSION_HENON_D2
//#define TEST_DIMENSION_HENON_D1
//#define TEST_DIMENSION_HENON_D0
//#define TEST_DIMENSION_LORENZ_D2

#ifdef TEST_DIMENSION_HENON_D2

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
#endif


#ifdef TEST_DIMENSION_HENON_D1
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
#endif

#ifdef TEST_DIMENSION_HENON_D0
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
#endif


#ifdef TEST_DIMENSION_LORENZ_D2
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
#endif
}//END TEST SUITE
#endif

int main() {
    return UnitTest::RunAllTests();
}


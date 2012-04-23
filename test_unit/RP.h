  TEST(Test_recurrence_plot)
  {
    
    double data[3] = { 1.0, 2.0, 3.0 };
    time_series teste(data, 3); //inicia um histograma com 10 bins com range de 1 até 10
    Attractor at_teste(teste,1,1);
    recurrence_plot rp(at_teste,3,0.5);
    for(unsigned j = 0; j < 3; j++){
      for(unsigned i = 0; i < 3; i++){
	if(i==j){
	  CHECK_CLOSE(rp.get(i,j),1.0,0.01);
	}else{
	  CHECK_CLOSE(rp.get(i,j),0.0,0.01);
	} 
	//std::cout << rp.get(i,j) << " ";
      }
      //std::cout << std::endl;
    }
    
  }
  
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
  
  TEST(Test_rp_burn)
  {  
    double data1[7] = { 1.0, 2.0, 3.0,3.0,5.0,6.0,7.0};
    time_series teste1(data1, 7); //inicia um histograma com 10 bins com range de 1 até 10
    Attractor at_teste1(teste1,1,1);
    recurrence_plot rp(at_teste1,7,0.5);
    ne_pairs burn(rp.burn(4,4));
    /*  
     * for(unsigned j = 0; j < 7; j++){
     *   for(unsigned i = 0; i < 7; i++){
     *   s t*d::cout << rp.get(j,i) << " ";
  }
  std::cout << std::endl;
  }
  
  for(unsigned j = 0; j < burn.size(); j++)
    std::cout << (burn.get(j))[0] << " , "<<  (burn.get(j))[1] << std::endl;
  */
    CHECK_EQUAL((burn.get(0))[0],4);CHECK_EQUAL((burn.get(0))[1],4);
    CHECK_EQUAL((burn.get(1))[0],5);CHECK_EQUAL((burn.get(1))[1],5);
    CHECK_EQUAL((burn.get(2))[0],3);CHECK_EQUAL((burn.get(2))[1],3);
    CHECK_EQUAL((burn.get(3))[0],6);CHECK_EQUAL((burn.get(3))[1],6);
    CHECK_EQUAL((burn.get(4))[0],2);CHECK_EQUAL((burn.get(4))[1],3);
    CHECK_EQUAL((burn.get(5))[0],3);CHECK_EQUAL((burn.get(5))[1],2);
    CHECK_EQUAL((burn.get(6))[0],2);CHECK_EQUAL((burn.get(6))[1],2);
    CHECK_EQUAL((burn.get(7))[0],1);CHECK_EQUAL((burn.get(7))[1],2);
    CHECK_EQUAL((burn.get(8))[0],1);CHECK_EQUAL((burn.get(8))[1],1);
    CHECK_EQUAL((burn.get(9))[0],0);CHECK_EQUAL((burn.get(9))[1],0);
    
    CHECK_EQUAL(rp.diagonal_size(burn),7);
  }
  
  TEST(Test_rp_diagonals)
  {  
    double data1[10] = { 1.0, 2.0, 3.0,3.0,5.0,6.0,7.0,2,3,20};
    time_series teste1(data1, 10); //inicia um histograma com 10 bins com range de 1 até 10
    Attractor at_teste1(teste1,1,1);
    recurrence_plot rp(at_teste1,10,0.5);
    ne_pairs burn(rp.burn(7,1));
    std::vector<unsigned> length;
    /*   
     * for(unsigned j = 0; j < 10; j++){
     *   for(unsigned i = 0; i < 10; i++){
     s *t*d::cout << rp.get(i,j) << " ";
     }
     std::cout << std::endl;
     }
     
     
     for(unsigned j = 0; j < length.size(); j++)
       std::cout << length[j] << std::endl;
     //*/
       CHECK_EQUAL(rp.diagonals(length),3);
       CHECK_EQUAL(rp.points_in_diagonals(),14);
       CHECK_CLOSE(rp.DET(),14.0/18.0,0.00001);
     }
     

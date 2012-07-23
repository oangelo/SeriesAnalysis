#include <fstream>

TEST(utils,EuclideanDistance){
    std::vector<double> v1,v2;
    v1.push_back(1);
    v1.push_back(1);
    v2.push_back(2);
    v2.push_back(2);
    ASSERT_DOUBLE_EQ(EuclideanDistance(v1,v2),sqrt(2));
}


TEST(utils,Count_Lines){
    std::ofstream file;
    file.open("teste.txt");
    file << "#this is a comment"  << std::endl;
    file <<  std::endl;
    for (size_t i = 0; i < 10; ++i)
    {
        file << i << '\t' << i << std::endl;
    }
    file.close();
    unsigned lines,columns;
    file_counter("teste.txt", lines, columns);
    rm("teste.txt");
    EXPECT_EQ(lines, 10);
    EXPECT_EQ(columns, 2);

    
    
}

TEST(utils,ReadFile){
    std::ofstream file;
    file.open("teste.txt");
    file << "#this is a comment"  << std::endl;
    file << std::endl;
    for (size_t i = 0; i < 10; ++i)
    {
        
        for (size_t j = 0; j < i; ++j)
            file << j << '\t';
            file << std::endl;
    }
    file << std::endl;
    file.close();
    std::vector< std::vector<int> > data(ReadFile<int>("teste.txt"));
    unsigned i(0),j(0);
    for(auto vet :data){
        j = 0;
        for(auto item : vet){
            EXPECT_EQ(j,data[i][j]);
            j++;
            //std::cout << item << ' ' ;
        }
        //std::cout << endl;
        ++i;
    }
    rm("teste.txt");
    //EXPECT_EQ(lines, 10);
    //EXPECT_EQ(columns, 2);
}

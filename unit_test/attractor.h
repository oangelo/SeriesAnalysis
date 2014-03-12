#ifndef ATTRACTOR_TEST_H
#define ATTRACTOR_TEST_H 

#include <proxime/numerical_methods/adams_bashforth.h>
#include <proxime/functions/lorenz.h>

TEST(Attractor, FalseNearestNeighbors_WhiteNoise) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(0, 100);
 
    std::vector<value> x;
    for(size_t i(0); i < 2000; ++i){
        x.push_back(dis(gen));
    }

    TimeSeries ts(x);
    auto fnn(FalseNearestNeighbors(ts, 3, 10));
    double first(fnn[0]);
    //Expect that white noise has more then 10 dimensions
    EXPECT_TRUE(fnn[fnn.size() - 1] / first > 0.2); 
}

TEST(Attractor, FalseNearestNeighbors_Lorenz) {
//Reference: Abarbanel Review of Modern Physics, vol. 65, p. 1349 1993
    labels_values variable;
    variable["x"] = 2.61622;
    variable["y"] = 0.32533;
    variable["z"] = 2.0335135;

    labels_values parameter;
    parameter["sigma"]= 16; //sigma
    parameter["gamma"]= 45.92; //r
    parameter["beta"] = 4.0 / 3.0; //b
    LorenzFunction function(parameter);

    AdamsBashforth4Th  attractor(function, variable, 0.01);
    std::vector<value> x;
    for(size_t i(0); i < 10000; ++i)
        ++attractor;
    for(size_t i(0); i < 8192; ++i){
        ++attractor;
        x.push_back(attractor["x"]);
    }

    TimeSeries ts(x);
    auto fnn(FalseNearestNeighbors(ts, 10, 5));
    double first(fnn[0]);
    double D2(fnn[1]* 100 / first); //percentage of fnn on D = 2 
    double D3(fnn[2]* 100 / first); //percentage of fnn on D = 2 
    EXPECT_NEAR(D2, 4, 0.5);
    EXPECT_NEAR(D3, 0, 0.1);
}

#endif /* ATTRACTOR_TEST_H */

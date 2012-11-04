#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include "gtest/gtest.h"
//#include <UnitTest++.h>

#include "../src/statistics.h"
#include "../src/time_series.h"

#define TRUE 1
#define FALSE 0

using namespace std;


#include "statistics.h"
#include "time_series.h"
#include "chaos.h"
#include "utils.h"
#include "rp.h"

int main(int argc, char *argv[])
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}


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

#define TRUE 1
#define FALSE 0

using namespace std;

#include "RP.h"
#include "statistics.h"
#include "time_series.h"
#include "utils.h"

int main() {
    return UnitTest::RunAllTests();
}


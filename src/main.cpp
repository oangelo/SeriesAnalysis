#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <sstream>

#include "statistics.h"
#include "time_series.h"
#include "attractor.h"
#include "rqa.h"
#include "statistics/statistics.h"

#define TRUE 1
#define FALSE 0

using namespace std;

void PrintMan(){
    std::cout << "Usage:" << std::endl;
    std::cout << "  ./time_series [options]" << std::endl;
    std::cout << "Options:" << std::endl;
    std::cout << "  -                                 Read data from the standard input " << std::endl;
    std::cout << "  --dimension, -d <integer>         Dimension of embending " << std::endl;
    std::cout << "  --delay, -tau <integer>           Time delay for embending " << std::endl;
    std::cout << "  --threshold, -th <real>           Threshold value for RPs " << std::endl;
    std::cout << "  --threshold_std, -th_std <real>   Use as threshold a <real> * st. deviation of the time series" << std::endl;
    std::cout << "  --file, -f <string>               Read data from file" << std::endl;
    std::cout << "  --file_name, -name <string>       Name of file to save" << std::endl;
    std::cout << "Objects:" << std::endl;
    std::cout << "  --attractor, -att                 Creates an attractor " << std::endl;
    std::cout << "  --time_series, -ts                Create a time series " << std::endl;
    std::cout << "  --recurrence_plot, -rp            Create a Recurrence Plot (--print or -p to print on screen)" << std::endl;
    std::cout << "  -ts_to_att                        Create a time series from a attractor" << std::endl;
    std::cout << "Analysis:" << std::endl;
    std::cout << "  --recurrence_analysis, -rqa       Print the recurence quatification analysis of the RP" << std::endl;
    std::cout << "  --false_nearest_neighbors, -fnn   False nearest Neighbors, a method to find embending dimension" << std::endl;
    std::cout << "  --mutual_information, -mi         Mutual information, a method to find the embending delay" << std::endl;
    std::cout << "  --autocorrelation, -ac            Autocorrelation, another method to find the embending delay" << std::endl;
}

int main(int argc, char* argv[]) {
    if( argc < 2 || std::string(argv[1]) == "--help" || std::string(argv[1]) == "-h") {
        PrintMan(); 
        return 0;
    }
    std::vector<std::vector<double>> data;

    TimeSeries *time_series = NULL;
    Attractor *attractor = NULL;
    RecurrencePlot *rp = NULL;

    unsigned rows = 0,column = 0;
    unsigned bins = 0;
    unsigned dimension = 0;
    unsigned delay = 0;
    unsigned window = 1;
    double threshold = 0;

    bool from_file = false;
    std::string file_name;
        //time series statistics
    Mean<double> ts_mean;
    StDeviation<double> ts_std;
    //use a multiplot of standard deviation as threshold
    double th_std = 0;
    //###########################################################################################
    //                                        Setting Data Up
    //###########################################################################################
    for (size_t i = 1; i < argc; ++i) {


        if(std::string(argv[i]) == "--dimension" || std::string(argv[i]) == "-d")
            if(i + 1 < argc) { 
                dimension = atoi(argv[i + 1]);
                std::cerr << "Dimension: " << dimension << std::endl;
            }

        if(std::string(argv[i]) == "--delay" || std::string(argv[i]) == "-tau")
            if(i + 1 < argc) {
                delay = atoi(argv[i + 1]);
                std::cerr << "Delay: " << delay << std::endl;
            }

        if(std::string(argv[i]) == "--threshold" || std::string(argv[i]) == "-th")
            if(i + 1 < argc) {
                threshold = atof(argv[i + 1]);
                std::cerr << "Threshold: " << threshold << std::endl;
            }

        if(std::string(argv[i]) == "--window" || std::string(argv[i]) == "-w")
            if(i + 1 < argc) {
                window = atof(argv[i + 1]);
                std::cerr << "Window: " << window << std::endl;
            }

        if(std::string(argv[i]) == "--threshold_std" || std::string(argv[i]) == "-th_std")
            if(i + 1 < argc) {
                th_std = atof(argv[i + 1]);
                //setting a fake threshold
                threshold = 1000;
                std::cerr << "Th_std: " << th_std << std::endl;
            }

        if(std::string(argv[i]) == "--column" || std::string(argv[i]) == "-c")
            if(i + 1 < argc) 
                column = atoi(argv[i + 1]);

        if( std::string(argv[i]) == "--bins")
            if(i + 1 < argc) 
                bins = atoi(argv[i + 1]);
        if(std::string(argv[i]) == "--file" || std::string(argv[i]) == "-f")
            if(i + 1 < argc){ 
                data =  ReadFile<double>(std::string(argv[i+1]));
                file_name = std::string(argv[i+1]);
                std::cerr << "Reading file:" << file_name  << std::endl;
                std::cerr << "Data size: " << data.size() << ", columns: " << data[0].size()  << std::endl;
                from_file = true; 
            }

        if(std::string(argv[i]) == "-"){
            data = ReadStdin<double>();
            std::cerr << "Data size: " << data.size() << ", columns: " << data[0].size()  << std::endl;
        }

        if(std::string(argv[i]) == "--file_name" || std::string(argv[i]) == "-name")
            file_name = std::string(argv[i+1]);


    }


    //Creating basics objects
    for (size_t i = 1; i < argc; ++i) {
        if(std::string(argv[i]) == "-ts" || std::string(argv[i]) == "--time_series"){
            time_series = new  TimeSeries(data, column);
        }
        if(std::string(argv[i]) == "-att" || std::string(argv[i]) == "--attractor"){
            attractor = new  Attractor(data);
        }
    }

    if(time_series){
        for (size_t i = 0; i < time_series->size(); ++i)
        {
            ts_mean((*time_series)[i]);
            ts_std((*time_series)[i]);
        }
    }
    //Using time series
    for (size_t i = 1; i < argc; ++i)
    {
        if(std::string(argv[i]) == "-ts_to_att"){
            if(time_series) {
                attractor = new Attractor(*time_series, dimension, delay);
                std::cerr << ">> Conjurating Attractor From Time Series" << std::endl;
            }
        }
    }

    //Using attractor
    if(threshold == 0){
        if(attractor) {
            std::cerr << ">> Trying to gess the threshold" << std::endl;
            threshold = FindThreshold(*attractor, 5, 0.1);
            std::cerr << ">> threshold: " << threshold << std::endl;
            if(threshold == 0.0)
                return 1;
        }
    }
    if(th_std != 0){
        if(attractor) {
            threshold = th_std * StdPointsDistances(*attractor);
            std::cerr << ">> threshold: " << threshold << std::endl;
        }
    }
    for (size_t i = 1; i < argc; ++i)
    {
        if(std::string(argv[i]) == "-recurrence_plot" or std::string(argv[i]) == "-rp"){
            if(attractor){
                std::cerr << ">> Conjurating Recurrence Plot From Attractor" << std::endl;
                rp = new RecurrencePlot(*attractor, threshold);
            }
            for (size_t j = 0; j < window; ++j)
                for (size_t i = 0; i < rp->size()-j; ++i)
                    (*rp)[i][i+j]=0;
            for (size_t j = 0; j < window; ++j)
                for (size_t i = j; i < rp->size(); ++i)
                    (*rp)[i][i-j]=0;
            
            if( (i + 1) <= argc && (std::string(argv[i + 1]) == "--print" || std::string(argv[i + 1]) == "-p"))
                rp->PrintOnScreen();
        }
    }

    //###########################################################################################
    //                                       Making Analysis 
    //###########################################################################################
    for(size_t i = 1; i < argc; ++i)
    {
        if((std::string(argv[i]) == "--mutual_information") || (std::string(argv[i]) == "-mi")){
            double normalize = 1;
            if(i + 1 < argc) 
                if( std::string(argv[i + 1]) == "--normalize")
                    normalize = MutualInformation(*time_series, 0, bins); 

            std::cout << "#delay x mutual_info" << std::endl;
            for (size_t i = 0; i < 0.01 * time_series->size(); ++i)
                std::cout << i << " " << MutualInformation(*time_series, i, bins) / normalize << std::endl; 
        } 

        if((std::string(argv[i]) == "--false_nearest_neighbors") || (std::string(argv[i]) == "-fnn")){
            std::cerr << ">> Nearest Neighbors Max Dimension: " << dimension  << std::endl; 
            std::cerr << ">> Delay: " << delay  << std::endl; 
            std::cerr << ">> Threshold: " << threshold << std::endl; 
            std::vector<unsigned> nff =  FalseNearestNeighbors(*time_series, delay, dimension, threshold, false);
            for (size_t i = 0; i < nff.size(); ++i)
                std::cout << i + 1 << " " << nff[i] << std::endl; 
        } 

        if((std::string(argv[i]) == "--autocorrelation") || (std::string(argv[i]) == "-ac")){
            std::cout << "# delay x auto_correlation" << std::endl;
            for (size_t i = 1; i < time_series->size() / 10.0; ++i)
                std::cout << i << " " << AutoCorrelation(*time_series, i) << std::endl; 
        }

        if((std::string(argv[i]) == "--recurrence_analysis") || (std::string(argv[i]) == "-rqa")) {
            if(rp){
                RecurrenceAnalytics analytics(*rp);
                std::cout << file_name  << " "; 
                std::cout << analytics.RR()  << " "; 
                std::cout << analytics.DET()  << " "; 
                std::cout << analytics.L()  << " "; 
                std::cout << analytics.LMax()  << " "; 
                std::cout << " "; 
                std::cout << analytics.LAM()  << " "; 
                std::cout << analytics.TT()  << " "; 
                std::cout << analytics.VMax()  << " "; 
                std::cout << " "; 
                std::cout << analytics.RATIO()  << " "; 
                std::cout << analytics.NumberOfDiagonals()  << " "; 
                std::cout << analytics.NumberOfRecurrence()  << " "; 
                //std::cout << ts_mean  << " "; 
                //std::cout << ts_std << " "; 

                std::cout << std::endl; 
            }
        }
    }

    if(time_series)
        delete time_series;
    if(attractor)
        delete attractor;
    if(rp)
        delete rp;
    return 0; 
}


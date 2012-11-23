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
void rp_files(std::string from_path,std::string to_path){
    std::vector<std::string> file_name(ls(from_path));
    mkdir(to_path);
    for(unsigned i=0;i<file_name.size();i++){   
        std::stringstream file_path;
        file_path << from_path << file_name[i]; 
        std::stringstream result_path;
        result_path << to_path << file_name[i];
        std::ofstream dados;

        Attractor att_data(file_path.str());
        double mean,std;
        MeanOrbitDistance(att_data,mean,std);
        std::cout << file_name[i] << ' ' << mean << ' ' << std <<  std::endl;
        RecurrencePlot rp(att_data,2*(mean+std));	
        std::string out=result_path.str();
        dados.open(out.c_str());
        //dados << "#RR= " << rp.RR() << std::endl; 
        //dados << "#DET= " << rp.DET() << std::endl;
        //dados << "#L= " << rp.L() << std::endl;
        for (unsigned k = 0; k < rp.size(); k++)
            for (unsigned j = 0; j < rp.size(); j++)
                if(rp[k][j]==1)
                    dados << k <<"  "<<  j << std::endl;

        dados.close();

    } 
}

/*
   void rp_ts_files(std::string from_path,std::string to_path){
   std::vector<std::string> file_name(ls(from_path));
   mkdir(to_path);
   for(unsigned i=0;i<file_name.size();i++){   
   std::stringstream file_path;
   file_path << from_path << file_name[i]; 
   std::stringstream result_path;
   result_path << to_path << file_name[i];
   std::ofstream dados;

   time_series ts(file_path.str());
   Attractor att_data(ts,4,2);
   recurrence_plot rp(att_data);	
   std::string out=result_path.str();
   dados.open(out.c_str());
   for (unsigned k = 0; k < rp.size(); k++)
   for (unsigned j = 0; j < rp.size(); j++)
   if(rp.get(k,j)==1)
   dados << k <<"  "<<  j << std::endl;

   dados.close();
   } 
   }

   void mutual_info(std::string from_path,std::string to_path){
   std::vector<std::string> file_name(ls(from_path));
   mkdir(to_path);
   for(unsigned i=0;i<file_name.size();i++){   
   std::stringstream file_path;
   file_path << from_path << file_name[i]; 
   std::stringstream result_path;
   result_path << to_path << file_name[i];
   std::ofstream dados;

   time_series teste(file_path.str());
   std::string out=result_path.str();
   dados.open(out.c_str());
   int TAU_MAX = 50;
   double buffer;
   for (int i = 0; i < TAU_MAX; i++) {
   if(i==0)
   {
   buffer=mutual_information(teste, i);
   dados << i << " " << 1 << endl;
   }else{
   dados << i << " " << mutual_information(teste, i)/buffer << endl;
   }
   }
   dados.close();


   }
   }


   void NN(std::string from_path,std::string to_path){
   std::vector<std::string> file_name(ls(from_path));
   mkdir(to_path);
   for(unsigned i=0;i<file_name.size();i++){   
   std::stringstream file_path;
   file_path << from_path << file_name[i]; 
   std::stringstream result_path;
   result_path << to_path << file_name[i];
   std::ofstream dados;

   time_series teste(file_path.str());
   std::string out=result_path.str();
   dados.open(out.c_str());
   int N = 7000;
   int tau = 1;
int dmax = 8;
double R_t = 12.0;
double fnn_list[dmax]; // number of false neigh for each dimension
false_nearest_nei(teste,tau,dmax,R_t,fnn_list,TRUE);
for (int i = 0; i < dmax; i++) {
    dados << i <<"  "<<  fnn_list[i]/N << std::endl;
}
dados.close();


}
}
*/
int main(int argc, char* argv[]) {
    bool from_file = false;
    std::vector<std::vector<double>> data;

    TimeSeries *time_series = NULL;
    double rows = 0,column = 0;
    unsigned bins = 0;
    Attractor *attractor = NULL;
    RecurrencePlot *rp;
    std::string file_name;
    unsigned dimension = 0;
    unsigned delay = 0;
    double treshold = 0;
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

        if(std::string(argv[i]) == "--treshold" || std::string(argv[i]) == "-th")
            if(i + 1 < argc) {
                treshold = atof(argv[i + 1]);
                std::cerr << "Treshold: " << treshold << std::endl;
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
                from_file = true; 
            }
        if(std::string(argv[i]) == "--file_name" || std::string(argv[i]) == "-name")
                file_name = std::string(argv[i+1]);

    }
    if(!from_file)
        data = ReadStdin<double>();

    //Creating basics objects
    for (size_t i = 1; i < argc; ++i) {
        if(std::string(argv[i]) == "-ts" || std::string(argv[i]) == "--time_series"){
            time_series = new  TimeSeries(data, column);
        }
        if(std::string(argv[i]) == "-att" || std::string(argv[i]) == "--attractor"){
            attractor = new  Attractor(data);
        }
    }

    //Using time series
    for (size_t i = 1; i < argc; ++i)
    {
        if(std::string(argv[i]) == "-att_from_ts"){
            if(time_series) {
                attractor = new Attractor(*time_series, dimension, delay);
                std::cerr << ">> Conjurating Attractor From Time Series" << std::endl;
            }
        }
    }
    //Using attractor
    for (size_t i = 1; i < argc; ++i)
    {
        if(std::string(argv[i]) == "-rp_from_att"){
            if(attractor){
                std::cerr << ">> Conjurating Recurrence Plot From Attractor" << std::endl;
                rp = new RecurrencePlot(*attractor, treshold);
                if( (i + 1) <= argc && (std::string(argv[i + 1]) == "--print" || std::string(argv[i + 1]) == "-p"))
                    rp->PrintOnScreen();
            }
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

        if((std::string(argv[i]) == "--FalseNearestNeighbors") || (std::string(argv[i]) == "-fnn")){
            std::cerr << ">> Nearest Neighbors Max Dimension: " << dimension  << std::endl; 
            std::cerr << ">> Delay: " << delay  << std::endl; 
            std::cerr << ">> Treshold: " << treshold << std::endl; 
            std::vector<unsigned> nff =  FalseNearestNeighbors(*time_series, delay, dimension, treshold, false);
            for (size_t i = 0; i < nff.size(); ++i)
                std::cout << i + 1 << " " << nff[i] << std::endl; 
        } 

        if((std::string(argv[i]) == "--patterns_measures") || (std::string(argv[i]) == "-pm")) {
            if(rp){
                burn::Paint(*rp,0,0,0);
                RecurrenceAnalytics analytics(*rp);
                std::cout << file_name  << " "; 
                std::cout << analytics.RR()  << " "; 
                std::cout << analytics.DET()  << " "; 
                std::cout << analytics.LAM()  << " "; 
                std::cout << analytics.RATIO()  << " "; 
                std::cout << analytics.L()  << " "; 
                std::cout << analytics.TT()  << " "; 
                //std::cout << analytics.LMax()  << " "; 
                //std::cout << analytics.VMax()  << " "; 
                //std::cout << analytics.DIV()  << " "; 
                //std::cout << analytics.ENTR()  << " "; 
                //std::cout << analytics.TREND()  << " "; 
                std::cout << analytics.NumberOfDiagonals()  << " "; 
                std::cout << analytics.NumberOfVerticals()  << " "; 
                std::cout << analytics.HitPercentage()  << " "; 
                std::cout << analytics.NumberOfUnknown()  << " "; 
                std::cout << analytics.NumberOfHorizontals()  << " "; 
                if(time_series){
                    Mean<double> mean;
                    StDeviation<double> std;
                    for (size_t i = 0; i < time_series->size(); ++i)
                    {
                        mean((*time_series)[i]);
                        std((*time_series)[i]);
                    }
                    std::cout << mean  << " "; 
                    std::cout << std << " "; 
                }
                std::cout << std::endl; 
            }
            
        }
        /*
           if((std::string(argv[i]) == "--cross_correlation") && (i+2 < argc)){
           TimeSeries ts1(std::string(argv[i+1]));
           if(i + 2 < argc && std::string(argv[i+2]) == "--normalize" )
           normalize = MutualInformation(ts,0); 
           std::cout << "#delay mutual_info" << std::endl;
           for (size_t i = 0; i < 0.01*ts.Size(); ++i)
           {
           std::cout << i << " " << MutualInformation(ts,i) / normalize << std::endl; 
           }

           } 
           */
    }
    delete time_series;
    delete attractor;
    return 0; 
}


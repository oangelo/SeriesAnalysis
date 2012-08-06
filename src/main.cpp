#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <sstream>

#include "statistics.h"
#include "time_series.h"
#include "chaos.h"
#include "recurrence_plot.h"
#include "recurrence_analytics.h"

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
    TimeSeries *time_series = NULL;
    double rows = 0;
    unsigned bins = 0;
    Attractor *attractor = NULL;
    RecurrencePlot *rp;
    std::string file_name;
    //###########################################################################################
    //                                        Setting Data Up
    //###########################################################################################
    for (size_t i = 1; i < argc; ++i)
    {
        if( std::string(argv[i]) == "-n")
            if(i + 1 < argc) 
                rows = atoi(argv[i + 1]);

        if( std::string(argv[i]) == "--bins")
            if(i + 1 < argc) 
                bins = atoi(argv[i + 1]);

        if(std::string(argv[i]) == "-ts"){
            time_series = new  TimeSeries(std::string(argv[i+1]), rows);
            file_name = std::string(argv[i+1]);
            std::cout << "# Reading file:" << file_name  << std::endl;
        }
        if(std::string(argv[i]) == "-att"){
            attractor = new  Attractor(std::string(argv[i+1]));
            file_name = std::string(argv[i+1]);
            std::cout << "# Reading file:" << file_name  << std::endl;
 
        }

    }
    //Using time series
    for (size_t i = 1; i < argc; ++i)
    {
        if(std::string(argv[i]) == "-at_from_ts"){
            attractor = new Attractor(*time_series, atoi(argv[i + 1]), atoi(argv[i + 2]));
            std::cout << "# Conjurating Attractor From Time Series" << std::endl;
        }
    }
    //Using attractor
    for (size_t i = 1; i < argc; ++i)
    {
        if(std::string(argv[i]) == "-rp_from_at"){
            if(attractor){
                std::cout << "# Conjurating RecurrencePlot From Attractor" << std::endl;
                rp = new RecurrencePlot(*attractor, atof(argv[i + 1]));
                if( (i + 1) < argc && std::string(argv[i + 2]) == "print")
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

            std::cout << "#delay mutual_info" << std::endl;
            for (size_t i = 0; i < 0.01 * time_series->size(); ++i)
                std::cout << i << " " << MutualInformation(*time_series, i, bins) / normalize << std::endl; 
        } 
        if((std::string(argv[i]) == "--patterns_measures") || (std::string(argv[i]) == "-pm"))
            if(rp){
                Paint(*rp,0,0,0);
                RecurrenceAnalytics analytics(*rp);
                std::cout << file_name  << " "; 
                std::cout << analytics.RR()  << " "; 
                std::cout << analytics.DET()  << " "; 
                std::cout << analytics.LAM()  << " "; 
                std::cout << analytics.RATIO()  << " "; 
                std::cout << analytics.L()  << " "; 
                std::cout << analytics.TT()  << " "; 
                std::cout << analytics.LMax()  << " "; 
                std::cout << analytics.VMax()  << " "; 
                std::cout << analytics.DIV()  << " "; 
                std::cout << analytics.ENTR()  << " "; 
                //std::cout << analytics.TREND()  << " "; 
                std::cout << std::endl; 
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


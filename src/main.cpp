#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <sstream>

#include "statistics.h"
#include "time_series.h"
#include "chaos.h"
#include "recurrence_plot.h"

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
        RecurrencePlot rp(att_data,mean+10*std,1000);	
        std::string out=result_path.str();
        dados.open(out.c_str());
        //dados << "#RR= " << rp.RR() << std::endl; 
        //dados << "#DET= " << rp.DET() << std::endl;
        //dados << "#L= " << rp.L() << std::endl;
        for (unsigned k = 0; k < rp.Size(); k++)
            for (unsigned j = 0; j < rp.Size(); j++)
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
int main() {
    /*
       std::string file_name[14]={"cassia","daniel","fernando","gabriel",
       "julio","juscelino","leandro","maciel","maurilio","pedrod",
       "pedrol","renata","suzane","thiago"};

       std::string second_f="pos1";
       for(unsigned i=0;i<14;i++){   
       std::stringstream file_path;
       file_path << "to_analyse/rr-remo/" << second_f <<"/" << file_name[i]; 
       time_series teste(file_path.str());
       std::stringstream file;
       file << "./results/" << second_f << "/" << file_name[i];

       std::ofstream dados;
       int TAU_MAX = 50;
       std::string out=file.str();
       dados.open(out.c_str());
       for (int i = 0; i < TAU_MAX; i++) {
       dados << i << " " << mutual_information(teste, i) << endl;
       }
       dados.close();

       }
       */

    /*std::string file_name[14]={"cassia","daniel","fernando","gabriel",
      "julio","juscelino","leandro","maciel","maurilio","pedrod",
      "pedrol","renata","suzane","thiago"};

      std::string second_f="pos1";
      for(unsigned i=0;i<14;i++){   
      std::stringstream file_path;
      file_path << "to_analyse/rr-remo/" << second_f <<"/" << file_name[i]; 
      time_series teste(file_path.str());
      std::stringstream file;
      file << "./results/" << second_f << "/" << file_name[i];

      std::ofstream dados;
      int N = 7000;
      int tau = 15;
      int dmax = 8;
      double R_t = 12.0;
      double fnn_list[dmax]; // number of false neigh for each dimension
      false_nearest_nei(teste,tau,dmax,R_t,fnn_list,TRUE);

      std::string out=file.str();
      dados.open(out.c_str());
      for (int i = 0; i < dmax; i++) {
      dados << i <<"  "<<  fnn_list[i]/N << std::endl;
      }
      dados.close();

      }
      */

    /*
    //Dados do pos1 da cassia = do pedrol, checar!!!
    std::string file_name[14]={"cassia","daniel","fernando","gabriel",
    "julio","juscelino","leandro","maciel","maurilio","pedrod",
    "pedrol","renata","suzane","thiago"};

    std::string second_f="pre1";
    for(unsigned i=0;i<14;i++){   
    std::stringstream file_path;
    file_path << "to_analyse/rr-remo/" << second_f <<"/" << file_name[i]; 
    time_series teste(file_path.str());
    std::stringstream file;
    file << "./results/" << second_f << "/" << file_name[i];
    std::ofstream dados;

    Attractor att_data(teste,5,15);
    recurrence_plot rp(att_data);	
    std::string out=file.str();
    dados.open(out.c_str());
    for (int k = 0; k < att_data.size(); k++)
    for (int j = 0; j < att_data.size(); j++)
    if(rp.get(k,j)==1)
    dados << k <<"  "<<  j << std::endl;

    dados.close();

    }
    //*/

    rp_files("to_analyse/dp_ns/","results/dp_ns/");
    rp_files("to_analyse/dp_s/","results/dp_s/");
    rp_files("to_analyse/rossler/","results/rossler/");
    //mutual_info("to_analyse/rr_posicoe/","results/rr_posicoe/");
    //NN("to_analyse/rr_posicoe/","results/rr_posicoe_d/");
    //rp_ts_files("to_analyse/rr_posicoe/","results/rr_posicoe_rp/");
    /*    std::ofstream dados;
          Attractor att_data("to_analyse/rossler/rosler_series_f3_norbet_sstep");
          recurrence_plot rp(att_data,0,5);	
          dados.open("rp.out");
          for (int k = 0; k < rp.size(); k++)
          for (int j = 0; j < rp.size(); j++)
          if(rp.get(k,j)==1)
          dados << k <<"  "<<  j << std::endl;

          dados.close();
          */  
    return 0; 
}


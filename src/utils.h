#ifndef UTILS_H_
#define UTILS_H_

#include <fstream>
#include <string>
#include <cstring>
#include <math.h>
#include <sys/types.h>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <dirent.h>
#include <errno.h>
#include <cassert> 
#include <iostream> 
/*
 * calculate de eclidian distance between vec_a and vec_b.
 * THEY MUST HAVE THE SAME SIZE (dim)
 */
double EuclideanDistance(const double *vec_a,const double *vec_b,int dim);
double EuclideanDistance(std::vector<double> vec_a,std::vector<double> vec_b);
/*
 * Calculate the log in any basis.
 */
double u_log(double value,double base);

void file_counter(std::string file_name,unsigned &lines,unsigned &columns);

std::vector<std::string> ls(std::string dir);

bool mkdir(std::string path);

bool rm(std::string file);

template<class type>
std::vector< std::vector<type> >  ReadFile(std::string file_name){
    std::vector< std::vector<type> > data;
    std::string line;
    std::ifstream myfile(file_name.c_str());
    if(myfile.is_open())
    {
        while (myfile.good())
        {
            std::vector<type> values;
            getline(myfile, line);
            if((line.front() != '#') && !line.empty() ){
                std::stringstream line_stream(line);
                type value;
                while(line_stream >> value)
                    values.push_back(value);
                data.push_back(values);
            }

        }

        myfile.close();
    }
    else std::cout << "Unable to open file" << std::endl; 
    return data;
}

//data[line][column]
template<class type>
    std::vector< std::vector<type> >  ReadStdin(){
    std::vector< std::vector<type> > data;
    std::string line;
        while (std::cin)
        {
            std::vector<type> values;
            getline(std::cin, line);
            if((line.front() != '#') && !line.empty() ){
                std::stringstream line_stream(line);
                type value;
                while(line_stream >> value)
                    values.push_back(value);
                data.push_back(values);
            }

        }
    return data;
}
#endif /* UTILS_H_ */

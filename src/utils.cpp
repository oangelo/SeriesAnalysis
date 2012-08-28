/*
 * Utilities.cpp
 *
 *  Created on: Feb 23, 2010
 *      Author: gralha
 */

#include "utils.h"

double EuclideanDistance(const double *vec_a,const double *vec_b,int dim)
{
    double sum = 0.0;
    for(int i=0;i<dim;i++){
        sum += (vec_a[i]-vec_b[i])*(vec_a[i]-vec_b[i]);
    }
    return(sqrt(sum));
}

double EuclideanDistance(std::vector<double> vec_a,std::vector<double> vec_b){
    double sum = 0.0;
    for(unsigned i=0;i<vec_a.size();i++){
        sum += (vec_a[i]-vec_b[i])*(vec_a[i]-vec_b[i]);
    }
    return(sqrt(sum));
}

double u_log(double value,double base)
{
    return(log(value)/log(base));
}

void file_counter(std::string file_name,unsigned &lines,unsigned &columns){
    std::vector<unsigned> counter;
    std::string line;
    std::ifstream myfile(file_name.c_str());
    if(myfile.is_open())
    {
        while (myfile.good())
        {
            getline(myfile, line);
            unsigned spaces = 0; 
            if((line.front() != '#') && !line.empty() ){
                for(unsigned i=0; i!=line.size(); ++i)
                    spaces += (line.at(i) == '\t');
                for(unsigned i=0; i != line.size(); ++i)
                    spaces += (line.at(i) == ' ');
                    counter.push_back(spaces);
            }

        }
        myfile.close();
    }
    else std::cout << "Unable to open file" << std::endl; 
    columns = counter[0]+1;
    lines = counter.size();

}



std::vector<std::string> ls(std::string dir)
{
    std::vector<std::string> files;
    DIR *dp;
    struct dirent *dirp;
    if((dp  = opendir(dir.c_str())) == NULL) {
        std::cout << "Error(" << errno << ") opening " << dir << std::endl;
        //return errno;
    }

    while ((dirp = readdir(dp)) != NULL) {
        if(std::string(dirp->d_name) != "." && std::string(dirp->d_name) != "..")  
            files.push_back(std::string(dirp->d_name));
    }
    closedir(dp);
    return files;
}

bool mkdir(std::string path){
    std::stringstream ss;
    ss << "mkdir -p " << path;
    if(system ((ss.str()).c_str())==0)
        return(1);
    else
        return(0);
}


bool rm(std::string file){
    std::stringstream ss;
    ss << "rm " << file;
    if(system ((ss.str()).c_str())==0)
        return(1);
    else
        return(0);
}



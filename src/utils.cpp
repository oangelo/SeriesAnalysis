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

double u_log(double value,double base)
{
    return(log(value)/log(base));
}

void file_counter(std::string file_name,unsigned &lines,unsigned &columns){
    std::string line;
    unsigned counter=0;
    unsigned spaces=0;
    std::ifstream myfile(file_name.c_str());
    if(myfile.is_open())
    {
        getline (myfile,line);
        counter++;
        for(unsigned i=0; i!=line.size(); ++i)
            spaces+=( line.at(i)=='\t');
        for(unsigned i=0; i!=line.size(); ++i)
            spaces+=( line.at(i)==' ');
        while ( myfile.good() )
        {
            getline (myfile,line);
            if(line!=""){counter++;}
        }
        myfile.close();
    }
    else std::cout << "Unable to open file"; 
    columns=spaces+1;
    lines=counter;

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




NePairs::NePairs() : pairs(){} 

const bool NePairs::push_back(unsigned i,unsigned j){
    bool exist=0;
    for(unsigned cont=0;cont<pairs.size();cont++)
        if(pairs[cont][0]==i && pairs[cont][1]==j)
            exist=1;
    if(!exist){
        std::vector<unsigned> aux(2);
        aux[0]=i;
        aux[1]=j;
        pairs.push_back(aux);
    }    
    return !exist;
};
const bool NePairs::push_back(std::vector<unsigned> aux){
    bool exist=0;
    for(unsigned cont=0;cont<pairs.size();cont++)
        if(pairs[cont][0]==aux[0] && pairs[cont][1]==aux[1])
            exist=1;
    if(!exist)
        pairs.push_back(aux);    
    return !exist;
};

const std::vector<unsigned> NePairs::get_pair(unsigned i) const{
    return(pairs[i]);
}

const std::vector<unsigned> NePairs::Take(){
    std::vector<unsigned> buffer(pairs[0]);
    pairs.erase(pairs.begin());
    return(buffer);
};

const unsigned NePairs::Size() const{
    return(pairs.size());
};  

void NePairs::push_back(NePairs foo){
    for(unsigned counter=0;counter<foo.Size();counter++)
        this->push_back(foo.get_pair(counter));
}

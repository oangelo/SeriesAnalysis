#include "time_series.h"

time_series::time_series(double const *data,size_t size)
{
    for (unsigned i = 0 ; i < size; i++){
        _data.push_back(data[i]);
    }
}

time_series::~time_series()
{
	_data.erase(_data.begin(),_data.end());
}

double& time_series::operator[](const int &aux)
{
	if( aux < 0 || ((unsigned int)aux) >= _data.size()){
		throw Index_error("Time series index out of range");
	}
    return _data[aux];
}

double time_series::mean()
{
	double mean =0;
	for(unsigned i=0;i<_data.size();i++){
		double x = _data[i];
		/*see wikipedia:
		http://en.wikipedia.org/wiki/Algorithms_for_calculating_variance
		*/
		mean = (mean*i+x)/(i+1);
	}
	return(mean);
}

double time_series::var()
{
	double mean_new=0,mean_old=0,var=0;
	for(unsigned i=0;i<_data.size();i++){
		double x = _data[i];
		/*see wikipedia:
		http://en.wikipedia.org/wiki/Algorithms_for_calculating_variance
		*/
	    mean_new = mean_old + (x-mean_old)/(i+1);
	    var = (i*var+(x-mean_new)*(x-mean_old))/(i+1);
	    mean_old=mean_new;
	}
	return(var);
}

double time_series::std()
{
	return(sqrt(this->var()));
}

double time_series::max()
{
	return(*(std::max_element( _data.begin(), _data.end() )) );
}

double time_series::min()
{
	return(*(std::min_element( _data.begin(), _data.end() )) );
}

unsigned int time_series::size()
{
	return(_data.size());
}

time_series::time_series(std::string file_name){
  std::ifstream indata;
  double aux;
  std::vector< std::vector<double> > data_value;
  
  indata.open(file_name.c_str()); // opens the file
  if(!indata) { // file couldn't be opened
      std::cerr << "Error: file could not be opened. file:" << file_name << std::endl;
      exit(1);
  }
  
  indata >> aux;
  _data.push_back(aux);
  unsigned cont=0;
  while ( !indata.eof() ) { // keep reading until end-of-file
      indata >> aux;
      _data.push_back(aux);
      cont++;
  }
  indata.close();
}

void time_series::print(std::string file_name){
  std::ofstream data;
  data.open (file_name.c_str());
  for(unsigned i=0;i<_data.size();i++){
    data << _data[i] << std::endl;	
  }
  data.close();
}
#include "time_series.h"

TimeSeries::TimeSeries(double const * const p_data,size_t size) : data(size)
{
    for (unsigned i = 0 ; i < size; i++){
        this->data[i]=p_data[i];
    }
}

TimeSeries::TimeSeries(const std::vector<double> & data) : data(data)
{
}

TimeSeries::TimeSeries(std::string file_name):data()
{ 
    std::ifstream indata;
    double aux;
    std::vector< std::vector<double> > data_value;
    indata.open(file_name.c_str()); // opens the file
    if(!indata)
    {
        std::cerr << "Error: file could not be opened. file:" << file_name << std::endl;
        exit(1);
    }
    indata >> aux;
    data.push_back(aux);
    while ( !indata.eof() ) { // keep reading until end-of-file
        indata >> aux;
        data.push_back(aux);
    }    
    indata.close();
}

TimeSeries::~TimeSeries()
{
}

const double& TimeSeries::operator[](const int &aux) const
{
    assert( aux > 0 || ((unsigned int)aux) <= data.size());
    return data[aux];
}

double TimeSeries::Mean() const
{
    double mean =0;
    for(unsigned i=0;i<data.size();i++){
        double x = data[i];
        /*see wikipedia:
http://en.wikipedia.org/wiki/Algorithms_for_calculating_variance
*/
        mean = (mean*i+x)/(i+1);
    }
    return(mean);
}

double TimeSeries::Var() const
{
    double mean_new=0,mean_old=0,var=0;
    for(unsigned i=0;i<data.size();i++){
        double x = data[i];
        /*see wikipedia:
http://en.wikipedia.org/wiki/Algorithms_for_calculating_variance
*/
        mean_new = mean_old + (x-mean_old)/(i+1);
        var = (i*var+(x-mean_new)*(x-mean_old))/(i+1);
        mean_old=mean_new;
    }
    return(var);
}

double TimeSeries::Std() const
{
    return(sqrt(this->Var()));
}

double TimeSeries::Max() const
{
    return(*(std::max_element( data.begin(), data.end() )) );
}

double TimeSeries::Min() const
{
    return(*(std::min_element( data.begin(), data.end() )) );
}

unsigned int TimeSeries::Size() const
{
    return(data.size());
}

void TimeSeries::Print(std::string file_name) const
{
    std::ofstream file;
    file.open (file_name.c_str());
    for(unsigned i=0;i<data.size();i++){
        file << data[i] << std::endl;	
    }
    file.close();
}



double Entropy(TimeSeries& ts){
	Histogram1D hist(ts.Min(),ts.Max()+EPSILON,sqrt(ts.Size()));
	for (unsigned i = 0; i < ts.Size();i++){
		hist(ts[i]);
	}
	double sum = 0;
	for (size_t i = 0 ; i < hist.Size(); i++){
		if(hist[i] > 0){ 
            double probability = static_cast<double>(hist[i])/hist.Sum();
            sum -= probability*(log(probability)/log(2));
        }
	}
	return (sum);
}

double AutoCorrelation(TimeSeries& ts,unsigned tau)
{
	double correlation = 0, mean;
	mean = ts.Mean();
	double var_tau = 0.0;
	for (unsigned i = 0; i < ts.Size()-tau; i++){
		correlation += (ts[i + tau] - mean) * (ts[i] - mean);
		var_tau += pow(ts[i] - mean, 2);
	}

	correlation = correlation / var_tau;

	return(correlation);
}

double MutualInformation(TimeSeries& ts,unsigned tau)
{
	int N = ts.Size()-tau;
	double sum = 0.0;
	Histogram1D pA(ts.Min(),ts.Max()+EPSILON,sqrt(ts.Size()));
	Histogram1D pB(ts.Min(),ts.Max()+EPSILON,sqrt(ts.Size()));
	int n_bins = pA.Size();
	int P_cond[n_bins][n_bins];
	int i=0,j=0;
	// initi the 2D histogram
	for(i=0;i<n_bins;i++){
		for(j=0;j<n_bins;j++){
			P_cond[i][j]=0;
		}
	}

    //increment the histograms
	for (i=0;i<N;i++){
		P_cond[pA(ts[i])][pB(ts[i+tau])]++;
	}
	//counting the 2D-histogram
	double logtwo = 1.0/log(2);
	for(i=0; i < n_bins; i++){
		if(pA[i] > 0){
			for(j=0; j < n_bins; j++){
				if(P_cond[i][j] > 0 && pB[j] > 0){
					double pij = ((double)P_cond[i][j] / N);
					double argv = pij / ((pA[i] * pB[j]) / pA.Sum()*pB.Sum());
					sum += pij*(log(argv)*logtwo);
				}
			}
		}
	}

	return(sum);
}


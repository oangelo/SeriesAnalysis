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

TimeSeries::TimeSeries(const std::vector<std::vector<double>> & many_data, unsigned column) : data()
{
    for(auto vec: many_data)
        data.push_back(vec[column]);
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

unsigned int TimeSeries::size() const
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



double Entropy(TimeSeries& ts, unsigned bins, double min, double max){
    pstatistics::Histogram histogram(bins, min, max);
	for (unsigned i = 0; i < ts.size();i++){
		histogram(ts[i]);
	}
	double sum = 0;
	for (size_t i = 0 ; i < histogram.BinsAmount(); i++){
		if(histogram[i].second > 0){ 
            double probability = static_cast<double>(histogram[i].second)/histogram.SamplesAmount();
            sum -= probability*(log(probability)/log(2));
        }
	}
	return (sum);
}

double AutoCorrelation(TimeSeries& ts, unsigned tau)
{
	double correlation = 0, mean;
	mean = ts.Mean();
	for (unsigned i = 0; i < ts.size()-tau; i++){
		correlation += (ts[i + tau] - mean) * (ts[i] - mean);
	}
    correlation /= ts.size()-tau;
	correlation /=  pow(ts.Std(),2);
	return correlation;
}

double CrossCorrelation(TimeSeries& ts1,TimeSeries& ts2)
{
	double correlation = 0, mean1, mean2;
	mean1 = ts1.Mean();
	mean2 = ts2.Mean();
	for (unsigned i = 0; i < ts1.size(); i++){
		correlation += (ts1[i] - mean1) * (ts2[i] - mean2);
	}
    correlation /= ts1.size();
    correlation /= (ts1.Std() * ts2.Std());
	return(correlation);
}

double MutualInformation(TimeSeries& ts, unsigned tau, unsigned bins)
{
	int N = ts.size() - tau;
	Histogram1D pA(ts.Min(), ts.Max()+EPSILON, bins);
	Histogram1D pB(ts.Min(), ts.Max()+EPSILON, bins);
	int n_bins = pA.size();
	int P_cond[n_bins][n_bins];
	int i = 0, j = 0;
	// initi the 2D histogram
	for(i=0; i < n_bins; i++){
		for(j=0;j<n_bins;j++){
			P_cond[i][j]=0;
		}
	}

    //increment the histograms
	for (i=0; i < N; i++){
		P_cond[pA(ts[i])][pB(ts[i+tau])]++;
	}
    /*
    for (size_t i = 0; i < n_bins; ++i)
    {
        for (size_t j = 0; j < n_bins; ++j)
        {
            std::cout << P_cond[i][j] << " ";
        } 
        std::cout << std::endl;
    }
    */
	//counting the 2D-histogram
	double logtwo = 1.0 / log(2);
	double sum = 0.0;
	for(i=0; i < n_bins; i++){
			for(j=0; j < n_bins; j++){
				if(pB[j] > 0 && pA[i] > 0){
					double pij = static_cast<double>(P_cond[i][j]) / N;
					double pa_i = static_cast<double>(pA[i]) / pA.Sum();
					double pb_j = static_cast<double>(pB[j]) / pB.Sum();
                    if(pij != 0)
					    sum += pij * (log(pij / (pa_i * pb_j)) * logtwo);
                    //std::cout << sum <<  " " <<  pij * (log(pij / (pa_i * pb_j)) * logtwo )<<    std::endl;
				}
			}
	}

	return(sum);
}

void MovingAverage(TimeSeries& ts, unsigned window){
    pstatistics::Mean mean;
    for (size_t i = 0; i < window / 2; ++i)
    {
        mean(ts[i]);
        std::cout << mean << std::endl;
    }
    for (size_t i = window / 2; i < ts.size() - window / 2; ++i){
        pstatistics::Mean moving_average;
        for (size_t j = i; j < i+window; ++j) {
            moving_average(ts[j]);
        }
        std::cout << moving_average << std::endl;
    }
    pstatistics::Mean mean_end;
    for (size_t i = ts.size() - window / 2; i < ts.size(); ++i){

    }
}


TimeSeries Surrogate(const TimeSeries& ts){
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, ts.size() - 1);
    std::vector<double> aux;
    for(size_t i(0); i < ts.size(); ++i)
        aux.push_back(ts[i]);
    for(size_t i(0); i < 2*ts.size(); ++i){
        int from(dis(gen));
        int to(dis(gen));
        double aux_value(aux[to]);
        aux[to] = aux[from];
        aux[from] = aux_value;
    }
    return TimeSeries(aux);
}

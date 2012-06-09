#include "statistics.h"


Histogram1D::Histogram1D(const double min, const double max, const unsigned number_of_bins)
    : max(max), min(min), histogram(number_of_bins,0.0),bin_width(static_cast<double>(max-min)/number_of_bins){
    if(min >= max)
    {
        std::cerr << " Max < min, was not possible to construct the histogram." << std::endl;
        throw value_erro;
    }
    if(number_of_bins == 0){
        std::cerr << "Number of beans must be bigger then 0, was not possible to construct the histogram." << std::endl;
        throw value_erro;
    }
}

//set values on the histogram
size_t Histogram1D::operator()(const double value) {
    histogram[HashFunction(value)]++;
    return HashFunction(value);
}

//get values from histogram
unsigned Histogram1D::operator[](const size_t index) const{
    return histogram[index];
}

double Histogram1D::Mean() const{
    double bin_w = bin_width,min_val = min;
    unsigned counter = 0;
    return std::accumulate(histogram.begin(),histogram.end(),0.0,[bin_w,&counter,min_val](double sum,unsigned value ){
                sum += value*(min_val + bin_w / 2 + counter*bin_w); 
                ++counter;
                return sum;
            }) / std::accumulate(histogram.begin(),histogram.end(),0.0); 
}

double Histogram1D::Std() const{
    double bin_w(bin_width),min_val(min);
    unsigned counter(0);
    double mean(Mean());
    return sqrt (std::accumulate(histogram.begin(),histogram.end(),0.0,[bin_w,&counter,min_val,mean](double sum,unsigned value ){
                sum += value*pow(mean - (min_val + bin_w / 2 + counter*bin_w),2); 
                ++counter;
                return sum;
            }) / std::accumulate(histogram.begin(),histogram.end(),0.0));
}

size_t Histogram1D::Max() {
    std::vector<unsigned>::iterator result = std::max_element(histogram.begin(),histogram.end()); 
    return std::distance(histogram.begin(),result);
}

size_t Histogram1D::Min() {
    std::vector<unsigned>::iterator result = std::min_element(histogram.begin(),histogram.end()); 
    return std::distance(histogram.begin(),result);
} 

unsigned Histogram1D::Sum() const{
    return std::accumulate(histogram.begin(), histogram.end(), 0);
}

std::pair<double,double> Histogram1D::BinRange(size_t index){
    return std::pair<double, double>(min+bin_width*index,min+bin_width*(index+1));
}

size_t Histogram1D::Size() const {
    return histogram.size();
}

size_t Histogram1D::HashFunction(double value) const{
    if(value >= max || value < min){
        std::cerr << "Is not possible to insert " << value << ", value is out of the range of the histogram." << std::endl;
        throw value_erro;
    }
    return static_cast<size_t>((value-min) / bin_width); 
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


/*
 * File:   Histogram.cpp
 * Author: angelo
 *
 * Created on September 7, 2009, 6:50 PM
 */


#include "statistics.h"

Histogram::Histogram(int n, double xmin, double xmax) {
	//checking error
    if((xmax-xmin) < EPSILON){
		throw Value_error("xmax must be different from xmin");
	}
    //Alocating Memory for the histogram
    h = gsl_histogram_alloc(n);
    gsl_histogram_set_ranges_uniform(h,xmin, xmax);
}

Histogram::Histogram(double std, size_t size, double xmin,double xmax){
	//checking error
	if((xmax-xmin) < EPSILON){
		throw Value_error("xmax must be different from xmin");
	}
	//Alocating Memory for the histogram
	double	bin_size = (3.5*std) / pow(size,1.0/3.0); //Scott's choice
	unsigned n = ceil((xmax-xmin)/bin_size);
    h = gsl_histogram_alloc(n);
	gsl_histogram_set_ranges_uniform(h,xmin, xmax);
}

Histogram::Histogram(Histogram& orig) {
    h = gsl_histogram_alloc(orig.n_bins());
    gsl_histogram_set_ranges_uniform(h,orig.range_min(), orig.range_max());
    //get the bin size
    double bin = (orig.range_max() - orig.range_min()) / orig.n_bins();

    //counting how many elements each bin have
    for (unsigned int i = 0 ; i < orig.n_bins() ; i++){
        for(int j = 0 ; j < orig.get(i) ; j++){
            this->increment(orig.range_min() + ((i*bin) +  bin/2));
        }
    }
    this->sum();
}

Histogram::~Histogram() {
    //Free the histogram memory
    gsl_histogram_free(h);
}

double Histogram::operator[](const int & aux) {
    /*
     * This operator the probability of a bin
     */
	//std::cout << "sum :" << _sum_value << std::endl;
	if(aux<0 || ((unsigned)aux) > (n_bins()-1))
		throw Index_error("Histogram index out of range");

    return (get((size_t)aux) / _sum_value);
}

double Histogram::operator[](const double & aux) {
    /*
     * This operator the probability of a bin
     */
	size_t i;
	if(! ((aux >= this->h->range[0] ) && (aux < this->h->range[h->n])) ){
		throw Value_error("Histogram out of range");
	}

	gsl_histogram_find (h,aux,&i);
    return (get(i) / _sum_value);
}

//foi ideia do angelo, eu queria fazer um find(): Ass: gralha
int Histogram::operator()(const double &aux){
	size_t i;
	//std::cout << aux << " " << h->range[h->n] << std::endl;
	if(! ((aux >= this->h->range[0] ) && (aux < this->h->range[h->n])) ){
		throw Value_error("Histogram out of range");
	}

	gsl_histogram_find (h,aux,&i);
	return((int)i);
}

void Histogram::increment(double x) {
    /*
     * This function updates the histogram h by adding one (1.0) to the bin whose
     * range contains the coordinate x.
     */
	if( (x >= this->h->range[0] ) && (x < this->h->range[h->n]) ){
		gsl_histogram_increment(h, x);
		sum(); /*Recalculating the sum because we add a new value*/
	}
	else{
		throw Value_error("Histogram out of range");
	}
}

size_t Histogram::n_bins(void) {
    //acessing C structure pointer uses structure->attr
    return (h->n);
}

double Histogram::range_max(void) {
    //acessing C structure pointer uses structure->attr
    return (h->range[(int) h->n]);
}

double Histogram::range_min(void) {
    //acessing C structure pointer uses structure->attr
    return (h->range[0]);
}

double Histogram::get(const size_t & i) {
    return (gsl_histogram_get(h, i));
}

void Histogram::reset() {
    gsl_histogram_reset(h);
}


/*
 *Histogram Statistics**********************************************************
 */
double Histogram::max_val() {
    /*
     *This function returns the maximum value contained in the histogram bins.
     */
    return (gsl_histogram_max_val(h));
}

size_t Histogram::max_bin() {
    /*
     * This function returns the index of the bin containing the maximum value.
     * In the case where several bins contain the same maximum value the smallest
     * index is returned.
     */
    return (gsl_histogram_max_bin(h));
}

double Histogram::min_val() {
    /*
     * This function returns the minimum value contained in the histogram bins.
     */
    return (gsl_histogram_min_val(h));
}

size_t Histogram::min_bin() {
    /*
     * This function returns the index of the bin containing the minimum value.
     * In the case where several bins contain the same maximum value the smallest
     * index is returned.
     */
    return (gsl_histogram_min_bin(h));
}

double Histogram::mean() {
    /*
     * This function returns the mean of the histogrammed variable,
     * where the histogram is regarded as a probability distribution.
     * Negative bin values are ignored for the purposes of this calculation.
     * The accuracy of the result is limited by the bin width.
     */
    return (gsl_histogram_mean(h));
}

double Histogram::sigma() {
    /*
     * This function returns the standard deviation of the histogrammed variable,
     * where the histogram is regarded as a probability distribution.
     * Negative bin values are ignored for the purposes of this calculation.
     * The accuracy of the result is limited by the bin width.
     */
    return (gsl_histogram_sigma(h));
}

double Histogram::sum() {
    /*
     * This function returns the sum of all bin values. Negative bin values are
     * included in the sum.
     */
    _sum_value=gsl_histogram_sum(h);
    return (_sum_value);
}




double entropy(time_series& ts){


	Histogram hist(ts.std(),ts.size(),ts.min(),ts.max()+EPSILON);

	for (unsigned i = 0; i < ts.size();i++){
		//std::cout << i <<"inside:" << ts._data.back()<< std::endl;
		hist.increment(ts[i]);
	}
	//hist.increment(1.2);
	double sum = 0;

	for (int i = 0 ; i < (int)hist.n_bins(); i++){
		if(hist[i] > 0) sum -= hist[i]*(log(hist[i])/log(2));
		//std::cout << hist[i]<< std::endl;
	}
	return (sum);
	//*/
}

double auto_corr_func(time_series& ts,unsigned tau)
{
	double c=0,mean;
	mean = ts.mean();
    //N =  ts.size() - tau;
	double var_tau = 0.0;
	for (unsigned i = 0 ; i < ts.size()-tau; i++){
		c+=(ts[i+tau]-mean)*(ts[i]-mean);
		var_tau += (ts[i]-mean)*(ts[i]-mean);
	}

	c=c/(var_tau);
	//checking for nans and infs
	if(c!=c){
		throw Value_error("Nan or inf found. Check var() method.");
	}

	return(c);
}

double mutual_information(time_series& ts,unsigned tau)
{
	int N = ts.size()-tau;
	double sum=0.0;
	Histogram pA(ts.std(),ts.size(),ts.min(),ts.max()+EPSILON);
	Histogram pB(ts.std(),ts.size(),ts.min(),ts.max()+EPSILON);
	int n_bins = pA.n_bins();
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
		pA.increment(ts[i]);
		pB.increment(ts[i+tau]);
		P_cond[pA(ts[i])][pA(ts[i+tau])]++;
	}
	//counting the 2D-histogram
	double logtwo = 1.0/log(2);
	for(i=0;i<n_bins;i++){
		if(pA[i] > 0){
			for(j=0;j<n_bins;j++){
				if(P_cond[i][j] > 0 && pB[j] > 0){
					double pij = ((double)P_cond[i][j] / N);
					double argv = pij / (pA[i]*pB[j]);
					sum += pij*(log(argv)*logtwo);
				}
			}
		}
	}

	return(sum);
}


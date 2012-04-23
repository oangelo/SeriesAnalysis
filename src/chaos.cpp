#include "chaos.h"

Attractor::Attractor(time_series &ts, unsigned int D_e, unsigned int tau) {
	__D_e = D_e;
	__tau=tau;
	__data = create_lagged_array(ts, tau, __D_e);
	__n_vec = ts.size() - tau*(__D_e-1);
	__length_side();
}

Attractor::Attractor(double **data, unsigned int D_e, unsigned int n_vec) {
	__D_e = D_e;
	__tau=0;
	__data = new double* [n_vec];
	for(unsigned int i=0;i<n_vec;i++) __data[i] = NULL;
	for(unsigned int i=0;i<n_vec;i++) __data[i] = new double[D_e];
	for(unsigned int v=0;v<n_vec;v++){
		for(unsigned int d=0;d<D_e;d++){
			__data[v][d] = data[v][d];
		}
	}
	__n_vec = n_vec;
	__length_side();
}

Attractor::Attractor(std::string file_name){
	unsigned lines;
	unsigned columns;
	file_counter(file_name,lines,columns);
	__D_e=columns;
	__n_vec=lines;
	__tau=0;
	__data = new double* [__n_vec];
	for(unsigned i=0;i<__n_vec;i++) 
		__data[i] = NULL;
	for(unsigned i=0;i<__n_vec;i++) 
		__data[i] = new double[__D_e];
	std::ifstream indata;
	indata.open(file_name.c_str()); // opens the file
	if(!indata) { // file couldn't be opened
		std::cerr << "Error: file could not be opened. file:" << file_name << std::endl;
		exit(1);
	}
	for(unsigned i=0;i<__D_e;i++)
		indata >> __data[0][i];
	unsigned cont=1;
	for(unsigned j=0;j<__n_vec;j++) { // keep reading until end-of-file
		for(unsigned i=0;i<__D_e;i++)
			indata >> __data[cont][i];      
		cont++;
	}
	indata.close();
}

Attractor::Attractor( Attractor& att){
	__D_e = att.get_dim();
	__n_vec = att.size();
	__data = new double* [__n_vec];
	for(unsigned i=0;i<__n_vec;i++) 
		__data[i] = NULL;
	for(unsigned i=0;i<__n_vec;i++) 
		__data[i] = new double[__D_e];

	unsigned cont=0;
	for(unsigned j=0;j<__n_vec;j++) { // keep reading until end-of-file
		for(unsigned i=0;i<__D_e;i++)
			__data[cont][i]=att.get(j,i);      
		cont++;
	}
}

Attractor::~Attractor() {
	del_lagged_array(__data, __n_vec, __D_e);
}

/**************************************************************************/
/*Warning, box counting has problems of convergence************************/
/**************************************************************************/
std::vector<unsigned int> Attractor::Box_counting_D(double epsilon) {

	//unsigned cont;
	n_box.clear();

	//Searching the max an min of the atractor
	double max[__D_e];
	double min[__D_e];
	__find(min, max);
	//attractor larger side
	__epsilon =__length*epsilon;
	//Creating Auxiliary vectors to __recursive_for
	unsigned cont_loop[__D_e]; //This help us to know which loop is going on the moment.
	unsigned walker[__D_e]; //This vector will assume all the possible values
	for (unsigned i = 0; i < __D_e; i++) { //initializing arrays
		cont_loop[i] = 0; walker[i] = 0;
	}
	unsigned int max_loop[__D_e]; //Shape of the matrix one would like to span
	for (unsigned i = 0; i < __D_e; i++) {
		max_loop[i] = ceil((max[i] - min[i]) / __epsilon)+1;
	}
	//Calling the function __recursive_for that will span the space
	__recursive_for(__D_e, 0, cont_loop, walker, max_loop, min);

	return(n_box);


}

/******************************************************************************/
/*This function generates in a recursive manner many "for" inside "for"********/
/******************************************************************************/
void Attractor::__recursive_for(unsigned dimension, unsigned cont,
		unsigned* cont_loop, unsigned *walker,
		unsigned* max_loop, double *min) {

	for (unsigned i = 0; i < max_loop[cont]; i++) {
		//This "if" stacks the loops
		if (cont < dimension - 1) {
			cont_loop[cont]++;
			walker[cont] = cont_loop[cont] % max_loop[cont];
			__recursive_for(dimension, cont + 1, cont_loop, walker, max_loop, min);
		} else {
			//Do the last loop on the stack
			double vet[dimension];
			for (unsigned j = 0; j < dimension - 1; j++) {
				vet[j] = min[j] + __epsilon * walker[j];
			}
			vet[dimension - 1] = min[dimension - 1] + __epsilon*i;
			int n_points = __points_in_box(vet, __epsilon);
			if(n_points!=0){
				n_box.push_back(n_points);}
		}
	}
}

/******************************************************************************/
/*Counting how many points there are in the boxes******************************/
/******************************************************************************/
int Attractor::__points_in_box(double *coord, double epsilon) {
	unsigned d = 0, v = 0;
	unsigned count_d = 0;
	int count = 0;
	//box boundaries
	double max[__D_e];
	double min[__D_e];
	for (d = 0; d < __D_e; d++) { //init box boundaries with the target box
		max[d] = coord[d] + epsilon / 2;
		min[d] = coord[d] - epsilon / 2;
	}
	for (v = 0; v < __n_vec; v++) {//running in every attractors point
		count_d = 0;
		for (d = 0; d < __D_e; d++) {
			if ((__data[v][d] < max[d]) && (__data[v][d] >= min[d])) {
				count_d++;
			}
		}
		if (count_d == __D_e) {
			count++;
		}
	}
	return (count);
}

/******************************************************************************/
/*Searching for the max and min values of the atracor**************************/
/******************************************************************************/
void Attractor::__find(double *min, double *max) {
	unsigned d = 0, v = 0;
	for (d = 0; d < __D_e; d++) {
		max[d] = __data[0][d];
		min[d] = __data[0][d];
	}
	for (d = 0; d < __D_e; d++) {
		for (v = 1; v < __n_vec; v++) {
			if (max[d] < __data[v][d]) {
				max[d] = __data[v][d];
			} else {
				if (min[d] > __data[v][d]) {
					min[d] = __data[v][d];
				}
			}
		}
	}
}

void Attractor::__length_side(){


	double max[__D_e];
	double min[__D_e];
	//Searching the max an min of the atractor
	__find(min, max);
	for(unsigned i=1;i<__D_e;i++){
		if((max[i]-min[i]) > __length){
			__length=max[i]-min[i];
		}
	}

}

double Attractor::correlation_integral(double length_fraction){

	if(length_fraction>=1 || length_fraction<0){
		throw Value_error("epsilon must be between 1 and 0");
	}
	// box side as a fraction of largest object side
	__epsilon =__length*length_fraction;

	int count = 0;
	for (unsigned i=0; i < __n_vec; i++){
		for (unsigned j=0; j < __n_vec; j++){
			double dist = __dist_eucl(__data[i],__data[j],__D_e);
			if(dist  < __epsilon) count++;
		}
	}

	return ((double)count/(__n_vec*__n_vec) );
}

double Attractor::get_epsilon(){
	return(__epsilon);
}

double Attractor::get(unsigned vec, unsigned dim) {
	return (__data[vec][dim]);
}
void Attractor::get_vec(unsigned vec,double *p_vec){
	for (unsigned i=0; i < __D_e; i++)
		p_vec[i]=__data[vec][i];
}

double **create_lagged_array(time_series &ts,
		const unsigned int tau,
		const unsigned int dim)
{
	unsigned int n_vec=ts.size()-tau*(dim-1);
	unsigned int i,j;
	double **data = new double* [n_vec];
	for(i=0;i<n_vec;i++) data[i] = NULL;

	for(i=0;i<n_vec;i++) data[i] = new double[dim];

	for(i=0;i<n_vec;i++){
		for(j=0;j<dim;j++){
			data[i][j]=ts[i+j*tau];
		}
	}
	return(data);
}

void del_lagged_array(double** data,
		const unsigned int n_vec,
		const unsigned int dim)
{
	for(unsigned i=0;i<n_vec;i++){
		delete [] data[i];
	}
	delete [] data;
}

unsigned int __find_nearest(Attractor &data,unsigned index)
{
	double dist = FLT_MAX;
	unsigned nn = 0;
	double vec1[data.get_dim()],vec2[data.get_dim()];
	data.get_vec(index,vec2);
	for(unsigned i=0 ; i < data.size()-data.get_tau(); i++){
		data.get_vec(i,vec1);
		double aux = __dist_eucl(vec1,vec2,data.get_dim());
		if( (dist >= aux) && (i!=index) ) {
			dist = aux;
			nn = i;
		}
	}
	return(nn);
}
//*
unsigned int false_nearest_nei(time_series& ts,
		unsigned tau,
		unsigned int d_max,
		double Rt,
		double *fnn_list,
		bool SECOND_COND)
{

	unsigned int D = 1,count_NN=1,i,j;
	double std;
	std=ts.std();

	while(D <= d_max && (count_NN !=0) ){
		Attractor att(ts,D,tau);
		double vec_i[att.get_dim()],vec_j[att.get_dim()];
		count_NN=0;
		for(i=0;i< ts.size()-tau*D;i++){
			j = __find_nearest(att,i);
			//if(j<att.size()-tau*D){
			att.get_vec(i,vec_i);
			att.get_vec(j,vec_j);
			double dist = __dist_eucl(vec_i,vec_j,att.get_dim());
			double check = fabs(ts[i+tau*D]-ts[j+tau*D]) / dist;
			if(check > Rt || (SECOND_COND && check >= 4*std) ) count_NN++;
			//}
		}
		fnn_list[D-1] = count_NN;
		D++;
	}
	return(--D);
}

//m_poist is the size of one side of the mxm matrix
recurrence_plot::recurrence_plot(Attractor &ts_data,unsigned m_points,double dist_limit){
	__ts_data=new Attractor(ts_data);
	unsigned step=1;
	if(m_points!=0){
		__size=m_points;
		step= ts_data.size()/__size;
	}else{
		__size=__ts_data->size();
	}

	__data = new unsigned* [__size];
	for(unsigned int i=0;i<__size;i++) 
		__data[i] = NULL;
	for(unsigned int i=0;i<__size;i++) 
		__data[i] = new unsigned[__size];

	if(dist_limit==0){
		double mean=0;
		unsigned total_points=pow(__size,2);
		if(dist_limit==0){
			for(unsigned j = 0; j < ts_data.size(); j=j+step)
				for(unsigned i = 0; i < ts_data.size(); i=i+step){
					double dist=0;
					for(unsigned k = 0; k < ts_data.get_dim(); k++)
						dist+=pow(ts_data.get(i,k)-ts_data.get(j,k),2);
					dist=sqrt(dist);
					mean+=dist/total_points;
				}
		}

		generate(mean/20);
	}else{
		generate(dist_limit);
	}
}
recurrence_plot::recurrence_plot(unsigned** data,unsigned size){
	//to not broke the destructor
	double **aux;
	aux= new double* [1];
	__ts_data=new Attractor(aux,0,0);
	delete[] aux;
	//*****************************
	__size=size; 
	__data = new unsigned* [__size];
	for(unsigned int i=0;i<__size;i++) 
		__data[i] = NULL;
	for(unsigned int i=0;i<__size;i++) 
		__data[i] = new unsigned[__size];
	for(unsigned int i=0;i<__size;i++)
		for(unsigned int j=0;j<__size;j++)
			__data[i][j]=data[i][j];

}
recurrence_plot::~recurrence_plot(){
	for(unsigned i=0;i<__size;i++){
		delete [] __data[i];
	}
	delete [] __data;
	delete __ts_data;
}

unsigned recurrence_plot::get(unsigned i, unsigned j){
	return(__data[i][j]);
}
unsigned recurrence_plot::size(){ 
	return(__size);

}


ne_pairs recurrence_plot::burn(unsigned i,unsigned j){
	assert(i<__size-1 && j<__size-1 && i>0 && j>0);
	//store the position of burned points
	ne_pairs cluster;
	//store points to be burned, and that needs to check neigboards
	ne_pairs neigboards;
	neigboards.push_back(i,j);
	while(neigboards.size()!=0){
		unsigned x=(neigboards.get(0))[0];
		unsigned y=(neigboards.get(0))[1];
		cluster.push_back(neigboards.take());
		//search for neigboards
		//bulk
		if(x<__size-1 && y<__size-1 && x >0 && y>0  ){
			if(__data[x+1][y]==1 && !cluster.exist(x+1,y))
				neigboards.push_back(x+1,y);
			if(__data[x][y+1]==1 && !cluster.exist(x,y+1))
				neigboards.push_back(x,y+1);
			if(__data[x+1][y+1]==1 && !cluster.exist(x+1,y+1))
				neigboards.push_back(x+1,y+1);

			if(__data[x-1][y]==1 && !cluster.exist(x-1,y))
				neigboards.push_back(x-1,y);
			if(__data[x][y-1]==1 && !cluster.exist(x,y-1))
				neigboards.push_back(x,y-1);
			if(__data[x-1][y-1]==1 && !cluster.exist(x-1,y-1))
				neigboards.push_back(x-1,y-1);

			if(__data[x-1][y+1]==1 && !cluster.exist(x-1,y+1))
				neigboards.push_back(x-1,y-1);
			if(__data[x+1][y-1]==1 && !cluster.exist(x+1,y-1))
				neigboards.push_back(x-1,y-1);  
		}
	}

	return(cluster);
}

unsigned recurrence_plot::points_in_diagonals(){
	std::vector<unsigned>  length;
	diagonals(length);
	unsigned sum=0;
	for(unsigned j = 0; j < length.size(); j++)
		if(length[j]>3)
			sum+=length[j];
	return(sum);
}

double recurrence_plot::L(){
	std::vector<unsigned>  length;
	diagonals(length);
	unsigned sum=0,counter=0;
	for(unsigned j = 0; j < length.size(); j++)
		if(length[j]>3){
			sum+=length[j];
			counter++;
		}
	return(((double)sum)/counter);
}

unsigned recurrence_plot::diagonals(std::vector<unsigned> & length){
	ne_pairs burnt;
	for(unsigned j = 1; j < __size-1; j++)
		for(unsigned i = 1; i < __size-1; i++)
			if(__data[i][j]==1)
				if(!burnt.exist(i,j)){
					ne_pairs cluster(this->burn(i,j));
					burnt.push_back(cluster);  
					unsigned diag_length=diagonal_size(cluster);
					if(diag_length>1)
						length.push_back(diag_length);	
				}
	return(length.size());
}

unsigned recurrence_plot::diagonal_size(ne_pairs & cluster){
	unsigned max=0;
	for(unsigned counter=0;counter<cluster.size();counter++){
		std::vector< unsigned > aux(cluster.get(counter));
		unsigned length=1;
		bool exist=1;
		while(exist){
			exist=cluster.exist(aux[0]+length,aux[1]+length);
			if(exist)
				length++;
		}
		if(length>max)
			max=length;
	}
	return(max);
}

void recurrence_plot::generate(double limit){
	Attractor &ts_data=*__ts_data;
	double vec_i[ts_data.get_dim()],vec_j[ts_data.get_dim()];
	double step= ts_data.size()/__size;
	for(unsigned j = 0; j < __size; j++)
		for(unsigned i = 0; i < __size; i++){
			ts_data.get_vec(i*step,vec_i);
			ts_data.get_vec(j*step,vec_j);
			double dist = __dist_eucl(vec_i,vec_j,ts_data.get_dim());
			if(dist < limit){
				__data[i][j]=1;
			}else{
				__data[i][j]=0;
			}
		}
}
double recurrence_plot::DET(){
	unsigned count=0;
	for(unsigned j = 0; j < __size; j++)
		for(unsigned i = 0; i < __size; i++)
			if(__data[i][j]==1)
				count++;

	return(((double)points_in_diagonals())/count);

}

double recurrence_plot::RR(){
	unsigned count=0;
	for(unsigned j = 0; j < __size; j++)
		for(unsigned i = 0; i < __size; i++)
			if(__data[i][j]==1)
				count++;
	return(((double)count)/(__size*__size));

}

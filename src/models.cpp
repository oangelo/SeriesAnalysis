/*
 * models.cpp
 *
 *  Created on: Apr 20, 2010
 *      Author: gralha
 */

#include "models.h"


/*****************/
/*Class Generator*/
/*****************/

Generator::Generator(){}

Generator::~Generator(){}


double* Generator::logistic_map(double r, double x0,size_t size)
{
	double xt=x0;
	double* data = new double[size];
	for (unsigned i=0;i<size;i++) data[i]=0.0;
	//removing the transient
	for(int i=0;i<TRANSIENT_LOGISTIC_MAP;i++){
		xt=r*xt*(1-xt);
	}
	data[0]=xt;
	for(unsigned i=1;i<size;i++){
		data[i]=r*data[i-1]*(1-data[i-1]);
	}
	return(data);
}

double* Generator::henon_map(char coord,double a,double b, double x0,double y0,size_t size)
{
	double xt=x0,yt=y0,yt_1=y0,xt_1=x0;
	double* datax = new double[size];
	double* datay = new double[size];
	for (unsigned i=0;i<size;i++) {datax[i]=0.0;datay[i]=0.0;}
	//removing the transient
	for(int i=0;i<TRANSIENT_LOGISTIC_MAP;i++){
		xt=1+yt_1-a*(xt_1)*(xt_1);
		yt=b*xt_1;
		xt_1=xt; yt_1=yt;
	}

	for(unsigned i=0;i<size;i++){
		xt=1+yt_1-a*(xt_1)*(xt_1);
		yt=b*xt_1;
		xt_1=xt; yt_1=yt;
		datax[i]=xt; datay[i]=yt;
	}
	switch(coord){
			case 'x':
			{
				delete datay; return(datax);
			}
			break;
			case 'y':
			{
				delete datax; return(datay);
			}
			break;
			default:
			{
				throw(Value_error("Coodinate must be x or y"));
			}
		}

		return(NULL);

}

/*We took out the time because lorenz does not depend on time*/
double* Generator::lorenz_flux(char coord,
							   double sigma,
							   double rho,
							   double beta,
							   const double r0[],
							 //double t0,
							   double h,
							   size_t size)
{
	double t0=0.0;
    Lorenz *par =  new Lorenz(sigma,rho,beta);
    Numerical_Integration flux(r0[0],r0[1],r0[2],t0,h,par);
	//allocating memory for the lorenz
	double* data = new double [size];

	//throw away the initial data, to make sure the orbit fallen on the attractor
	for(int i=0;i<1000;i++)
		flux.next();
	switch(coord){
		case 'x':
		{
			for (unsigned int i=0;i<size;i++){
			        	flux.next();
			        	data[i] = flux.get_x();
			}
		}
		break;
		case 'y':
		{
			for (unsigned int i=0;i<size;i++){
		      	flux.next();
		       	data[i] = flux.get_y();
			}
		}
		break;
		case 'z':
		{
			for (unsigned int i=0;i<size;i++){
		       	flux.next();
		       	data[i] = flux.get_z();
			}
		}
		break;
		default:{
			throw(Value_error("Coodinate must be x,y or z"));
		}
	}
	delete par;
	return(data);
}
/******* End Generator ***********/

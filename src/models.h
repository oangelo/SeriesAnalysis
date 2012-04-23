/*
 * models.h
 *
 *  Created on: Apr 20, 2010
 *      Author: gralha
 */

#ifndef MODELS_H_
#define MODELS_H_
#include "numerical_integration.h" //have the numerical methods
#include <fstream>
#include <string>
#include <math.h>
#include <sys/types.h>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <dirent.h>
#include <errno.h>
#include"lib_exceptions.h" 
#define TRANSIENT_LOGISTIC_MAP 10000

/*
 *  Generate time series to be models
 */
class Generator
{
public:
	Generator();
	/*
	 *  BE CAREFUL: client must free the pointers returned. No memory manage!
	 */
	virtual ~Generator();
	/*
	 *  Logistic map: see http://en.wikipedia.org/wiki/Logistic_map
	 */
	double* logistic_map(double r,double x0,size_t size);
	/*
	 * generate the one coordinate time series from the lorenz attractor.
	 *
	 * lorenz attractor: see An Introduction to Computer Simulation Methods:
	 * Applications to Physical Systems (3rd Edition) (Paperback)
	 * Harvey Gould (Author), Jan Tobochnik (Author), Wolfgang Christian (Author)
	 *
	 * see to: http://en.wikipedia.org/wiki/Lorenz_attractor
	 */
	double* lorenz_flux(char coord,
						double sigma,
						double rho,
						double beta,
						const double r0[],
						//double t0,
						double h,
						size_t size);

	/*
	 *
	 * generate the one coordinate time series from the renon map.
	 *
	 * generate the time series for the renon map:
	 * see: http://en.wikipedia.org/wiki/Hénon_map
	 */
	double *henon_map(char coord,
					  double a,
					  double b,
					  double x0,
					  double y0,
					  size_t size);
};

#endif /* MODELS_H_ */

/*
 * File:   Numerical_Integration.h
 * Author: angelo
 *
 * Created on October 27, 2009, 5:01 PM
 */

#ifndef _NUMERICAL_INTEGRATION_H
#define	_NUMERICAL_INTEGRATION_H

class Equation {
public:
	virtual double dx(double T, double X, double Y, double Z)=0;
	virtual double dy(double T, double X, double Y, double Z)=0;
	virtual double dz(double T, double X, double Y, double Z)=0;
};


class Lorenz:public Equation{
public:
	Lorenz(double sigma,double rho,double beta);
	virtual ~Lorenz();
	double dx(double T, double X, double Y, double Z);
	double dy(double T, double X, double Y, double Z);
	double dz(double T, double X, double Y, double Z);
private:
	double _sigma,_rho,_beta;
};

class Numerical_Integration {

public:
    double get_x();
    double get_y();
    double get_z();
    double get_t();
    void next();

    Numerical_Integration(double x0, double y0, double z0,double t0,double h,Equation *eq);
    virtual ~Numerical_Integration();

private:
    double _x, _y, _z, _dt, _t, _tcont;
    Equation *_eq;
    void runge_kutta();
};


#endif	/* _NUMERICAL_INTEGRATION_H */


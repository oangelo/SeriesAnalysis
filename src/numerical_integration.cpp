	/*
 * File:   Numerical_Integration.cpp
 * Author: angelo
 *
 * Created on October 27, 2009, 5:01 PM
 */

#include "numerical_integration.h"

/*
double Equation::dx(double T, double X, double Y, double Z) = 0;

double Equation::dy(double T, double X, double Y, double Z) {
}

double Equation::dz(double T, double X, double Y, double Z) {
}
*/
Lorenz::Lorenz(double sigma,double rho,double beta){
	_sigma = sigma; _rho = rho; _beta = beta;
}

Lorenz::~Lorenz(){

}

double Lorenz::dx(double T, double X, double Y, double Z) {
    double func;
    func = _sigma * (Y - X);
    return (func);
}

double Lorenz::dy(double T, double X, double Y, double Z) {
    double func;
    func = X * (_rho - Z) - Y;
    return (func);
}

double Lorenz::dz(double T, double X, double Y, double Z) {
    double func;
    func = X * Y - _beta*Z;
    return (func);
}
//*****************************************************

void Numerical_Integration::runge_kutta()
//Runge kuta Para as equacoes do atrator de lorenz!
{
	//TODO: Generalizar o runge kutta para N dimensões
    double k[3][4];

    k[0][0] = _dt * _eq->dx(_t, _x, _y, _z);
    k[1][0] = _dt * _eq->dy(_t, _x, _y, _z);
    k[2][0] = _dt * _eq->dz(_t, _x, _y, _z);

    k[0][1] = _dt * _eq->dx(_t + 0.5 * _dt, _x + (0.5 * k[0][0]), _y + (0.5 * k[1][0]), _z + (0.5 * k[2][0]));
    k[1][1] = _dt * _eq->dy(_t + 0.5 * _dt, _x + (0.5 * k[0][0]), _y + (0.5 * k[1][0]), _z + (0.5 * k[2][0]));
    k[2][1] = _dt * _eq->dz(_t + 0.5 * _dt, _x + (0.5 * k[0][0]), _y + (0.5 * k[1][0]), _z + (0.5 * k[2][0]));

    k[0][2] = _dt * _eq->dx(_t + 0.5 * _dt, _x + (0.5 * k[0][1]), _y + (0.5 * k[1][1]), _z + (0.5 * k[2][1]));
    k[1][2] = _dt * _eq->dy(_t + 0.5 * _dt, _x + (0.5 * k[0][1]), _y + (0.5 * k[1][1]), _z + (0.5 * k[2][1]));
    k[2][2] = _dt * _eq->dz(_t + 0.5 * _dt, _x + (0.5 * k[0][1]), _y + (0.5 * k[1][1]), _z + (0.5 * k[2][1]));


    k[0][3] = _dt * _eq->dx(_t + _dt, _x + (k[0][2]), _y + (k[1][2]), _z + (k[2][2]));
    k[1][3] = _dt * _eq->dy(_t + _dt, _x + (k[0][2]), _y + (k[1][2]), _z + (k[2][2]));
    k[2][3] = _dt * _eq->dz(_t + _dt, _x + (k[0][2]), _y + (k[1][2]), _z + (k[2][2]));

    _x = _x + (k[0][0] + 2 * k[0][1] + 2 * k[0][2] + k[0][3]) / 6;
    _y = _y + (k[1][0] + 2 * k[1][1] + 2 * k[1][2] + k[1][3]) / 6;
    _z = _z + (k[2][0] + 2 * k[2][1] + 2 * k[2][2] + k[2][3]) / 6;
}

double Numerical_Integration::get_x() {
    return (_x);
}

double Numerical_Integration::get_y() {
    return (_y);
}

double Numerical_Integration::get_z() {
    return (_z);
}

double Numerical_Integration::get_t() {
    return (_t);
}

Numerical_Integration::Numerical_Integration(double x0, double y0, double z0,double t0,double h,Equation *eq){
    _x = x0;
    _y = y0;
    _z = z0;
    _dt = h;
    _t = t0;
    _tcont = 0;
    _eq=eq;
}

void Numerical_Integration::next() {
    runge_kutta();
    _tcont++;
    _t += _dt;
}


Numerical_Integration::~Numerical_Integration() {
    // TODO Auto-generated destructor stub
}

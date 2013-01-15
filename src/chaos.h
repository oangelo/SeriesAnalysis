#ifndef CHAOS_H
#define CHAOS_H 

#include <map>
#include <vector>

#include "attractor.h"

//Surface is defined by a variable(an unsingned) and a value (a double).
//as a map: surface[variable] = value
std::vector<std::vector<double>> PoincareMap(Attractor attractor, std::map<unsigned, double>  surface);
std::vector<double> PointOfSurfaceIntersect(std::vector<double> point1, 
                                                         std::vector<double> point2,
                                                         std::map<unsigned, double>  surface);


#endif /* CHAOS_H */

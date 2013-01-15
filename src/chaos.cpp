#include "chaos.h"

//Surface is defined by a variable(an unsingned) and a value (a double).
//as a map of: surface[variable] = value
std::vector<std::vector<double>> PoincareMap(Attractor attractor, std::map<unsigned, double>  surface){
    std::vector<std::vector<double>> map;

    return map;
}

std::vector<double> PointOfSurfaceIntersect(std::vector<double> point1, 
                                                         std::vector<double> point2,
                                                         std::map<unsigned, double>  surface)
{
    std::vector<double> intersect_point(point1.size() - surface.size());
    bool cross_surface = false;
    unsigned count_cross = 0;
    for(auto& variable: surface)
        if(point1[variable.first] > variable.second)
            if(point2[variable.first] < variable.second)
                ++count_cross;
    if(count_cross == surface.size())
        cross_surface = true;

    return intersect_point;
}

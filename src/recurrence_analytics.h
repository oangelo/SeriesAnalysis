#ifndef RECURRENCE_ANALYTICS_H
#define RECURRENCE_ANALYTICS_H 

double RR();
double DET();
double L();



std::vector<unsigned> Diagonals();
std::vector<unsigned> Verticals();
unsigned CountBlackDots();

PairsList Burn(unsigned i,unsigned j);
PairsList Paint(unsigned i,unsigned j, unsigned color);
unsigned DiagonalSize(PairsList  cluster);

#endif /* RECURRENCE_ANALYTICS_H */

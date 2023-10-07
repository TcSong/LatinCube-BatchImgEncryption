#ifndef CHAOTICSYSTEM_H_INCLUDED
#define CHAOTICSYSTEM_H_INCLUDED

#include <stdio.h>

typedef struct {
    double x;
    double y;
    double z;
} ipvLorenz;

void iterateIpvdLorenz(int num, ipvLorenz *ipvLnz);

void preIterateIpvLorenz(int num, ipvLorenz *ipvLnz);


#endif // CHAOTICSYSTEM_H_INCLUDED

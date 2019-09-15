#pragma once

#include "termsarray.h"

#define PI 3.14159265358979323846264338327950288419716939
#define TORADIANS(x) ((x) * PI / 180)

double Fix360(double x);
double Fix2Pi(double x);
void pdms(double x);
void phms(double x);

// from radians, print decimal degrees:
#define PDEG(x) std::cout << #x " is: " << (x) * 180.0 / PI << std::endl;
// from radians, print decimal seconds:
#define PSEC(x) PDEG(x * 3600);


double sumseries(Termsarray* series, double tau);

#include "stdafx.h"
#include <iostream>
#include "math.h"
#include "utility.h"
using namespace std;


double Fix360(double x)
{
    x = fmod(x, 360.0);
    if (x < 0.0)
    {
        x += 360.0;
    }
    return x;
}

double Fix2Pi(double x)
{
    x = fmod(x, 2 * PI);
    if (x < 0.0)
    {
        x += 2 * PI;
    }
    return x;
}

void pdms(double x)
{
    if (x < 0)
    {
        cout << "-";
    }
    x *= 180 / PI;
    double degs, mins, secs;

    double frac = fabs(modf(x, &degs));
    degs = fabs(degs);
    frac *= 60;
    secs = 60 * fabs(modf(frac, &mins));

    cout << int(degs) << "o " << int(mins) << "' " << secs << "\"" << endl;
}


void phms(double x)
{
    x *= 12 / PI;
    double degs, mins, secs;

    double frac = fabs(modf(x, &degs));
    frac *= 60;
    secs = 60 * fabs(modf(frac, &mins));
    cout << int(degs) << "h " << int(mins) << "m " << secs << "s" << endl;
}


// sum a vsop series - the return value for an L series (longitude)
// or a B series (latitude) is in RADIANS
// The result for an R series (heliocentric radius vector) is in
// Astronomical Units
double sumseries(Termsarray* series, double tau)
{
    // get number of entries in the table:
    int entries;
    for (entries = 0; series[entries].a != NULL; entries++)
    {
    }

    double* sums = new double[entries];

    for (int i = 0; i < entries; i++)
    {
        double sum = 0;
        for (int j = 0; j < series[i].siz; j++)
        {
            double a = series[i].a[j * 3];
            double b = series[i].a[j * 3 + 1];
            double c = series[i].a[j * 3 + 2];
            double term = a * cos(b + c * tau);
            sum += term;
        }
        sums[i] = sum;
    }

	int i;
    double finalsum;
    double taupower;
    for (i = 0, finalsum = 0, taupower = 1.0;
            i < entries;
                i++, taupower *= tau)
    {
        finalsum += sums[i] * taupower;
    }
    delete[] sums;
    
    return finalsum;
}
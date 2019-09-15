#include "stdafx.h"
#include "utility.h"
#include "epsilon0.h"

// returns epsilon 0 IN RADIANS - from the expression on page 135
// (equation 21.3)
double GetEpsilon0(double jd)
{
    double T = (jd - 2451545.0) / 36525.0;
    double U = T / 100.0;

    // get epsilon 0 in arcseconds according to equation 21.3:
    double epsilon0 = 3600.0 * 23.0 + 60.0 * 26.0 + 21.448 -
                      4680.93 * U -
                      1.55 * U * U +
                      1999.25 * U * U * U -
                      51.38 * U * U * U * U -
                      249.67 * U * U * U * U * U -
                      39.05 * U * U * U * U * U * U +
                      7.12 * U * U * U * U * U * U * U +
                      27.87 * U * U * U * U * U * U * U * U +
                      5.79 *  U * U * U * U * U * U * U * U * U +
                      2.45 *  U * U * U * U * U * U * U * U * U * U;

    // return in radians:
    return epsilon0  * PI / 180.0 / 3600.0;
}


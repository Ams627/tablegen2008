#include "stdafx.h"
#include "utility.h"
#include <math.h>

// figures in this namespace are from table 21.A page 133
namespace nutation {

#define TERMS 63

struct NUTATIONARGS {
    double D, M, MDASH, F, OMEGA;
};

struct NUTATIONCOEFFS {
    double lon0, lon1, obl0, obl1;
};

const static struct NUTATIONARGS arguments[] = {
    {0.0,       0.0,    0.0,    0.0,    1.0},
    {-2.0,      0.0,    0.0,    2.0,    2.0},
    {0.0,       0.0,    0.0,    2.0,    2.0},
    {0.0,       0.0,    0.0,    0.0,    2.0},
    {0.0,       1.0,    0.0,    0.0,    0.0},
    {0.0,       0.0,    1.0,    0.0,    0.0},
    {-2.0,      1.0,    0.0,    2.0,    2.0},
    {0.0,       0.0,    0.0,    2.0,    1.0},
    {0.0,       0.0,    1.0,    2.0,    2.0},
    {-2.0,      -1.0,   0.0,    2.0,    2.0},
    {-2.0,      0.0,    1.0,    0.0,    0.0},
    {-2.0,      0.0,    0.0,    2.0,    1.0},
    {0.0,       0.0,    -1.0,   2.0,    2.0},
    {2.0,       0.0,    0.0,    0.0,    0.0},
    {0.0,       0.0,    1.0,    0.0,    1.0},
    {2.0,       0.0,    -1.0,   2.0,    2.0},
    {0.0,       0.0,    -1.0,   0.0,    1.0},
    {0.0,       0.0,    1.0,    2.0,    1.0},
    {-2.0,      0.0,    2.0,    0.0,    0.0},
    {0.0,       0.0,    -2.0,   2.0,    1.0},
    {2.0,       0.0,    0.0,    2.0,    2.0},
    {0.0,       0.0,    2.0,    2.0,    2.0},
    {0.0,       0.0,    2.0,    0.0,    0.0},
    {-2.0,      0.0,    1.0,    2.0,    2.0},
    {0.0,       0.0,    0.0,    2.0,    0.0},
    {-2.0,      0.0,    0.0,    2.0,    0.0},
    {0.0,       0.0,    -1.0,   2.0,    1.0},
    {0.0,       2.0,    0.0,    0.0,    0.0},
    {2.0,       0.0,    -1.0,   0.0,    1.0},
    {-2.0,      2.0,    0.0,    2.0,    2.0},
    {0.0,       1.0,    0.0,    0.0,    1.0},
    {-2.0,      0.0,    1.0,    0.0,    1.0},
    {0.0,       -1.0,   0.0,    0.0,    1.0},
    {0.0,       0.0,    2.0,    -2.0,   0.0},
    {2.0,       0.0,    -1.0,   2.0,    1.0},
    {2.0,       0.0,    1.0,    2.0,    2.0},
    {0.0,       1.0,    0.0,    2.0,    2.0},
    {-2.0,      1.0,    1.0,    0.0,    0.0},
    {0.0,       -1.0,   0.0,    2.0,    2.0},
    {2.0,       0.0,    0.0,    2.0,    1.0},
    {2.0,       0.0,    1.0,    0.0,    0.0},
    {-2.0,      0.0,    2.0,    2.0,    2.0},
    {-2.0,      0.0,    1.0,    2.0,    1.0},
    {2.0,       0.0,    -2.0,   0.0,    1.0},
    {2.0,       0.0,    0.0,    0.0,    1.0},
    {0.0,       -1.0,   1.0,    0.0,    0.0},
    {-2.0,      -1.0,   0.0,    2.0,    1.0},
    {-2.0,      0.0,    0.0,    0.0,    1.0},
    {0.0,       0.0,    2.0,    2.0,    1.0},
    {-2.0,      0.0,    2.0,    0.0,    1.0},
    {-2.0,      1.0,    0.0,    2.0,    1.0},
    {0.0,       0.0,    1.0,    -2.0,   0.0},
    {-1.0,      0.0,    1.0,    0.0,    0.0},
    {-2.0,      1.0,    0.0,    0.0,    0.0},
    {1.0,       0.0,    0.0,    0.0,    0.0},
    {0.0,       0.0,    1.0,    2.0,    0.0},
    {0.0,       0.0,    -2.0,   2.0,    2.0},
    {-1.0,      -1.0,   1.0,    0.0,    0.0},
    {0.0,       1.0,    1.0,    0.0,    0.0},
    {0.0,       -1.0,   1.0,    2.0,    2.0},
    {2.0,       -1.0,   -1.0,   2.0,    2.0},
    {0.0,       0.0,    3.0,    2.0,    2.0},
    {2.0,       -1.0,   0.0,    2.0,    2.0}};

const static struct NUTATIONCOEFFS coefficients[TERMS] = {
    {-171996.0, -174.2, 92025.0,8.9},
    {-13187.0,  -1.6,   5736.0, -3.1},
    {-2274.0,    0.2,   977.0,  -0.5},
    {2062.0,    0.2,    -895.0,    0.5},
    {1426.0,    -3.4,    54.0,    -0.1},
    {712.0,    0.1,    -7.0,    0.0},
    {-517.0,    1.2,    224.0,    -0.6},
    {-386.0,    -0.4,    200.0,    0.0},
    {-301.0,    0.0,    129.0,    -0.1},
    {217.0,    -0.5,    -95.0,    0.3},
    {-158.0,    0.0,    0.0,    0.0},
    {129.0,     0.1,    -70.0,  0.0},
    {123.0,     0.0,    -53.0,  0.0},
    {63.0,      0.0,    0.0,    0.0},
    {63.0,      1.0,    -33.0,  0.0},
    {-59.0,     0.0,    26.0,   0.0},
    {-58.0,     -0.1,   32.0,   0.0},
    {-51.0,     0.0,    27.0,   0.0},
    {48.0,      0.0,    0.0,    0.0},
    {46.0,      0.0,    -24.0,  0.0},
    {-38.0,     0.0,    16.0,   0.0},
    {-31.0,     0.0,    13.0,   0.0},
    {29.0,      0.0,    0.0,    0.0},
    {29.0,      0.0,    -12.0,  0.0},
    {26.0,      0.0,    0.0,    0.0},
    {-22.0,     0.0,    0.0,    0.0},
    {21.0,      0.0,    -10.0,  0.0},
    {17.0,      -0.1,   0.0,    0.0},
    {16.0,      0.0,    -8.0,   0.0},
    {-16.0,     0.1,    7.0,    0.0},
    {-15.0,     0.0,    9.0,    0.0},
    {-13.0,     0.0,    7.0,    0.0},
    {-12.0,     0.0,    6.0,    0.0},
    {11.0,      0.0,    0.0,    0.0},
    {-10.0,     0.0,    5.0,    0.0},
    {-8.0,      0.0,    3.0,    0.0},
    {7.0,       0.0,    -3.0,   0.0},
    {-7.0,      0.0,    0.0,    0.0},
    {-7.0,      0.0,    3.0,    0.0},
    {-7.0,      0.0,    3.0,    0.0},
    {6.0,       0.0,    0.0,    0.0},
    {6.0,       0.0,    -3.0,   0.0},
    {6.0,       0.0,    -3.0,   0.0},
    {-6.0,      0.0,    3.0,    0.0},
    {-6.0,      0.0,    3.0,    0.0},
    {5.0,       0.0,    0.0,    0.0},
    {-5.0,      0.0,    3.0,    0.0},
    {-5.0,      0.0,    3.0,    0.0},
    {-5.0,      0.0,    3.0,    0.0},
    {4.0,       0.0,    0.0,    0.0},
    {4.0,       0.0,    0.0,    0.0},
    {4.0,       0.0,    0.0,    0.0},
    {-4.0,      0.0,    0.0,    0.0},
    {-4.0,      0.0,    0.0,    0.0},
    {-4.0,      0.0,    0.0,    0.0},
    {3.0,       0.0,    0.0,    0.0},
    {-3.0,      0.0,    0.0,    0.0},
    {-3.0,      0.0,    0.0,    0.0},
    {-3.0,      0.0,    0.0,    0.0},
    {-3.0,      0.0,    0.0,    0.0},
    {-3.0,      0.0,    0.0,    0.0},
    {-3.0,      0.0,    0.0,    0.0},
    {-3.0,      0.0,    0.0,    0.0}};

} // namespace nutation

// Get the nutation in Longitude (dpsi) and nutation in obliquity (despilon)
// These values are returned in radians:
// See page 132 for the details of the calculation

void GetNutation(double& dpsi, double &depsilon, double jd)
{
    double T = (jd - 2451545.0)/36525;
    double T2 = T * T;
    double T3 = T2 * T;

    double D = 297.85036 + 445267.111480 * T - 0.0019142 * T2 + T3 / 189474.0;
    double M = 357.52772 + 35999.050340 * T - 0.0001603 * T2 - T3 / 300000.0;
    double MDASH = 134.96298 + 477198.867398 * T + 0.0086972 * T2 + T3 / 56250.0;
    double F = 93.2719100 + 483202.017538 * T - 0.0036825 * T2 + T3 / 327270.0;
    double OMEGA = 125.04452 - 1934.136261 * T + 0.0020708 * T2 + T3 / 450000.0;

    double argument, sincoeff, coscoeff;
    dpsi = depsilon = 0;

    for (int i = 0 ; i < TERMS; i++)
    {
        argument = nutation::arguments[i].D * D +
                   nutation::arguments[i].M * M +
                   nutation::arguments[i].MDASH * MDASH +
                   nutation::arguments[i].F * F +
                   nutation::arguments[i].OMEGA * OMEGA;

        argument = TORADIANS(argument);
        sincoeff = nutation::coefficients[i].lon0 + nutation::coefficients[i].lon1 * T;
        coscoeff = nutation::coefficients[i].obl0 + nutation::coefficients[i].obl1 * T;

        dpsi += sincoeff * sin(argument);
        depsilon += coscoeff * cos(argument);
    }

    // convert dpsi and depsilon to radians:
    dpsi = dpsi  * PI / 36000000.0 / 180.0;
    depsilon = depsilon * PI / 36000000.0 / 180.0;
}
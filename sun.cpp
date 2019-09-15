#include "stdafx.h"
#include <iostream>
#include <math.h>

#include "ADate.h"
#include "utility.h"
#include "nutation.h"
#include "epsilon0.h"
#include "meeusplanets.h"


void Normalize(double &m);
void GetSunPos(double &alpha, double &delta, CADate date, double time=0);
double FinalM(double a1, double a2, double a3,
              double d1, double d2, double d3,
              double phi, double L, double m,
              double theta0, double h0);

// Return variation IN RADIANS in sun's geocentric longitude (delta lamba)
// See Meeus page 156 for the equation
double GetDeltaLamda(double jd)
{
    double T = (jd - 2451545.0) / 365250;

    double result =
            3548.193 +
            118.568 * sin(87.5287 + 359993.7286 * T) +
            2.476 * sin(85.0561 + 719987.4571 * T) +
            1.376 * sin(27.8502 + 4452671.1152 * T) +
            0.119 * sin(73.1375 + 450368.88564 * T) +
            0.114 * sin(337.2264 + 329644.6718 * T) +
            0.086 * sin(222.5400 + 659289.3436 * T) +
            0.078 * sin(162.8136 + 9224659.7195 * T) +
            0.054 * sin(82.5823 + 1079981.1857 * T) +
            0.052 * sin(171.5189 + 225184.4282 * T) +
            0.034 * sin(30.3214 + 4092677.3866 * T) +
            0.033 * sin(119.8105 + 337181.4711 * T) +
            0.023 * sin(247.5418 + 299295.6151 * T) +
            0.023 * sin(325.1526 + 315559.5560 * T) +
            0.021 * sin(155.1241 + 675553.2846 * T) +
            7.311 * T * sin(333.4515 + 359993.7286 * T) +
            0.305 * T * sin(330.9814 + 719987.4571 * T) +
            0.010 * T * sin(328.5170 + 1079981.1857 * T) +
            0.309 * T * T * sin(241.4518 + 359993.7286 * T) +
            0.021 * T * T * sin(205.0482 + 719987.4571 * T) +
            0.004 * T * T * sin(297.8610 + 4452671.1152 * T) +
            0.010 * T * T * T * sin(154.7066 + 359993.7286 * T);
    
    // to degrees:
    result /= 3600.0;
    // to radians:
    result *= PI / 180.0;

    return result;
}



void SunRise(double& rise, double& set, double lat, double lon, CADate date)
{
        double T;
        double jd = date.GetJulianDate();
        T = (jd - 2451545.0) / 36525.0;

        // sidereal time at greenwich page 83
        double theta0 = 100.46061837 + 36000.770053608 * T +
                        0.000387933 * T * T - (T * T * T) / 38710000.0;

        theta0 = fmod(theta0, 360.0);
        if (theta0 < 0.0)
        {
            theta0 += 360.0;
        }
        theta0 *= PI / 180.0;

        double alpha1, alpha2, alpha3;
        double delta1, delta2, delta3;

        // sun's position on day before, day specified and day after
        GetSunPos(alpha1, delta1, date - 1);
        GetSunPos(alpha2, delta2, date);
        std::cout << "------------------" << std::endl;
        phms(alpha2);
        pdms(delta2);
        std::cout << "------------------" << std::endl;
        GetSunPos(alpha3, delta3, date + 1);

        // for the sun only - see pages 97-98 - get h0 in radians:
        double h0 = -0.8333333333 * PI / 180.0; // RADIANS

        // use correct symbols - see page 97 and covert to radians:
        double phi = lat * PI / 180.0;
        double L = lon * PI / 180.0;

        // equation 14.1 - see page 98
        double cosH0 = (sin(h0) - sin(phi) *
                        sin(delta2))  /
                       (cos(phi) * cos(delta2));

        double cosHM1 = (sin(h0) - sin(phi) *
                        sin(delta1))  /
                       (cos(phi) * cos(delta1));


        double cosHP1 = (sin(h0) - sin(phi) *
                        sin(delta3))  /
                       (cos(phi) * cos(delta3));

        double alphae, deltae;
        GetSunPos(alphae, deltae, CADate(20, 3, 2000), (7.0 + 35.0/60.0) / 24.0);
        double cosHPe = (sin(h0) - sin(phi) *
                        sin(deltae))  /
                       (cos(phi) * cos(deltae));


        // find H0 itself from its cosine:
        if (cosH0 > 1.0000000000000)
        {
            cosH0 = cosH0;
        }
        double H0 = acos(cosH0);

        // calculate approximate times in fractions of a day:
        // (equations 14.2 - see page 98)
        double m0 = (alpha2 + L - theta0) / 2.0 / PI;
        double m1 = m0 - (H0 / 2.0 / PI);
        double m2 = m0 + (H0 / 2.0 / PI);

        // make sure the above calculated fractions are between zero and one:
        Normalize(m0);
        Normalize(m1);
        Normalize(m2);

        // Interpolate and iterate using the method on page 99
        rise = FinalM(alpha1, alpha2, alpha3, delta1, delta2, delta3,
            phi, L, m1, theta0, h0);

        set = FinalM(alpha1, alpha2, alpha3, delta1, delta2, delta3,
            phi, L, m2, theta0, h0);
}


// Get the sun's right ascension and declination IN RADIANS
// See the chapter on Solar Coordinates (Chapter 24)
// Page 151 onwards
// This function uses the more accurate method of page 154 onwards:
void GetSunPos(double &alpha, double &delta, CADate date, double time)
{
    double jde = date.GetJulianDate() + time;

    double tau = (jde - 2451545.0) / 365250;

    double l, b, r;

    // get heliocentric coordinates for earth from VSOP series:
    l = sumseries(vsopEarth::aLong, tau); // radians
    b = sumseries(vsopEarth::aLat, tau);  // radians
    r = sumseries(vsopEarth::aRad, tau);  // astronomical units

    // adjust to get geocentric coordinates of the sun:
    double sun = l + PI;
    double beta = -b;

    // get in 0-360 degrees:
    sun = Fix2Pi(sun);

    using namespace std;
    cout.precision(10);

    // Do the FK5 corrections - page 154 equations 24.9
    double T = 10 * tau;
    double lamdadash = sun - (1.397 * T + 0.00031 * T * T) * PI / 180.0;
    double deltasun = -0.09033 / 3600.0 * PI / 180.0;
    double deltabeta = 0.03916 * (cos(lamdadash) - sin(lamdadash)) / 3600.0 * PI / 180.0;
    sun += deltasun;

    beta += deltabeta;

    // Get nutations in longitude and obliquity - these are returned
    // in RADIANS:
    double dpsi, depsilon;
    GetNutation(dpsi, depsilon, jde);

    sun += dpsi;

    // Caculate the correction in the sun's longitude due to abberation:
    // Equations 24.11 on page 155 and the long equation on page 156
    double deltalambda = GetDeltaLamda(jde); // in RADIANS
    double abberation = 0.005775518 * r * deltalambda;
    sun -= abberation;

    double epsilon0 = GetEpsilon0(jde);
    epsilon0 += depsilon;

    // get alpha and delta in RADIANS:
    alpha = atan2( (cos(epsilon0) * sin(sun)),
                         cos(sun));

    delta = asin(sin(epsilon0) * sin(sun));
}



void Normalize(double &m)
{
    while (m < 0.0)
    {
        m += 1.0;
    }

    while (m > 1.0)
    {
        m -= 1.0;
    }
}


double interp(double x1, double x2, double x3, double n)
{
    double a = x2 - x1;
    double b = x3 - x2;
    double c = b - a;
    return x2 + 0.5 * n * (a + b + n * c);
}

double FinalM(double a1, double a2, double a3,
              double d1, double d2, double d3,
              double phi, double L, double m,
              double theta0, double h0)
{
    double n, alpha, delta, H, sinh, deltam, h;
    const double deltaT = 67.0;

    double theta;

    do
    {
        theta = theta0 + (360.985647 * PI / 180.0) * m; // page 99
        n = m + deltaT / 86400.0;        // page 99
        alpha = interp(a1, a2, a3, n);   // interpolate - equation 3.3 page 25
        delta = interp(d1, d2, d3, n);   // interpolate - equation 3.3 page 25

        H = theta - L - alpha;

        sinh = sin(phi) * sin(delta) +
               cos(phi) * cos(delta) * cos(H);

        h = asin(sinh);

        deltam = (h - h0) /
                (2 * PI * cos(delta) * cos(phi) * sin(H));

        m = m + deltam;
    }
    while (fabs(deltam) > 0.00001);
    return m;
}

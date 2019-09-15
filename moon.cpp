#include "stdafx.h"
#include <math.h>
#include "moon.h"
#include "utility.h"
#include "moonterms.h"

namespace {
    void GetMoonPos(double &alpha, double &delta, double &pi, CADate date);
    void FixInStraightLine(double &a, double &b, double &c);
    void Norm360(double &x);
    void Normalize(double &m);
    double FinalM(double a1, double a2, double a3,
                  double d1, double d2, double d3,
                  double phi, double L, double m,
                  double theta0, double h0);
    double interp(double x1, double x2, double x3, double n);
}

void MoonRise(double& rise, double& set, double lat, double lon, CADate date)
{
    double alpha1, alpha2, alpha3, delta1, delta2, delta3, pi, dummy;

    GetMoonPos(alpha1, delta1, dummy, date - 1);
    GetMoonPos(alpha2, delta2, pi, date);
    GetMoonPos(alpha3, delta3, dummy, date + 1);

    Norm360(alpha1);
    Norm360(alpha2);
    Norm360(alpha3);
    Norm360(delta1);
    Norm360(delta2);
    Norm360(delta3);

    FixInStraightLine(alpha1, alpha2, alpha3);
    FixInStraightLine(delta1, delta2, delta3);

    CString cs1, cs2, cs3;

    double phi = lat;
    double L = lon;

    double h0 = 0.7275 * pi - 34.0 / 60.0;

    double cosH0 = (sin(TORADIANS(h0)) - sin(TORADIANS(phi)) *
                    sin(TORADIANS(delta2)) ) /
                   (cos(TORADIANS(phi)) * cos(TORADIANS(delta2)));

    double H0 = acos(cosH0) * 180.0 / PI;

    double T = (date.GetJulianDate() - 2451545.0) / 36525.0;
    double theta0 = 100.46061837 + 36000.770053608 * T +
                    0.000387933 * T * T - (T * T * T) / 38710000.0;

    double m0 = (alpha2 + L - theta0) / 360.0;
    double m1 = m0 - (H0 / 360.0);
    double m2 = m0 + (H0 / 360.0);

    Normalize(m0);
    Normalize(m1);
    Normalize(m2);

    rise = FinalM(alpha1, alpha2, alpha3, delta1, delta2, delta3,
                  phi, L, m1, theta0, h0);

    set = FinalM(alpha1, alpha2, alpha3, delta1, delta2, delta3,
                 phi, L, m2, theta0, h0);

}


namespace {

    void GetMoonPos(double &alpha, double &delta, double &pi, CADate date)
    {
        double jd = date.GetJulianDate();
        double T = (jd - 2451545.0) / 36525.0;
        double T2 = T * T;
        double T3 = T * T * T;
        double T4 = T * T * T * T;

        double L = 280.46645 + 36000.76983 * T + 0.0003032 * T2;    // 24.2

        double LDASH = 218.3164591 + 481267.88134236 * T -
                       0.0013268 * T2 + T3 / 538841.0 - T4 / 65194000.0;    // 45.1

        double D = 297.8502042 + 445267.1115168 * T - 0.0016300 * T2 +  T3 / 545868.0
                    - T4 / 113065000.0;   // 45.2

        double M = 357.5291092 + 35999.0502909 * T - 0.0001536 * T2 + T3 / 24490000.0;    // 45.3

        double MDASH = 134.9634114 + 477198.8676313 * T +
                       0.0089970 * T2 + T3 / 69699.0 - T4 / 14712000.0; // 45.4

        double F = 93.2720993 + 483202.0175273 * T - 0.0034029 * T2 - T3 / 3526000.0 +
                   T4 / 863310000.0;    // 45.5

        double E = 1.0 - 0.002516 * T - 0.0000074 * T2;
        double E2 = E * E;

        double A1 = 119.75 + 131.849 * T;
        double A2 = 53.09 + 479264.290 * T;
        double A3 = 313.45 + 481266.484 * T;

        Norm360(D);
        Norm360(M);
        Norm360(MDASH);
        Norm360(LDASH);
        Norm360(F);
        Norm360(A1);
        Norm360(A2);
        Norm360(A3);
        Norm360(L);

        double sig1 = 0.0, sigr = 0.0, sigb = 0.0;

        const int LOOP1 = sizeof(t45a) / sizeof(MOONTERM1);

        for (int i = 0; i < LOOP1; i++)
        {
            double arg = t45a[i].arg[0] * D +
                         t45a[i].arg[1] * M +
                         t45a[i].arg[2] * MDASH +
                         t45a[i].arg[3] * F;

            double term1 = t45a[i].coeff[0] * sin(TORADIANS(arg));
            double term2 = t45a[i].coeff[1] * cos(TORADIANS(arg));


            if (abs(t45a[i].arg[1]) == 1)
            {
                term1 *= E;
                term2 *= E;
            }
            else if (abs(t45a[i].arg[1]) == 2)
            {
                term1 *= E2;
                term2 *= E2;
            }

            sig1 += term1;
            sigr += term2;
        }

        sig1 += 3958.0 * sin(TORADIANS(A1)) + 1962.0 * sin(TORADIANS(LDASH - F)) + 318.0 * sin(TORADIANS(A2));

        const int LOOP2 = sizeof(t45a) / sizeof(MOONTERM1);
        for (int i = 0; i < LOOP2; i++)
        {
            double arg = t45b[i].arg[0] * D +
                         t45b[i].arg[1] * M +
                         t45b[i].arg[2] * MDASH +
                         t45b[i].arg[3] * F;

            double term = t45b[i].coeff * sin(TORADIANS(arg));

            if (abs(t45b[i].arg[1]) == 1)
            {
                term *= E;
            }
            else if (abs(t45b[i].arg[1]) == 2)
            {
                term *= E2;
            }

            sigb += term;
        }

        sigb += -2235.0 * sin(TORADIANS(LDASH)) + 382.0 * sin(TORADIANS(A3)) + 175.0 * sin(TORADIANS(A1 - F)) +
                175.0 * sin(TORADIANS(A1 + F)) + 127.0 * sin(TORADIANS(LDASH - MDASH)) - 115.0 * sin(TORADIANS(LDASH + MDASH));

        double lamda = LDASH + sig1 / 1E6;
        double beta = sigb / 1E6;
        double bigDelta = 385000.56 + sigr / 1000.0;

        double omega = 125.04452 - 1934.136261 * T + 0.0020708 * T2 + T3 / 450000.0;
        double nutation = -17.20 * sin(TORADIANS(omega)) - 1.32 * sin(TORADIANS(2.0 * L)) -
                          0.23 * sin(TORADIANS(2.0 * LDASH)) + 0.21 * sin(TORADIANS(2.0 * omega));
        nutation /= 3600.0;
        lamda += nutation;

        double epsilon = 23.0 + 26.0 / 60.0 + 21.448 / 3600.0;

        double deltaEpsilon = 9.2 * cos(TORADIANS(omega)) +
                              0.57 * cos(TORADIANS(2.0 * L)) +
                              0.10 * cos(TORADIANS(2.0 * LDASH)) -
                              0.09 * cos(TORADIANS(2.0 * omega));

        deltaEpsilon /= 3600.0;
        epsilon += deltaEpsilon;

        alpha = atan2(sin(TORADIANS(lamda)) * cos(TORADIANS(epsilon)) - tan(TORADIANS(beta)) * sin(TORADIANS(epsilon)),
                      cos(TORADIANS(lamda)));
        delta = asin( sin(TORADIANS(beta)) * cos(TORADIANS(epsilon)) +
                      cos(TORADIANS(beta)) * sin(TORADIANS(epsilon)) * sin(TORADIANS(lamda)) );

        alpha *= 180.0 / PI;
        delta *= 180.0 / PI;
        pi = 180.0 / PI * asin(6378.14 / bigDelta);
    }


    void FixInStraightLine(double &a, double &b, double &c)
    {
        double d1 = fabs(a - b), d2 = fabs(a - c), d3 = fabs(b - c);

        if (d1 > 60.0 || d2 > 60.0 || d3 > 60.0)
        {

            if (d1 < d2 && d1 < d3) // a - b is smallest diff so c is wrong
            {
                if (c > a)
                {
                    c -= 360.0;
                }
                else
                {
                    c += 360.0;
                }
            }
            if (d2 < d1 && d2 < d3) // a - c is smallest diff so b is wrong
            {
                if (b > a)
                {
                    b -= 360.0;
                }
                else
                {
                    b += 360.0;
                }
            }
            if (d3 < d1 && d3 < d2) // b - c is smallest diff so a is wrong
            {
                if (a > b)
                {
                    a -= 360.0;
                }
                else
                {
                    a += 360.0;
                }
            }
        }
    }

    void Norm360(double &x)
    {
        x = fmod(x, 360.0);
        if (x < 0.0)
        {
            x += 360.0;
        }
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
            theta = theta0 + 360.985647 * m; // page 99
            n = m + deltaT / 86400.0;        // page 99
            alpha = interp(a1, a2, a3, n);   // interpolate - equation 3.3 page 25
            delta = interp(d1, d2, d3, n);   // interpolate - equation 3.3 page 25

            H = theta - L - alpha;

            sinh = sin(TORADIANS(phi)) * sin(TORADIANS(delta)) +
                   cos(TORADIANS(phi)) * cos(TORADIANS(delta)) * cos(TORADIANS(H));

            h = asin(sinh) * 180.0 / PI;

            deltam = (h - h0) /
                    (360.0 * cos(TORADIANS(delta)) * cos(TORADIANS(phi)) * sin(TORADIANS(H)));

            m = m + deltam;
        }
        while (fabs(deltam) > 0.00001);
        return m;
    }

    double interp(double x1, double x2, double x3, double n)
    {
        double a = x2 - x1;
        double b = x3 - x2;
        double c = b - a;
        return x2 + 0.5 * n * (a + b + n * c);
    }
} // namespace

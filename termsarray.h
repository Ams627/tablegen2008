#pragma once

struct Termsarray
{
    double* a;
    int siz;
};

#define XSIZEX(x) x, sizeof x / sizeof(double) / 3

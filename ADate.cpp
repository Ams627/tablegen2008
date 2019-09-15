#include "stdafx.h"
#include "ADate.h"
#include "time.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CADate::CADate()
{
    m_cached = false;
}

CADate::CADate(int d, int m, int y)
{
    m_serial = (1461 * (y + 4800 + (m - 14) / 12)) / 4 +
             (367 * (m - 2 - 12 * ((m - 14) / 12))) / 12 -
             (3 * ((y + 4900 + (m - 14) / 12) / 100)) / 4 + d - 32075;

    m_cached = false;
}

CADate::CADate(int serial)
{
    m_serial = serial;
    m_cached = false;
}

CADate::~CADate()
{

}

void CADate::SetDMY(int d, int m, int y)
{
    m_serial = (1461 * (y + 4800 + (m - 14) / 12)) / 4 +
             (367 * (m - 2 - 12 * ((m - 14) / 12))) / 12 -
             (3 * ((y + 4900 + (m - 14) / 12) / 100)) / 4 + d - 32075;

}

void CADate::SetToday()
{
    time_t now = time(NULL);
    // time_t is a number or seconds since midnight 1/1/1970
    // so divide this down to get the number of days then add
    // the julian day of 1/1/1970:
    m_serial = static_cast<int>(now / (3600 * 24) + 2440588);
    m_cached = false;
}


void CADate::GetDMY(int &d, int &m, int &y)
{
    int l, n, i, j;

    l = m_serial + 68569;
    n = (4 * l) / 146097;
    l = l - (146097 * n + 3) / 4;
    i = (4000 * (l + 1)) / 1461001;
    l = l - (1461 * i) / 4 + 31;
    j = (80 * l) / 2447;
    d = l - (2447 * j) / 80;
    l = j / 11;
    m = j + 2 - 12 * l;
    y = 100 * (n - 49) + i + l;

    m_cachedD = d;
    m_cachedM = m;
    m_cachedY = y;
    m_cached = true;
}


// This function "rounds" the date down to the previous "day"
// i.e. if day is Tuesday and "this" represents a wednesday,
// 1 will be subtracted from m_serial

int CADate::SetPrevious(int day)
{
//    ATLASSERT(day >= 0 && day < 7);

    int correction = (m_serial - day) % 7;
    m_serial -= correction;
    m_cached = false;
    return m_serial;
}

int CADate::GetDayNumber()
{
//    CADate tempDate(1, 1, GetYear());
//    return m_serial - tempDate.m_serial;

    return m_serial - CADate(1, 1, GetYear()).m_serial;
}

// return the week number according to ISO ISO8601
// i.e. week 1 is the week containing the first thursday of the year
int CADate::GetWeekNumber()
{
    DWORD j1, j0, j0n, j1n, j0p, j1p, jp, result;
    int d, m, y;

    GetDMY(d, m, y);

    /* calculate julian date for January 1st of the year given and */
    /* January 1st of the next year and previous year:             */

    j1 = 367L * y - 7 * y / 4 + 1721045L;
    j1n = 367L * (y + 1) - 7 * (y + 1) / 4 + 1721045L;
    j1p = 367L * (y - 1) - 7 * (y - 1) / 4 + 1721045L;

    /* calculate origin Mondays for this year next and previous: */
    /* (always Monday before the first Thursday in the year) */
    j0 = j1 + 3 - ((j1 + 3) % 7);
    j0n = j1n + 3 - ((j1n + 3) % 7);
    j0p = j1p + 3 - ((j1p + 3) % 7);

    /* calculate julian date for date specified: */
    jp = 367L * y - 7 * (y + (m + 9) / 12) / 4
         + 275 * m / 9 + d + 1721014L;

    if (jp >= j0n)
        result = 1;
    else if (jp < j0)
    {
        /* Now calculate a January week 52/53: */
        if (j0p + 364 > jp)
            result = 52;
        else
            result = 53;
    }
    else
        result = (jp - j0) / 7 + 1;

    return (int)result;
}


int CADate::SetJan1(int y)
{
    int x = (1461 * (y + 4799)) / 4 -(3 * ((y + 4899) / 100)) / 4  - 31739;
    return x;
}


void CADate::SetEaster(int y)
{
    int c, n, i, j, k, l, d, m;

    c = y / 100;
    n = y % 19;
    k = (c - 17) / 25;
    i = c - c / 4 - (c - k) / 3 + 19 * n + 15;
    i = i % 30;
    i = i - (i / 28) * (1 - (i / 28) * (29 / (i + 1)) * ((21 - n) / 11));
    j = y + y / 4 + i + 2 - c + c / 4;
    j = j % 7;
    l = i - j;
    m = 3 + (l + 40) / 44;
    d = l + 28 - 31 * (m / 4);

    SetDMY(d, m, y);
}

char* CADate::GetMonthName(int m) // static
{
    return "May";
}

// prefix++
CADate CADate::operator ++()
{
    m_serial ++;
    m_cached = false;
    return *this;
}

// postfix++
CADate CADate::operator ++(int x)
{
    m_serial ++;
    m_cached = false;
    return *this;
}

//prefix --
CADate CADate::operator --()
{
    m_serial --;
    m_cached = false;
    return *this;
}

//postfix --
CADate CADate::operator --(int x)
{
    m_serial --;
    m_cached = false;
    return *this;
}

CADate CADate::operator +(int x)
{
    CADate d(m_serial + x);
    return d;
}

CADate CADate::operator -(int x)
{
    CADate d(m_serial - x);
    return d;
}

CADate CADate::operator +=(int x)
{
    m_serial += x;
    m_cached = false;
    return *this;
}


double CADate::GetJulianDate()
{
    double jd = double(m_serial);
    return jd - 0.5;
}

void CADate::SetFromJD(int jd)
{
    m_serial = jd;
    m_cached = false;
}

static char *ShortMonths[]={"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};
char * CADate::GetShortMonthName()
{
    return ShortMonths[GetMonth() - 1];
}


// EU version of DST - from last Sunday in March to Last Sunday in October
// returns true if we are in
bool CADate::IsInEUDST()
{
    bool result = false;
    int year = GetYear();

    CADate marchDate(31, 3, year);
    marchDate.Cache();  // debug
    marchDate.SetPrevious(6);
    marchDate.Cache(); // debug
    CADate octDate(31, 10, year);
    octDate.Cache(); // debug
    octDate.SetPrevious(6);
    octDate.Cache(); // debug
    if (m_serial >= marchDate.m_serial && m_serial < octDate.m_serial)
    {
        result = true;
    }

    return result;
}

void CADate::Cache()
{
    int d, m, y;
    GetDMY(d, m, y);
}

bool CADate::InRange(CADate &d1, CADate &d2)
{
    bool result = false;

    if (d1.m_serial < d2.m_serial)
    {
        if (m_serial >= d1.m_serial && m_serial <= d2.m_serial)
        {
            result = true;
        }
    }
    else
    {
        if (m_serial >= d2.m_serial && m_serial <= d1.m_serial)
        {
            result = true;
        }
    }
    return result;
}

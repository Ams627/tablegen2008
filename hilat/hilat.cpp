#include "stdafx.h"
#include "..\adate.h"
#include "..\nutation.h"
#include "..\utility.h"
#include "..\epsilon0.h"

#include "sun.h"

int _tmain(int argc, TCHAR* argv[], TCHAR* envp[])
{
    double rise, set;
    CADate date(21, 3, 2000);
    SunRise(rise, set, 85.0, 0, date);
	return 0;
}



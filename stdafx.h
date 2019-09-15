#pragma warning(disable:4258)
// Change these values to use different versions

#define _CRT_SECURE_NO_WARNINGS

#define WINVER           0x0701
#define _WIN32_WINNT     0x0701
#define _WIN32_IE        0x0700
#define _RICHEDIT_VER    0x0200


#include <atlbase.h>
#include <atlapp.h>

extern CAppModule _Module;
#include <atlapp.h>

// #include <atlwin.h>
#include <atlctrls.h>
#include <atlctrlx.h>
#include <atlmisc.h>
#include <atlcrack.h>
// #include <atlframe.h>
// #include <atluser.h>
// #include <atldlgs.h>

#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>
#include <map>
#include <iomanip>
#include <ams/errorutils.h>
#include <ams/utility.h>
#include <ams/debugutils.h>

#include "Directories.h"
extern Directories g_dirs;

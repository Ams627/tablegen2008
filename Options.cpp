#include "stdafx.h"
#include "Options.h"
#include "CustomControls.h"

OptMap g_optMap;
    
// first parameter is the key in the map and the name by which the option is known to the rest
// of the code. It is stored in the options file under this name too.

#define OPTITEM(name, params) g_optMap.insert(std::make_pair(#name, params))

#define BOOLOPT(name, idd, guid, bdefault) \
    g_optMap.insert(std::make_pair(#name, new BoolOption(idd, guid, bdefault, #name)))
#define INTOPT(name, idd, guid, idefault, min, max) \
    g_optMap.insert(std::make_pair(#name, new IntOption(idd, guid, idefault, min, max)))

Options::Options(std::string filename) : filename_(filename)
{
    INTOPT(start year, IDC_EDITSTARTYEAR, GUIDEditBoxYear, 2006, 1753, 9999);
    INTOPT(end year, IDC_EDITENDYEAR, GUIDEditBoxYear, 2006, 1753, 9999);
    BOOLOPT(year planner, IDC_CHECKYEARPLANNER, GUIDCheckBox, true);    
    BOOLOPT(year calendar, IDC_CHECKYEARCALENDAR, GUIDCheckBox, true);
    BOOLOPT(month calendar, IDC_CHECKMONTHCALENDAR, GUIDCheckBox, true);
    BOOLOPT(sun and moon tables, IDC_CHECKSUNMOON, GUIDCheckBox, true);
    BOOLOPT(planetary tables, IDC_CHECKPLANETS, GUIDCheckBox, true);
    BOOLOPT(slider test, IDC_STATICFORSLIDER1, GUIDRGBControl, true);
    BOOLOPT(slider test 2, IDC_STATICFORSLIDER2, GUIDRGBControl, true);
    BOOLOPT(birthdays, IDC_BIRTHDAYS, GUIDBirthdayListView, true);
    BOOLOPT(generate xml, IDC_CHECKGENERATEXML, GUIDCheckBox, false);

// DEBUG:

    std::map<UINT, int> iddfreqmap;
    for (OptMap::iterator p = g_optMap.begin(); p != g_optMap.end(); p++)
    {
        UINT idd = p->second->GetIDD();
        iddfreqmap[idd]++;
        int freq = iddfreqmap[idd];
        if (freq > 1)
        {
            std::stringstream ss;
            ss << "control IDC ";
            ss << idd << " occurs more than once in the options map\n";
            ss << "controls must occur only once\n";
            ss << "program will stop";
            ::MessageBox(NULL, ss.str().c_str(), "Option Map Error", MB_OK);
            ATLASSERT(FALSE);
        }
    }


//    BOOLOPT(slider test 3, IDC_STATICFORSLIDER3, GUIDSliderFromStatic, true);
}

Options::~Options(void)
{
    for (OptMap::iterator p = g_optMap.begin(); p != g_optMap.end(); p++)
    {
        delete p->second;
    }
}

void Options::WriteToFile()
{
    std::ofstream f(filename_.c_str());
    for (OptMap::iterator p = g_optMap.begin(); p != g_optMap.end(); p++)
    {
        OptionBase *pbase = p->second;
        f << p->first << "|" << *(p->second) << std::endl;
    }
    f.close();
}

void Options::ReadFromFile()
{
    std::ifstream f(filename_.c_str());
    std::string s;
    while (getline(f, s))
    {
        // split the string at the vertical bar character to find the option name:
        std::string optname;
        std::size_t pos = s.find('|');
        if (pos != std::string::npos)
        {
            optname = s.substr(0, pos);
            // if the option name isn't found in the map, then ignore it:
            OptMap::iterator it = g_optMap.find(optname);
            if (it != g_optMap.end())
            {
                std::string optvalue = s.substr(pos + 1);
                (it->second)->Read(optvalue);
            }

        }
    }
}

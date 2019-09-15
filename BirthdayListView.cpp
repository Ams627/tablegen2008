#include "stdafx.h"
#include "BirthdayListView.h"
#include "ADate.h"

class BirthdayFileError
{

};

namespace {
    int AsciiHexToBin(char c)
    {
        if (c >= '0' && c <= '9')
        {
            c -= '0';
        }
        else if (c >= 'A' && c <= 'F')
        {
            c -= 'A' + 10;
        }
        else if (c >= 'a' && c <= 'f')
        {
            c -= 'a' + 10;
        }
        else c = -1;
        return c;
    }
}

LRESULT BirthdayListView::OnControlInit(UINT msg, WPARAM wParam, LPARAM lParam)
{
    // Note that the LVS_OWNERDATA must be set in the dialog editor since dynamically changing
    // to this style is not supported

    ModifyStyle(0, LVS_REPORT|LVS_SHOWSELALWAYS);
    // extended style specific to listview - can't use windows extended style:
    SetExtendedListViewStyle(LVS_EX_HEADERDRAGDROP|LVS_EX_GRIDLINES|LVS_EX_FULLROWSELECT);
    InsertColumn(1, "Day Number", LVCFMT_LEFT, 10, -1, -1, 0);
    InsertColumn(2, "Date", LVCFMT_LEFT, 20, -1, -1, 1);
    InsertColumn(3, "Name", LVCFMT_LEFT, 100,-1, -1, 2);
    InsertColumn(4, "Link", LVCFMT_LEFT, 100,-1, -1, 3);
    GetHeader().ModifyStyle(0, HDS_FULLDRAG);
    SetItemCountEx(365, LVSICF_NOINVALIDATEALL);

    // Initialise column widths etc:
    GeneralLVControl::Init("BdayLViewWidths");
    int editable[] = {0, 0, 1, 1};
    SetEditable(editable, sizeof editable / sizeof editable[0]);

    std::string birthdayFilename = g_dirs.GetAppData();
    birthdayFilename += "birthdays.dat";

    OUTPUT(birthdayFilename);
    std::ifstream bdayfile(birthdayFilename.c_str());
    if (bdayfile)
    {
        std::string buf;
        while (std::getline(bdayfile, buf))
        {
            if (buf.length() > 10)
            {
                size_t f = buf.find_first_not_of(" \t");
                if (f != std::string::npos)
                {
                    if (buf[0] == 'b') // Birthday code
                    {
                        int minutesSince1800 = 0;
                        for (int i = 0; i < 8; i++)
                        {
                            char c = buf[i + 1];
                            int res = AsciiHexToBin(c);
                            if (res == -1)
                            {
                                throw BirthdayFileError();
                            }
                            else
                            {
                                minutesSince1800 = minutesSince1800 * 16 + res;
                            }
                        }
                        
                        std::string personName = buf.substr(10);
                        int daysSince1800 = minutesSince1800 / 60 / 24;
                        int serialDayNumber = daysSince1800 + 2378497;
                        CADate d(serialDayNumber);
                        int dayInYear = d.GetDayNumber();
                        std::pair<int, std::string> bdayInfo(dayInYear, personName);
                        bdaylist_.insert(bdayInfo);
                    }
                }
            }
            OUTPUT(buf);
        }
    }

    bdayfile.close();

    return TRUE;
}

LRESULT BirthdayListView::OnGetDispInfo(NMHDR* pNMHDR)
{
    NMLVDISPINFO* gdi = (NMLVDISPINFO*)pNMHDR;
    LVITEM& item = gdi->item;
    if (item.mask & LVIF_TEXT)
    {
        if (item.iSubItem == 0)
        {
            std::stringstream ss;
            ss << gdi->item.iItem + 1;
            lstrcpy(gdi->item.pszText, ss.str().c_str());
            //OUTPUTH(item.cchTextMax);
        }
        else if (item.iSubItem == 1)
        {
            CADate date(1, 1, 2000);
            date += item.iItem;
            std::stringstream ss;
            ss << date.GetDay() << " " << date.GetShortMonthName();
            //OUTPUTH(item.cchTextMax);
            lstrcpy(gdi->item.pszText, ss.str().c_str());
        }
        else if (item.iSubItem == 2)
        {
            std::pair<BDIT, BDIT> mapentries = bdaylist_.equal_range(item.iItem);
            for (BDIT p = mapentries.first; p != mapentries.second; ++p)
            {
                if (p != mapentries.first)
                {
                    lstrcat(gdi->item.pszText, ",");
                }
                lstrcat(gdi->item.pszText, p->second.c_str());
            }
        }
    }
    return 0;
}


void BirthdayListView::GetEditItemString(std::string& x, int item, int subitem)
{
    x.clear();

    if (subitem == 2)
    {
        std::pair<BDIT, BDIT> mapentries = bdaylist_.equal_range(item);
        for (BDIT p = mapentries.first; p != mapentries.second; ++p)
        {
            if (p != mapentries.first)
            {
                x += ",";
            }
            x += p->second;
        }       
    }
}

void SetEditItemString(const std::string& s)
{
    
}

void BirthdayListView::OnEndEdit(char *s)
{
    std::string str(s);
    size_t r1, r2;
    r1 = 0;
    
    while ((r2 = str.find_first_of(",", r1)) != std::string::npos)
    {
        std::string res = str.substr(r1, r2 - r1);
        ams::Trim(res);
        OutputDebugString(res.c_str());
        OutputDebugString("a\n");
        r2++;
        r1 = r2;
    }

}

REGISTER_CONTROL(GUIDBirthdayListView, BirthdayListView);


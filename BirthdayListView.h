#pragma once
#include "GeneralLVControl.h"
#include "ams/wtl/ControlFactory.h"
#include "Options.h"

GUID __declspec(selectany) GUIDBirthdayListView = // {05B2DC8C-C70B-435b-A8C1-D0E653CD7702}
{ 0x5b2dc8c, 0xc70b, 0x435b, { 0xa8, 0xc1, 0xd0, 0xe6, 0x53, 0xcd, 0x77, 0x2 } };

typedef std::multimap<int, std::string> BDMAP;
typedef std::multimap<int, std::string>::const_iterator BDIT;

class BirthdayListView : public GeneralLVControl
{
    std::string widthFilename_;
    BDMAP bdaylist_;
public:

    BEGIN_MSG_MAP(BirthdayListView)
        MESSAGE_HANDLER_EX(WM_CONTROLINIT, OnControlInit)
        REFLECTED_NOTIFY_CODE_HANDLER_EX(LVN_GETDISPINFOA, OnGetDispInfo)
        REFLECTED_NOTIFY_CODE_HANDLER_EX(LVN_GETDISPINFOW, OnGetDispInfo)
        
        CHAIN_MSG_MAP(GeneralLVControl)
    END_MSG_MAP();

    LRESULT OnControlInit(UINT msg, WPARAM wParam, LPARAM lParam);
    LRESULT OnGetDispInfo(NMHDR* pNMHDR);
    void StopEditing();
    void OnEndEdit(char* s);
    void GetEditItemString(std::string& x, int item, int subitem);
};

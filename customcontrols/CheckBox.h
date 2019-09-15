#pragma once

#include "ams/wtl/ControlFactory.h"
#include "../Options.h"

GUID __declspec(selectany) GUIDCheckBox = // {02604B40-8D8C-4d2f-A56B-EBBAA1E53026}
     { 0x2604b40, 0x8d8c, 0x4d2f, { 0xa5, 0x6b, 0xeb, 0xba, 0xa1, 0xe5, 0x30, 0x26 } };

class CheckBox : public CWindowImpl<CheckBox, CButton>
{
public: 
    BEGIN_MSG_MAP(CheckBox)
        MESSAGE_HANDLER_EX(WM_CONTROLINIT, OnControlInit)
        MESSAGE_HANDLER_EX(WM_CONTROLEXIT, OnControlExit)
    END_MSG_MAP()

    // This function is the response to WM_CONTROL_INIT. It initialise the control with the current
    // value from the options data structure. A pointer to the relevant option structure is held in
    // the window's user data and can be retrieved with GetWindowLong
    LRESULT OnControlInit(UINT msg, WPARAM wParam, LPARAM lParam)
    {
        OptionBase* pOptionInfo = reinterpret_cast<OptionBase*>(::GetWindowLongPtr(m_hWnd, GWL_USERDATA));
        if (pOptionInfo->GetBool() == 0)
        {
            SetCheck(BST_UNCHECKED);
        }
        else
        {
            SetCheck(BST_CHECKED);         
        }
        return 0;
    }

    LRESULT OnControlExit(UINT msg, WPARAM wParam, LPARAM lParam)
    {
        OptionBase* pOptionInfo = reinterpret_cast<OptionBase*>(::GetWindowLongPtr(m_hWnd, GWL_USERDATA));
        int check = GetCheck();
        bool result = (check == BST_CHECKED) ? true : false;
        pOptionInfo->Set(result);
        return 0;
    }
    
};


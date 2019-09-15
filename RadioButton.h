#pragma once
#include "ams/wtl/ControlFactory.h"
#include "Options.h"

GUID __declspec(selectany) GUIDRadioButton = // {EE5065D9-92FE-400f-8BD1-6C04416742BB}
{ 0xee5065d9, 0x92fe, 0x400f, { 0x8b, 0xd1, 0x6c, 0x4, 0x41, 0x67, 0x42, 0xbb } };


class RadioButton : public CWindowImpl<RadioButton, CButton>
{
public:
    BEGIN_MSG_MAP(RadioButton)
        MESSAGE_HANDLER_EX(WM_CONTROLINIT, OnControlInit)
        MESSAGE_HANDLER_EX(WM_CONTROLEXIT, OnControlExit)
    END_MSG_MAP()

    // This function is the response to WM_CONTROL_INIT. It initialise the control with the current
    // value from the options data structure. A pointer to the relevant option structure is held in
    // the window's user data and can be retrieved with GetWindowLong
    LRESULT OnControlInit(UINT msg, WPARAM wParam, LPARAM lParam)
    {
        OptionBase* pOptionInfo = reinterpret_cast<OptionBase*>(::GetWindowLongPtr(m_hWnd, GWL_USERDATA));
        int option = pOptionInfo->GetInt();
//        ::CheckRadioButton(GetParent(), pOptionInfo->GetIDD(), pOptionInfo->GetIDD() + option);
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

REGISTER_CONTROL(GUIDRadioButton, RadioButton);
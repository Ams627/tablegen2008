#pragma once
#include "ams/wtl/ControlFactory.h"
#include "../Options.h"

 GUID __declspec(selectany) GUIDEditBoxYear = // {7C517F5E-EAF7-4d69-BE67-1CB5CA71658F}
    { 0x7c517f5e, 0xeaf7, 0x4d69, { 0xbe, 0x67, 0x1c, 0xb5, 0xca, 0x71, 0x65, 0x8f } };


class UpDown: public CWindowImpl<UpDown, CUpDownCtrl>
{
public:
    BEGIN_MSG_MAP(UpDown)
    END_MSG_MAP();
};


class EditBoxYear : public CWindowImpl<EditBoxYear, CEdit>
{
    UpDown updown_;
public:
    EditBoxYear(void);
    BEGIN_MSG_MAP(EditBoxYear)
        MSG_WM_CHAR(OnChar)
        MESSAGE_HANDLER_EX(WM_CONTROLINIT, OnControlInit)
        MESSAGE_HANDLER_EX(WM_CONTROLEXIT, OnControlExit)
    END_MSG_MAP();
public:
    void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
    {
        nChar = nChar;
    }

    // This function is the response to WM_CONTROL_INIT. It initialises the control with the default
    // value from the options data structure. A pointer to the relevant option structure is held in
    // the window's user data and can be retrieved with GetWindowLong
    LRESULT OnControlInit(UINT msg, WPARAM wParam, LPARAM lParam)
    {
        long l = ::GetWindowLongPtr(m_hWnd, GWL_USERDATA);
        OptionBase* pOptionInfo = reinterpret_cast<OptionBase*>(::GetWindowLongPtr(m_hWnd, GWL_USERDATA));
        std::ostringstream oss;
        oss << pOptionInfo->GetInt();
        SetWindowText(oss.str().c_str());

        // Add an up down control to the right of the edit box:
        updown_.Create(GetParent(), CRect(0, 0, 10, 10), "", WS_VISIBLE|WS_CHILD|UDS_NOTHOUSANDS|UDS_ARROWKEYS|UDS_ALIGNRIGHT|UDS_SETBUDDYINT);
        updown_.SetRange32(pOptionInfo->GetMin(), pOptionInfo->GetMax());
        updown_.SetBuddy(*this);

        return 0;
    }

    LRESULT OnControlExit(UINT msg, WPARAM wParam, LPARAM lParam)
    {
        // get the option structure and store the integer from the edit box in it:
        OptionBase* pOptionInfo = reinterpret_cast<OptionBase*>(::GetWindowLongPtr(m_hWnd, GWL_USERDATA));
        int len = GetWindowTextLength();
        std::vector<char> buf(len + 1); 
#ifdef DEBUG
        std::generate_n(buf.begin(), len + 1, rand);
#endif
        RR GetWindowText(&buf[0], len + 1);
        int i = atoi(&buf[0]);
        pOptionInfo->Set(i);

        return 0;
    }

    virtual ~EditBoxYear(void);
};

REGISTER_CONTROL(GUIDEditBoxYear, EditBoxYear);
#pragma once

#define WM_LOSTFOCUS WM_APP + 101

class ListViewEdit : public CWindowImpl<ListViewEdit, CEdit>
{
public:
    BEGIN_MSG_MAP(ListViewEdit)
        MSG_WM_GETDLGCODE(OnGetDlgCode)
        MSG_WM_KILLFOCUS(OnKillFocus)
        MSG_WM_CHAR(OnChar)
    END_MSG_MAP()

    void OnKillFocus(HWND);
    void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
    LRESULT OnGetDlgCode(MSG* msg);
};

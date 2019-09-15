#include "stdafx.h"
#include "ListViewEdit.h"

//GUID __declspec(selectany) GUIDListViewEdit = { 0x0a8ae269, 0x1b07, 0x4939, {0xb9, 0xbd, 0xa6, 0xa4, 0x5d, 0x2d, 0xc3, 0xee} };
//REGISTER_CONTROL(GUIDListViewEdit, ListViewEdit);

void ListViewEdit::OnKillFocus(HWND)
{
    ::PostMessage(GetParent(), WM_LOSTFOCUS, 0, 0);
}

void ListViewEdit::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
    if (nChar == VK_ESCAPE)
    {
        ::PostMessage(GetParent(), WM_LOSTFOCUS, 0, 0);
        SetMsgHandled(TRUE);
    }
    if (nChar == VK_RETURN)
    {
        ::PostMessage(GetParent(), WM_LOSTFOCUS, 1, 0);
    }
    else
    {
        SetMsgHandled(FALSE);
    }
}


LRESULT ListViewEdit::OnGetDlgCode(MSG* msg)
{
    // this means we get all the keys pressed in the edit box instead of the dialog getting certain
    // accelerator keys like the escape key.
    return DLGC_WANTALLKEYS;
}
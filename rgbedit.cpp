#include "stdafx.h"
#include "rgbedit.h"


void RGBEdit::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
    if (isxdigit(nChar))
    {
        if (nChar >= 'a' && nChar <= 'f')
        {
//            nChar = nChar + 'A' - 'a';
            SetMsgHandled(TRUE);
            const MSG* pMsg = GetCurrentMessage();
            PostMessage(pMsg->message, nChar, pMsg->lParam);
        }
        else
        {
            SetMsgHandled(FALSE);
        }
    }
    else if (iscntrl(nChar))
    {
        SetMsgHandled(FALSE);
    }
    else
    {
        SetMsgHandled(TRUE);
    }
}

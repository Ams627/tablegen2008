#pragma once


class RGBEdit : public CWindowImpl<RGBEdit, CEdit>
{
public:
    BEGIN_MSG_MAP(RGBEdit)
        MSG_WM_CHAR(OnChar)
    END_MSG_MAP();

    void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
};
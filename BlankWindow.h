#pragma once

class BlankWindow : public CWindowImpl<BlankWindow,
    CWindow,
    CWinTraits<WS_VISIBLE|WS_CAPTION> >
{
public:
    BlankWindow(void);
    virtual ~BlankWindow(void);
    BEGIN_MSG_MAP(BlankWindow)
        MSG_WM_MOVE(func)
    END_MSG_MAP()

    void func(CPoint pnt)
    {
        pnt = pnt;
    }
};



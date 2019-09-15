#pragma once 
#include "ams\wtl\dialogfactory.h"

class BirthdayDialog: public FactoryDialog
{
public:
    BEGIN_MSG_MAP(BirthdayDialog)
        MSG_WM_INITDIALOG(OnInitDialog)
        REFLECT_NOTIFICATIONS();
    END_MSG_MAP()
    BirthdayDialog(void);
    virtual ~BirthdayDialog(void);

    LRESULT OnInitDialog(HWND hwnd, LPARAM lParam)
    {
        return TRUE;
    }
};
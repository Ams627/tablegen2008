#pragma once
#include "ams\wtl\dialogfactory.h"

class GeneralOptionsDlg: public FactoryDialog
{
public:
    BEGIN_MSG_MAP(GeneralOptionsDlg)
        MSG_WM_INITDIALOG(OnInitDialog)
    END_MSG_MAP()
    GeneralOptionsDlg(void);
    virtual ~GeneralOptionsDlg(void);

    LRESULT OnInitDialog(HWND hwnd, LPARAM lParam)
    {
        return TRUE;
    }
};

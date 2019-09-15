#pragma once
#include "ams\wtl\dialogfactory.h"

class ArseOptionsDialog: public FactoryDialog
{
public:
    BEGIN_MSG_MAP(ArseOptionsDialog)
        REFLECT_NOTIFICATIONS()
    END_MSG_MAP()
    ArseOptionsDialog(void);
public:
    LRESULT OnCtlColor(HDC, HWND);
    ~ArseOptionsDialog(void);
};

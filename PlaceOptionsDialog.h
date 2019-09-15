#pragma once
#include "ams\wtl\dialogfactory.h"

class PlaceOptionsDialog: public FactoryDialog
{
public:
    BEGIN_MSG_MAP(PlaceOptionsDialog)
        REFLECT_NOTIFICATIONS();
    END_MSG_MAP()
    PlaceOptionsDialog(void);
    virtual ~PlaceOptionsDialog(void);
};

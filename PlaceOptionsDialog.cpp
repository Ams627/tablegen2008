#include "stdafx.h"
#include "resource.h"
#include "PlaceOptionsDialog.h"

PlaceOptionsDialog::PlaceOptionsDialog(void)
{
    IDD = IDD_PLACEOPTIONS;
}

PlaceOptionsDialog::~PlaceOptionsDialog(void)
{
}

REGISTER_DIALOG(IDD_PLACEOPTIONS, PlaceOptionsDialog);
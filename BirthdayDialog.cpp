#include "stdafx.h"
#include "resource.h"
#include "BirthdayDialog.h"


BirthdayDialog::BirthdayDialog()
{
    IDD = IDD_BIRTHDAY;
}

BirthdayDialog::~BirthdayDialog()
{
}
REGISTER_DIALOG(IDD_BIRTHDAY, BirthdayDialog);
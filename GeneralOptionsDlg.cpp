#include "stdafx.h"
#include "resource.h"
#include "GeneralOptionsDlg.h"

GeneralOptionsDlg::GeneralOptionsDlg(void)
{
    IDD = IDD_GENERALOPTIONS;
}

GeneralOptionsDlg::~GeneralOptionsDlg(void)
{
}

REGISTER_DIALOG(IDD_GENERALOPTIONS, GeneralOptionsDlg);
#include "stdafx.h"
#include "resource.h"
#include "ArseOptionsDialog.h"

ArseOptionsDialog::ArseOptionsDialog(void)
{
    IDD = IDD_ARSEOPTIONS;
}

ArseOptionsDialog::~ArseOptionsDialog(void)
{
}

LRESULT ArseOptionsDialog::OnCtlColor(HDC, HWND)
{
    HBRUSH brush = CreateSolidBrush(RGB(100, 100, 100));
    return (LRESULT)brush;
}

REGISTER_DIALOG(IDD_ARSEOPTIONS, ArseOptionsDialog);
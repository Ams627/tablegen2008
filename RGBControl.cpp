#include "stdafx.h"
#include <ams/debugutils.h>
#include "RGBControl.h"

RGBControl::RGBControl(void)
{
}

RGBControl::~RGBControl(void)
{
}

#define GAP 40

LRESULT RGBControl::OnControlInit(UINT msg, WPARAM wParam, LPARAM lParam)
{
    // "Replace" the static control with 3 sliders (trackbars) and an edit control
    // You can either slide the sliders to control the colour or you can type
    // directly in the edit control.

    CRect staticRect;
    GetWindowRect(staticRect);
    HFONT dialogFont = GetFont();

    // Make the static control so small as to be invisible:
    SetWindowPos(NULL, 0, 0, 0, 0, SWP_NOMOVE|SWP_NOZORDER);

    CWindow dialogWindow = GetParent();
    dialogWindow.ScreenToClient(staticRect);

    HWND hwnd1 = redbar_.Create(dialogWindow, staticRect, "", WS_CHILD|WS_VISIBLE|TBS_TOOLTIPS, 0, 1);
    redbar_.SetRange(0, 255);
    redbar_.SetWindowLongPtr(GWL_USERDATA, (ULONG_PTR)m_hWnd);

    staticRect.OffsetRect(0, GAP);
    HWND hwnd2 = greenbar_.Create(dialogWindow, staticRect, "", WS_CHILD|WS_VISIBLE|TBS_TOOLTIPS, 0, 2);
    greenbar_.SetRange(0, 255);
    greenbar_.SetWindowLongPtr(GWL_USERDATA, (ULONG_PTR)m_hWnd);

    // Create the edit control that displays the full RGB value:
    CRect editRect(staticRect);
    editRect.OffsetRect(staticRect.right - staticRect.left + 20, 0);
    HWND rgbWnd = rgbEdit_.Create(dialogWindow, editRect, "", WS_CHILD|WS_VISIBLE|WS_TABSTOP|ES_UPPERCASE, WS_EX_CLIENTEDGE);
    rgbEdit_.SetFont(dialogFont);   // make the edit box have the same font as the dialog
    rgbEdit_.SetLimitText(6);       // six hex digits

    CRect colourSquareRect(editRect);
    colourSquareRect.OffsetRect(editRect.right - editRect.left + 20, 0);
    colourSquare_.Create(dialogWindow, colourSquareRect, "", WS_CHILD|WS_VISIBLE);

    staticRect.OffsetRect(0, GAP);
    HWND hwnd3 = bluebar_.Create(dialogWindow, staticRect, "", WS_CHILD|WS_VISIBLE|TBS_TOOLTIPS, 0, 3);
    bluebar_.SetRange(0, 255);
    bluebar_.SetWindowLongPtr(GWL_USERDATA, (ULONG_PTR)m_hWnd);
    return 0;
}

REGISTER_CONTROL(GUIDRGBControl, RGBControl);
#include "stdafx.h"
#include "SliderFromStatic.h"

SliderFromStatic::SliderFromStatic(void)
{
}

SliderFromStatic::~SliderFromStatic(void)
{
}

LRESULT SliderFromStatic::OnControlInit(UINT msg, WPARAM wParam, LPARAM lParam)
{
    // Make the static control invisible:
//    ModifyStyle(WS_VISIBLE, 0);
    CRect staticRect;
    GetWindowRect(staticRect);
    CWindow dialogWindow = GetParent();
    dialogWindow.ScreenToClient(staticRect);
    staticRect.left += 100;
    HWND hwnd = trackbar_.Create(dialogWindow, staticRect, "", WS_CHILD|WS_VISIBLE);
    return 0;
}

REGISTER_CONTROL(GUIDSliderFromStatic, SliderFromStatic);

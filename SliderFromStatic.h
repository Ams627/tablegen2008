#pragma once
#include "ControlFactory.h"
#include "Options.h"

GUID __declspec(selectany) GUIDSliderFromStatic = // {D3CB15E3-B466-4b79-AA44-67A2B988EC51}
    { 0xd3cb15e3, 0xb466, 0x4b79, { 0xaa, 0x44, 0x67, 0xa2, 0xb9, 0x88, 0xec, 0x51 } };
 
namespace {
class MyTrackBarCtrl : public CWindowImpl<MyTrackBarCtrl, CTrackBarCtrl>
{
public:
    DECLARE_EMPTY_MSG_MAP();
};
}


// Subclass a static control so we can make it invisible then create (using CreateWindow)
// a group of sliders in its place
class SliderFromStatic : public CWindowImpl<SliderFromStatic, CStatic>
{
    MyTrackBarCtrl trackbar_;
    BEGIN_MSG_MAP(SliderFromStatic)
        MESSAGE_HANDLER_EX(WM_CONTROLINIT, OnControlInit)
    END_MSG_MAP();
public:
    LRESULT OnControlInit(UINT msg, WPARAM wParam, LPARAM lParam);
    SliderFromStatic(void);
public:
    virtual ~SliderFromStatic(void);
};


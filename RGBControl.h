#pragma once
#include <ams/debugutils.h>
#include "ams/wtl/ControlFactory.h"
#include "Options.h"
#include "rgbedit.h"

namespace {

GUID __declspec(selectany) GUIDRGBControl =// {F40D9A52-4672-4b60-9726-166E0295AFEE}
    { 0xf40d9a52, 0x4672, 0x4b60, { 0x97, 0x26, 0x16, 0x6e, 0x2, 0x95, 0xaf, 0xee } };


class MyTrackBarCtrl : public CWindowImpl<MyTrackBarCtrl, CTrackBarCtrl>
{
public:
    BEGIN_MSG_MAP(MyTrackBarCtrl)
        MSG_OCM_HSCROLL(OnHScroll)
    END_MSG_MAP();
    void OnHScroll(int, SHORT, HWND)
    {
        const MSG* msg = GetCurrentMessage();
        HWND hwnd = (HWND)GetWindowLongPtr(GWL_USERDATA);
        OutputDebugString("sending to hwnd:...");
        OUTPUT(hwnd);
        ::SendMessage(hwnd, msg->message, msg->wParam, msg->lParam);
    }
};
}

class ColourSquare : public CWindowImpl<ColourSquare, CStatic>
{
    DWORD colour_;
public:
    BEGIN_MSG_MAP(ColourSquare)
        MSG_WM_PAINT(OnPaint)
    END_MSG_MAP();

    void OnPaint(CDC)
    {
        //  NOTE:  The device context passed in does not exist.  This is a WTL bug
        PAINTSTRUCT paint;
        CDC dc = BeginPaint(&paint);
        CRect rect;
        GetClientRect(rect);
        dc.FillSolidRect(rect.left, rect.top, rect.Width(), rect.Height(), colour_);
        EndPaint(&paint);
    }

    void SetColour(const DWORD colour)
    {
        colour_ = colour;
    }
};

class RGBControl : public CWindowImpl<RGBControl, CStatic>
{
    MyTrackBarCtrl redbar_, greenbar_, bluebar_;
    RGBEdit rgbEdit_;
    ColourSquare colourSquare_;
public:
    BEGIN_MSG_MAP(RGBControl)
        MESSAGE_HANDLER_EX(WM_CONTROLINIT, OnControlInit)
        MSG_OCM_HSCROLL(OnHScroll) // Reflected HSCROLL message from trackbars
    END_MSG_MAP();

    inline void SetEditFromTrackBars()
    {
        DWORD rgbValue = (bluebar_.GetPos() << 16) + (greenbar_.GetPos() << 8) + redbar_.GetPos();
        std::stringstream rgbstream;
        rgbstream << std::hex << std::uppercase << std::setfill('0') << std::setw(6) << rgbValue;
        rgbEdit_.SetWindowText(rgbstream.str().c_str());
        colourSquare_.SetColour(rgbValue);
        colourSquare_.Invalidate();
    }

    void OnHScroll(int code, SHORT pos, HWND hwnd)
    {
        if (code == TB_THUMBTRACK || code == TB_LINEDOWN || code == TB_LINEUP ||
            code == TB_TOP || code == TB_BOTTOM ||
            code == TB_PAGEDOWN || code == TB_PAGEUP)
        {
            SetEditFromTrackBars();
        }
        OUTPUT(code);
        OUTPUT(pos);
    }

    RGBControl(void);
    LRESULT OnControlInit(UINT msg, WPARAM wParam, LPARAM lParam);
    virtual ~RGBControl(void);
};

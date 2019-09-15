#pragma once
#define WM_DUMMY WM_APP + 100
#include "ListViewEdit.h"


// Header Control - this control subclasses the list controls' built in header control
class MyHeaderCtrl : public CWindowImpl<MyHeaderCtrl, CHeaderCtrl>
{
    int lButtonDownXAdjust;

    BEGIN_MSG_MAP(MyHeaderCtrl)
//        MSG_WM_MOUSEMOVE(OnMouseMove)
        MSG_WM_SETCURSOR(OnSetCursor)
         MSG_WM_LBUTTONDOWN(OnLButtonDown)
    END_MSG_MAP()

    MyHeaderCtrl() : lButtonDownXAdjust(0) {};

    LRESULT OnMouseMove(UINT, CPoint)
    {
        SetMsgHandled(TRUE);
        return 0;
    }
    LRESULT OnSetCursor(HWND, UINT, UINT)
    {
        CPoint point;
        GetCursorPos(&point);
        ScreenToClient(&point);
        HDHITTESTINFO hdHitTest;
        hdHitTest.pt = point;
        LRESULT lResult = SendMessage(HDM_HITTEST, 0, reinterpret_cast<LPARAM>(&hdHitTest));
        
        if (hdHitTest.flags == HHT_ONDIVIDER || hdHitTest.flags == HHT_ONDIVOPEN)
        {
            HCURSOR weCursor = LoadCursor(NULL, IDC_SIZEWE);
            HCURSOR oldCursor = SetCursor(weCursor);
        }
        else
        {
            HCURSOR arrowCursor = LoadCursor(NULL, IDC_ARROW);
            HCURSOR oldCursor = SetCursor(arrowCursor);
        }
        SetMsgHandled(TRUE);

        return 0;
    }
    LRESULT OnLButtonDown(UINT nFlags, CPoint point)
    {
        HDHITTESTINFO hdHitTest;
        hdHitTest.pt = point;
        LRESULT lResult = SendMessage(HDM_HITTEST, 0, reinterpret_cast<LPARAM>(&hdHitTest));

        if (hdHitTest.flags == HHT_ONDIVIDER)
        {
            CRect rect;
            LRESULT lResult = SendMessage(HDM_GETITEMRECT, hdHitTest.iItem, reinterpret_cast<LPARAM>(static_cast<LPRECT>(rect)));
            OUTPUT(rect.left);
            OUTPUT(rect.top);
            OUTPUT(rect.Width());
            OUTPUT(rect.Height());

            if (rect.Width() == 0 && hdHitTest.iItem > 0)
            {
                int currentItem = hdHitTest.iItem;

                while (hdHitTest.iItem == currentItem) 
                {
                    OutputDebugString("Item: ");
                    OUTPUT(hdHitTest.iItem);
                    hdHitTest.pt.x--;
                    LRESULT lResult = SendMessage(HDM_HITTEST, 0, reinterpret_cast<LPARAM>(&hdHitTest));
                }
                PostMessage(WM_LBUTTONUP, 0, MAKELPARAM(hdHitTest.pt.x, hdHitTest.pt.y));
                PostMessage(WM_LBUTTONDOWN, 0, MAKELPARAM(hdHitTest.pt.x, hdHitTest.pt.y));
                SetMsgHandled(TRUE);
            }
        }   
        if (hdHitTest.flags == HHT_ONDIVOPEN)
        {
            while (hdHitTest.flags == HHT_ONDIVOPEN) 
            {
                OUTPUT(hdHitTest.pt.x);
                OUTPUT(hdHitTest.pt.y);
                OUTPUT(hdHitTest.iItem);
                hdHitTest.pt.x--;
                lButtonDownXAdjust++;
                LRESULT lResult = SendMessage(HDM_HITTEST, 0, reinterpret_cast<LPARAM>(&hdHitTest));
            }
            OutputDebugString("No longer on DIV OPEN\n");

            int currentItem = hdHitTest.iItem;

            while (hdHitTest.iItem == currentItem) 
            {
                OUTPUT(hdHitTest.pt.x);
                OUTPUT(hdHitTest.pt.y);
                OUTPUT(hdHitTest.iItem);
                hdHitTest.pt.x--;
                lButtonDownXAdjust++;
                LRESULT lResult = SendMessage(HDM_HITTEST, 0, reinterpret_cast<LPARAM>(&hdHitTest));
            }

            SetMsgHandled(TRUE);
            //ReleaseCapture();
            CPoint cursorpos(hdHitTest.pt.x, hdHitTest.pt.y);
            ClientToScreen(&cursorpos);
            SetCursorPos(cursorpos.x, cursorpos.y);
            PostMessage(WM_LBUTTONUP, 0, MAKELPARAM(hdHitTest.pt.x, hdHitTest.pt.y));
            PostMessage(WM_LBUTTONDOWN, 0, MAKELPARAM(hdHitTest.pt.x, hdHitTest.pt.y));
        }
        else
        {
            hdHitTest = hdHitTest;
            OutputDebugString("Normal OnLButtonDown\n");
            OUTPUT(hdHitTest.pt.x);
            OUTPUT(hdHitTest.pt.y);
            ClientToScreen(&point);
            SetCursorPos(point.x + lButtonDownXAdjust, point.y);
            lButtonDownXAdjust = 0;
            SetMsgHandled(FALSE);
        }
        return 0;
    }
};

class GeneralLVControl : public CWindowImpl<GeneralLVControl, CListViewCtrl>
{
    std::string widthFilename_;
    int numberOfCols_;
    CMenu rightClickMenu_;
    MyHeaderCtrl headerCtrl_;
    bool tracking_;
    bool editing_;
    std::vector<int> editable_;
    ListViewEdit lvedit_;

    BEGIN_MSG_MAP(GeneralLVControl)
//        MSG_WM_NOTIFY(OnNotify)
        NOTIFY_CODE_HANDLER_EX(HDN_ENDTRACKA, OnEndTrack)
        NOTIFY_CODE_HANDLER_EX(HDN_ENDTRACKW, OnEndTrack)
        NOTIFY_CODE_HANDLER_EX(HDN_BEGINTRACKA, OnBeginTrack)
        NOTIFY_CODE_HANDLER_EX(HDN_BEGINTRACKW, OnBeginTrack)
        NOTIFY_CODE_HANDLER_EX(HDN_ENDDRAG, OnEndDrag)
        NOTIFY_CODE_HANDLER_EX(HDN_ITEMCHANGINGW, OnItemChanging)
        NOTIFY_CODE_HANDLER_EX(HDN_ITEMCHANGINGA, OnItemChanging)
        MESSAGE_HANDLER_EX(WM_DUMMY, OnDummyMessage)
        MSG_WM_CONTEXTMENU(OnContextMenu)
        MSG_WM_COMMAND(OnCommand)
        MSG_WM_LBUTTONDOWN(OnLButtonDown)
        MSG_WM_LBUTTONDBLCLK(OnLButtonDblClk)
        MSG_WM_HSCROLL(OnHScroll)
        MSG_WM_VSCROLL(OnVScroll)
        MSG_WM_MOUSEWHEEL(OnMouseWheel)
        MESSAGE_HANDLER_EX(WM_LOSTFOCUS, OnLostFocus);
//         MSG_WM_MOUSEMOVE(OnMouseMove);
    END_MSG_MAP();

    GeneralLVControl() : tracking_(false), editing_(false) {}

    virtual void GetEditItemString(std::string&, int item, int subitem);
    void SetEditable(int*, int);
    void OnContextMenu(HWND, CPoint);
    LRESULT OnBeginTrack(NMHDR* pNMHDR);
    LRESULT OnEndTrack(NMHDR* pNMHDR); // a divider got dragged thus a column now has a new width
    LRESULT OnEndDrag(NMHDR* pNMHDR);  // a column got dragged into a new position
    LRESULT OnNotify(int, LPNMHDR);
    LRESULT OnItemChanged(NMHDR* pNMHDR);
    LRESULT OnItemChanging(NMHDR* pNMHDR);
    void WriteWidthFile();
    void ReadWidthFile();
    void Init(std::string filename);
    LRESULT OnDummyMessage(UINT msg, WPARAM wParam, LPARAM lParam);
    LRESULT OnCommand(UINT, int, HWND);
    LRESULT OnMouseMove(UINT, CPoint);
    void OnLButtonDblClk(UINT nFlags, CPoint point);
    void OnLButtonDown(UINT nFlags, CPoint point);
    LRESULT OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
    LRESULT OnLostFocus(UINT msg, WPARAM wParam, LPARAM lParam);
    void StopEditing();
    virtual void OnEndEdit(char* s);
    void OnHScroll(UINT nSBCode, UINT nPos, HWND hScrollBar);
    void OnVScroll(UINT nSBCode, UINT nPos, HWND hScrollBar);
};
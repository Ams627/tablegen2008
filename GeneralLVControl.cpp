#include "stdafx.h"
#include "GeneralLVControl.h"

namespace {
    const int MaxColumns = 50;
    const int MenuBaseID = 1024;
    const int MinColumnWidth = 30;
    const int HIDEMASK = 0x8000;
    const WPARAM wParamEndTrack = 0;
    const WPARAM wParamEndDrag = 1;
    short columnWidths[MaxColumns] = {0};
}

void GeneralLVControl::Init(std::string filename)
{
    // All columns are editable until the editable mask is set:
    std::fill(editable_.begin(), editable_.end(), 1);

    // make the filename of the width file for this list view control
    // it will probably be a file under C:\Documents and Settings\<USER NAME>\Application Data\w-suite
    widthFilename_= g_dirs.GetAppData();
    widthFilename_ += filename;

    // Get the number of columns in this list view control - we can only get this from the
    // header:
    CHeaderCtrl& headerCtrl = GetHeader();
    headerCtrl_.SubclassWindow(headerCtrl);

    numberOfCols_ = headerCtrl_.GetItemCount();

    // Make a menu containing all the column names:
    rightClickMenu_.CreatePopupMenu();
    // make the right click menu to show and hide columns for the header
    for (int i = 0; i < numberOfCols_; ++i)
    {
        HDITEM hditem;
        std::vector<TCHAR> labelText(256); // maximum size of label's text
        hditem.mask = HDI_TEXT|HDI_WIDTH; // get the text of the column
        hditem.cchTextMax = labelText.size();
        hditem.pszText = &labelText[0];
        RR headerCtrl_.GetItem(i, &hditem);

        UINT mask;
        if (hditem.cxy == 0)
        {
            mask = MF_STRING|MF_UNCHECKED;
        }
        else
        {
            mask = MF_STRING|MF_CHECKED;
        }
        rightClickMenu_.AppendMenu(mask, UINT_PTR(i + MenuBaseID), hditem.pszText);
    }
    rightClickMenu_.AppendMenu(MF_STRING|MF_UNCHECKED, UINT_PTR(0), "Reset Default");
    rightClickMenu_.AppendMenu(MF_STRING|MF_UNCHECKED, UINT_PTR(0), "Show All");

    // If the width file exists then read in the column widths and set them:
    if (GetFileAttributes(widthFilename_.c_str()) != INVALID_FILE_ATTRIBUTES)
    {
        ReadWidthFile();
        for (int i = 0; i < numberOfCols_; ++i)
        {
            unsigned short newWidth = columnWidths[i];
            if (columnWidths[i] & HIDEMASK)
            {
                RR SetColumnWidth(i, 0);
            }
            else
            {
                if (columnWidths[i] < MinColumnWidth)
                {
                    columnWidths[i]= MinColumnWidth;
                }
                RR SetColumnWidth(i, columnWidths[i]);
            }
        }
    }
    else // the width file does not exist if we get to this point
    {
        // store the current column widths in the column width array so that the array 
        // can be written to a file if one of the widths gets changed:
        for (int i = 0; i < numberOfCols_; i++)
        {
            HDITEM hditem;
            hditem.mask = HDI_WIDTH; // get the width of the column
            RR headerCtrl_.GetItem(i, &hditem);

            // 
            if (hditem.cxy < MinColumnWidth)
            {
                SetColumnWidth(i, MinColumnWidth);
                columnWidths[i] = MinColumnWidth;
            }
            else
            {
                columnWidths[i] = hditem.cxy;
            }
        }
    }
}

void GeneralLVControl::ReadWidthFile()
{
    std::ostringstream debugss;
    debugss << "Reading width file " << widthFilename_ << std::endl;
    OutputDebugString(debugss.str().c_str());
 
    std::ifstream fin(widthFilename_.c_str(), std::ios::binary);
    if (fin)
    {
        for(int i = 0; i < sizeof(columnWidths) / sizeof(columnWidths[0]); i++)
        {
            fin.read(reinterpret_cast<char*>(&columnWidths[i]), sizeof(columnWidths[0]));  
        }

        std::vector<int> columnorder(numberOfCols_);
        for(size_t i = 0; i < columnorder.size(); i++)
        {
            char c;
            fin.get(c);
            columnorder[i] = static_cast<int>(static_cast<unsigned char>(c));
        }
        fin.close();
        CHeaderCtrl& headerCtrl = GetHeader();
        headerCtrl.SetOrderArray(columnorder.size(), &columnorder[0]);
        std::ostringstream debugss;
        debugss << "Finished reading width file " << std::endl;
        OutputDebugString(debugss.str().c_str());
        
    }
    else
    {
        throw std::exception("Cannot open widths file for reading");
    }
}

void GeneralLVControl::WriteWidthFile()
{
    std::ostringstream debugss;
    debugss << "Writing width file " << widthFilename_ << std::endl;
    OutputDebugString(debugss.str().c_str());

    std::vector<int> columnOrder(numberOfCols_);
    CHeaderCtrl& headerCtrl = GetHeader();
    headerCtrl.GetOrderArray(columnOrder.size(), &columnOrder[0]);

//     debugss.str("");
//     debugss << "order array: ";
//     for (size_t i = 0; i < columnOrder.size(); ++i)
//     {
//         debugss << columnOrder[i] << " ";
//     }
//     debugss << "\n";
//     OutputDebugString(debugss.str().c_str());


    std::ofstream fout(widthFilename_.c_str(), std::ios::binary);
    if (fout)
    {
        // write the column widths:
        for(int i = 0; i < sizeof(columnWidths) / sizeof(columnWidths[0]); i++)
        {
//             std::ostringstream debugss;
//             debugss << "writing " << sizeof(columnWidths[0]) << " bytes: ";
//             for (int j = 0; j < sizeof(columnWidths[0]); j++)
//             {
//                 BYTE* p = reinterpret_cast<BYTE*>(&columnWidths[i]);
//                 debugss << std::hex << std::uppercase << std::setfill('0') << std::setw(2) << int(p[j]) << ' ';
//             }
//             debugss << std::endl;
//             OutputDebugString(debugss.str().c_str());
            fout.write(reinterpret_cast<char*>(&columnWidths[i]), sizeof(columnWidths[0]));
        }

        // write the column order array and add dummy zeroes to the end of the file:
        for(size_t i = 0; i < columnOrder.size(); ++i)
        {
            fout.put(static_cast<char>(columnOrder[i] & 0xFF));
        }
        for (size_t i = 0; i < MaxColumns - columnOrder.size(); ++i)
        {
            fout.put(0);
        }

        fout.close();

        debugss.str("");
        debugss << "Finished writing width file " << std::endl;
        OutputDebugString(debugss.str().c_str());
    }
    else
    {
        throw std::exception("cannot open file for writing");
    }
}

LRESULT GeneralLVControl::OnBeginTrack(NMHDR* pNMHDR)
{
    LRESULT result = FALSE;
    tracking_ = true;
    NMHEADER *pHeader = reinterpret_cast<NMHEADER*>(pNMHDR);
    HDITEM *pItem = pHeader->pitem;
    int width = pItem->cxy;

    // Don't allow tracking out of zero width items:
    if (width == 0)
    {
        result = TRUE;
    }

    return result;
}


LRESULT GeneralLVControl::OnEndTrack(NMHDR* pNMHDR)
{
    // Get new width of header item:
    NMHEADER *pHeader = reinterpret_cast<NMHEADER*>(pNMHDR);
    HDITEM *pItem = pHeader->pitem;
    int width = pItem->cxy;

    // Get the index of the header item and store in the widths array:
    int item = pHeader->iItem;
    columnWidths[item] = width;

    // This message ends the tracking operation - this is sent because windows must think the tracking
    // operation is over before we assume it is over.
    PostMessage(WM_DUMMY, wParamEndTrack, 0);
    return FALSE;
}

LRESULT GeneralLVControl::OnEndDrag(NMHDR* pNMHDR)
{
// debug:
    std::vector<int> order(numberOfCols_);
    CHeaderCtrl& headerCtrl = GetHeader();
    headerCtrl.GetOrderArray(order.size(), &order[0]);
    std::ostringstream ss;
    for (size_t i = 0; i < order.size(); i++)
    {
        ss << order[i] << " ";
    }
    ss << std::endl;
    OutputDebugString(ss.str().c_str());
// end debug

    // OK - this IS weird but it's microsoft's fault - the column order array is not updated
    // at this point so we have to post another message in the queue - when that message gets
    // processed the column order array will be consistent with the recent drag operation:
    PostMessage(WM_DUMMY, wParamEndDrag, 0); 

//    Invalidate();
    return 0;
}



LRESULT GeneralLVControl::OnDummyMessage(UINT msg, WPARAM wParam, LPARAM lParam)
{
    if (wParam == wParamEndTrack)
    {
        tracking_ = false;
    }
    WriteWidthFile();
    return 0;
}


void GeneralLVControl::OnContextMenu(HWND, CPoint point)
{
    CHeaderCtrl& ctrl = GetHeader();
    CRect headerRect;
    ctrl.GetWindowRect(headerRect);

    if (headerRect.PtInRect(point))
    {
        rightClickMenu_.TrackPopupMenu(TPM_RIGHTALIGN | TPM_RIGHTBUTTON,
            point.x,
            point.y,
            *this);
    }
}

LRESULT GeneralLVControl::OnItemChanging(NMHDR* pNMHDR)
{
    LRESULT result = FALSE;
    NMHEADER *pHeader = reinterpret_cast<NMHEADER*>(pNMHDR);
    HDITEM *pItem = pHeader->pitem;
    if (pItem->mask & HDI_WIDTH)
    {
        // don't allow the column width to become less than the minimum unless we are initialising
        // the control (in which case we need to allow zero to indicate the column is hidden)
        if (tracking_ && pItem->cxy < MinColumnWidth)
        {
            result = TRUE;
        }
        else if (pItem->cxy < MinColumnWidth)
        {
            pItem = pItem;
        }
    }
    return result;
}


// WM_COMMAND sent from the menu which appears when the user right-clicks on the header:
LRESULT GeneralLVControl::OnCommand(UINT code, int id, HWND hWnd)
{
    if (id >= MenuBaseID && id < MenuBaseID + numberOfCols_)
    {
        int column = id - MenuBaseID;
        if (columnWidths[column] & HIDEMASK)
        {
            columnWidths[column] &= ~HIDEMASK;
            RR SetColumnWidth(column, columnWidths[column]);
            rightClickMenu_.CheckMenuItem(id, MF_CHECKED|MF_BYCOMMAND);
        }
        else
        {
            RR SetColumnWidth(column, 0);
            columnWidths[column] |= HIDEMASK;
            rightClickMenu_.CheckMenuItem(id, MF_UNCHECKED|MF_BYCOMMAND);
        }
        WriteWidthFile();
    }
    return 0;
}

LRESULT GeneralLVControl::OnMouseMove(UINT, CPoint point)
{
    return 0;
}

LRESULT GeneralLVControl::OnNotify(int code, LPNMHDR pNMHDR)
{
    OUTPUT_NM(pNMHDR->code);  
    return 0;
}


void GeneralLVControl::SetEditable(int* p, int count)
{
    std::copy(p, p + count, std::back_inserter(editable_));   
}


void GeneralLVControl::OnLButtonDblClk(UINT nFlags, CPoint point)
{
    LVHITTESTINFO lvHitTestInfo;
    lvHitTestInfo.pt = point;
    int i = SubItemHitTest(&lvHitTestInfo);
    OUTPUT(i);
    OUTPUT(lvHitTestInfo.iItem);
    OUTPUT(lvHitTestInfo.iSubItem);

    OutputDebugString("OnLButtonDblClk\n");
    if (editing_)
    {
        StopEditing();
    }
    else
    {
        if (editable_[lvHitTestInfo.iSubItem])
        {
            editing_ = true;

            CRect rect;
            OUTPUT(lvHitTestInfo.iItem);
            OUTPUT(lvHitTestInfo.iSubItem);
            if (lvHitTestInfo.iSubItem == 0)
            {
                GetItemRect(lvHitTestInfo.iItem, rect, LVIR_BOUNDS);
                CRect itemHeaderRect;
                CHeaderCtrl& ctrl= GetHeader();
                ctrl.GetItemRect(0,itemHeaderRect);
                rect.left = itemHeaderRect.left;
                rect.right = itemHeaderRect.right;
                OUTPUTRECT(itemHeaderRect);
                OUTPUTRECT(rect);
            }
            else
            {
                GetSubItemRect(lvHitTestInfo.iItem, lvHitTestInfo.iSubItem, LVIR_BOUNDS, rect);
            }
            lvedit_.Create(*this, &rect, "", WS_CHILD|WS_VISIBLE|ES_SUNKEN|ES_AUTOHSCROLL, WS_EX_CLIENTEDGE);
            lvedit_.SetFocus();

            // Make the edit box font the same as the listview font:
            HFONT listviewfont = reinterpret_cast<HFONT>(SendMessage(WM_GETFONT, 0, 0));
            lvedit_.SetFont(listviewfont);

            // Set the initial text in the edit box:
            std::string text;
            GetEditItemString(text, lvHitTestInfo.iItem, lvHitTestInfo.iSubItem);
            lvedit_.SetWindowText(text.c_str());
        }
    }
}

void GeneralLVControl::OnLButtonDown(UINT nFlags, CPoint point)
{
    OutputDebugString("OnLButtonDown\n");
    StopEditing();
    SetMsgHandled(FALSE);
}


LRESULT GeneralLVControl::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
    OutputDebugString("OnMouseWheel\n");
    StopEditing();
    SetMsgHandled(FALSE);
    return 0;
}


LRESULT GeneralLVControl::OnLostFocus(UINT msg, WPARAM wParam, LPARAM lParam)
{
    if (wParam == 1) // User pressed enter
    {
        int l = lvedit_.GetWindowTextLength();
        std::vector<char> text(l);
        RR lvedit_.GetWindowText(&text[0], l);
        OnEndEdit(&text[0]);
    }
    OutputDebugString("OnLostFocus\n");
    StopEditing();
    SetMsgHandled(FALSE);
    return 0;
}


void GeneralLVControl::StopEditing()
{
    if (editing_)
    {
        editing_ = false;
        lvedit_.DestroyWindow();
        SetFocus();
        Invalidate();
    }
}


void GeneralLVControl::OnHScroll(UINT nSBCode, UINT nPos, HWND hScrollBar)
{
    StopEditing();
    SetMsgHandled(FALSE);
}

void GeneralLVControl::OnVScroll(UINT nSBCode, UINT nPos, HWND hScrollBar)
{
    StopEditing();
    SetMsgHandled(FALSE);
}


void GeneralLVControl::GetEditItemString(std::string& x, int item, int subitem)
{
    x = "override this";
}

void GeneralLVControl::OnEndEdit(char *s)
{

}
#include "stdafx.h"
#include <ams/BrowseForFolder.h>
#include <ams/errorutils.h>
#include "OptionFrameDlg.h"
#include "options.h"
#include "ams/wtl/ControlFactory.h"

extern CAppModule _Module;

BOOL CALLBACK EnumChildProc(HWND hwnd, LPARAM lParam);
BOOL CALLBACK EnumChildProcDebug(HWND hwnd, LPARAM lParam);

LRESULT OptionFrameDialog::OnInitDialog(HWND hwnd, LPARAM lParam)
{
//    _Module.GetMessageLoop()->AddMessageFilter(this);
   

    m_tree.SubclassWindow(GetDlgItem(IDC_TREE));
    currentDialog_ = 0;

    RECT outerrect, okrect, treerect;

    // Get the treeview rectangle:
    RR m_tree.GetWindowRect(&treerect);
    RR ScreenToClient(&treerect);
    childXOffset_ = treerect.right;
    childYOffset_ = treerect.top;

    // Get the top of the outer dialog:
    RR GetWindowRect(&outerrect);

    // Get the top of the OK button
    CWindow OKButton = GetDlgItem(IDOK);
    RR OKButton.GetWindowRect(&okrect);
    RR ScreenToClient(&okrect);

    // Calculate the height of the child dialogs so that they don't cover the OK / Cancel / Apply buttons
    childHeight_ = okrect.top - treerect.top;

    // Insert items into the tree control and create a dialog for each item
    // Initially all dialogs are invisible

    int maxDlgWidth = 0;
    for (int i = 0; i < entries_; ++i)
    {
        TVINSERTSTRUCT tvi;
        tvi.hInsertAfter = NULL;
        tvi.hParent = NULL;
        tvi.item.mask = TVIF_HANDLE|TVIF_PARAM|TVIF_TEXT|TVIF_STATE;
        tvi.item.pszText = pDialogItem_[i].name;

        tvi.item.stateMask = TVIS_SELECTED;
        if (i == 0)
        {
            tvi.item.state = TVIS_SELECTED;
        }
        else
        {
            tvi.item.state = 0;
        }
        FactoryDialog* dlg = CreateFactoryDialog(pDialogItem_[i].idd);
        HWND hwnd = dlg->Create(m_hWnd);
        // Make dialog invisible and a child window:
        dlg->ModifyStyle(WS_VISIBLE, WS_CHILD);
        dialogHWNDs_[i] = hwnd;
        RECT rect;
        dlg->GetWindowRect(&rect);
        int width = rect.right - rect.left;
        if (width > maxDlgWidth)
        {
            maxDlgWidth = width;
        }
        dlg->MoveWindow(childXOffset_, childYOffset_, width, childHeight_);
        tvi.item.lParam = (LPARAM)dlg;
        m_tree.InsertItem(&tvi);
    }
    
    m_tree.SetFocus();
    SetWindowPos(NULL,
                 200,
                 200,
                 maxDlgWidth + childXOffset_,
                 outerrect.bottom - outerrect.top, 
                 SWP_NOZORDER);

    SubclassControls();

    return FALSE;
}


class CMyWindow : public CWindowImpl<CMyWindow, CWindow>
{

};

void OptionFrameDialog::SubclassControls()
{
    // Create all the windows in the options map from their GUIDs using the control factory,
    // then Subclass the dialog controls to the newly created windows. Basically we are replacing
    // the WNDPROCS for each dialog control with the WNDPROCs for the created window.

    // Create new map that maps idds to the child HWNDs
    std::map<UINT, HWND> iddmap;

#ifdef DEBUG
    // Check that every IDD in resource.h is unique - only in debugging build
    std::map<UINT, int> iddDebugMap_;
    // Enumerate child windows of each dialog.
    for(int i = 0; i < entries_; ++i)
    {
        EnumChildWindows(dialogHWNDs_[i], EnumChildProcDebug, (LPARAM)&iddDebugMap_);
    }
#endif

    // Enumerate child windows of each dialog.
    for(int i = 0; i < entries_; ++i)
    {
        EnumChildWindows(dialogHWNDs_[i], EnumChildProc, (LPARAM)&iddmap);
    }

    // Create the control type required by the options map, then call
    // subclasswindow to subclass the dialog window.
    // Note - do not change the (CMyWindow*) cast to another type of 
    // cast - it will not work!
    for (OptMap::iterator p = g_optMap.begin(); p != g_optMap.end(); ++p)
    {
        CMyWindow *pWindow = (CMyWindow*)CreateFactoryControl(p->second->GetWindowType());
        pWindow->SubclassWindow(iddmap[p->second->GetIDD()]);
        p->second->SetHWND(pWindow->m_hWnd);

        UINT idd = p->second->GetIDD();

        // Store a pointer to the option data for the window to use itself in the windows user data:
        ::SetWindowLongPtr(pWindow->m_hWnd, GWL_USERDATA, reinterpret_cast<LONG_PTR>(p->second));


        // Send a message to the control so that it can initialise itself:
        pWindow->SendMessage(WM_CONTROLINIT, 0, 0);
    }
}

BOOL CALLBACK EnumChildProc(HWND hwnd, LPARAM lParam)
{
    std::map<UINT, HWND> *piddmap = (std::map<UINT, HWND>*)lParam;
    UINT idd = GetDlgCtrlID(hwnd);
    (*piddmap)[idd] = hwnd;
    return TRUE;
}

#ifdef DEBUG
BOOL CALLBACK EnumChildProcDebug(HWND hwnd, LPARAM lParam)
{
    std::map<UINT, int> *piddmap = (std::map<UINT, int>*)lParam;
    UINT idd = GetDlgCtrlID(hwnd);
    (*piddmap)[idd]++;
    if ((*piddmap)[idd] > 1 && idd < 0x80000000)
    {
        std::ostringstream oss;
        oss << "Error: Dialog control IDs must be unique throughout the project\n" << idd << " is not unique. (hwnd = " << hwnd << ")Modify resource.h";
        ::MessageBox(NULL, oss.str().c_str(), "Control ID error", MB_OK);
    }
    return TRUE;
}
#endif


LRESULT OptionFrameDialog::OnSelChanged(int /*idCtrl*/, LPNMHDR pNMHDR, BOOL& /*bHandled*/)
{
    NMTREEVIEW *ptv = (NMTREEVIEW*)pNMHDR;
    FactoryDialog* pOldDlg = (FactoryDialog*)ptv->itemOld.lParam;
    FactoryDialog* pNewDlg = (FactoryDialog*)ptv->itemNew.lParam;

    if (pOldDlg && ::IsWindow(pOldDlg->m_hWnd))
    {
        pOldDlg->ShowWindow(SW_HIDE);
    }
    pNewDlg->ShowWindow(SW_SHOW);

    return 0;
}



LRESULT OptionFrameDialog::OnOK(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
    // Get the data out of the controls and into the option structures:
    for (OptMap::iterator p = g_optMap.begin(); p != g_optMap.end(); ++p)
    {
        HWND wnd = p->second->GetHWND();
        ::SendMessage(wnd, WM_CONTROLEXIT, 0, 0);
    }
    EndDialog(1);
    return 0;
}

LRESULT OptionFrameDialog::OnCancel(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
    EndDialog(0);
    return 0;
}

LRESULT OptionFrameDialog::OnTvnItemexpandedTree(int /*idCtrl*/, LPNMHDR pNMHDR, BOOL& /*bHandled*/)
{
    LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
    // TODO: Add your control notification handler code here

    return 0;
}


BOOL OptionFrameDialog::PreTranslateMessage(MSG* pMsg)
{
    OUTPUT(pMsg->message);
    return 0;
}


// BOOL OptionFrameDialog::ProcessWindowMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam,
//                                   LRESULT& lResult, DWORD dwMsgMapID)
// {
//     OUTPUT(uMsg);
// }

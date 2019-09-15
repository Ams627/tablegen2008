#pragma once
#include <ams/wtl/dialogfactory.h>
#include "resource.h"

struct OptDialogItem {
    char* name;
    UINT idd;
};

class CMyTree : public CWindowImpl<CMyTree, CTreeViewCtrl>
{public:
    BEGIN_MSG_MAP(CMyTree)
    END_MSG_MAP()
};

class OptionFrameDialog: public CDialogImpl<OptionFrameDialog>
{
    CMyTree m_tree;
    const OptDialogItem* pDialogItem_;
    const int entries_;
    std::vector<HWND> dialogHWNDs_;
    int childXOffset_, childYOffset_;
    int childWidth_, childHeight_;
    int currentDialog_;

public:
    enum { IDD = IDD_OPTIONFRAME};
    
    OptionFrameDialog(OptDialogItem* odi, int entries) : pDialogItem_(odi), entries_(entries), dialogHWNDs_(entries_) {}

    BEGIN_MSG_MAP(OptionFrameDialog)
        MSG_WM_INITDIALOG(OnInitDialog)
        COMMAND_ID_HANDLER(IDOK, OnOK)
        COMMAND_ID_HANDLER(IDCANCEL, OnCancel)
        NOTIFY_HANDLER(IDC_TREE, TVN_SELCHANGED, OnSelChanged)
//         COMMAND_HANDLER(IDC_GENERATE, BN_CLICKED, OnGenerate)
//         COMMAND_HANDLER(IDC_BROWSE, BN_CLICKED, OnBrowse)
        NOTIFY_HANDLER(IDC_TREE, TVN_ITEMEXPANDED, OnTvnItemexpandedTree)
    END_MSG_MAP()

    LRESULT OnInitDialog(HWND hwnd, LPARAM lParam);
    LRESULT OnOK(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
    LRESULT OnCancel(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);

    LRESULT OnSelChanged(int /*idCtrl*/, LPNMHDR pNMHDR, BOOL& /*bHandled*/);
public:
    LRESULT OnTvnItemexpandedTree(int /*idCtrl*/, LPNMHDR pNMHDR, BOOL& /*bHandled*/);
    void SubclassControls();
    BOOL PreTranslateMessage(MSG* pMsg);
};

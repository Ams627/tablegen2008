#pragma once
#include "resource.h"

class CMyEdit : public CWindowImpl<CMyEdit, CEdit>
{public:DECLARE_EMPTY_MSG_MAP()};

class CTablegenDlg: public CDialogImpl<CTablegenDlg>
{
    CMyEdit m_startEdit, m_endEdit;
    CString m_startFolder;
public:
	LRESULT OnGenerate(WORD, WORD, HWND, BOOL&);
    LRESULT OnBrowse(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	enum { IDD = IDD_TABLEGEN };

	BEGIN_MSG_MAP(CTablegenDlg)
		MSG_WM_INITDIALOG(OnInitDialog)
		COMMAND_ID_HANDLER(IDOK, OnOK)
		COMMAND_ID_HANDLER(IDCANCEL, OnCancel)
        COMMAND_HANDLER(IDC_GENERATE, BN_CLICKED, OnGenerate)
        COMMAND_HANDLER(IDC_BROWSE, BN_CLICKED, OnBrowse)
	END_MSG_MAP()

	LRESULT OnInitDialog(HWND hwnd, LPARAM lParam);
	LRESULT OnOK(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnCancel(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
};


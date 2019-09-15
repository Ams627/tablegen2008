#include "stdafx.h"
#include <ams/BrowseForFolder.h>
#include <ams/errorutils.h>
#include "tablegendlg.h"
#include "MonthCalGen.h"




LRESULT CTablegenDlg::OnInitDialog(HWND hwnd, LPARAM lParam)
{
    m_startEdit.SubclassWindow(GetDlgItem(IDC_EDITSTART));
    m_endEdit.SubclassWindow(GetDlgItem(IDC_EDITEND));
    return 0;
}

LRESULT CTablegenDlg::OnOK(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
    return 0;
}

LRESULT CTablegenDlg::OnCancel(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
    EndDialog(0);
    return 0;
}

LRESULT CTablegenDlg::OnGenerate(WORD, WORD, HWND, BOOL &)
{
    std::vector<char> startYearString(5), endYearString(5); 
    int startYear, endYear;

    int getres = m_startEdit.GetWindowText(&startYearString[0], startYearString.size());
    if (getres != 0)
    {
        startYear = atoi(&startYearString[0]);
    }
    else
    {
        startYear = 2006;
    }

    getres = m_endEdit.GetWindowText(&endYearString[0], endYearString.size());
    if (getres != 0)
    {
        endYear = atoi(&endYearString[0]);
    }
    else
    {
        endYear = 2006;
    }

    std::vector<char> currentDir(_MAX_PATH);
    RR GetCurrentDirectory(currentDir.size(), &currentDir[0]);

    for (int year = startYear; year <= endYear; year++)
    {
        std::ostringstream oss;
        oss << year;
        ams::CreateDirectoryRec(oss.str().c_str());

        RR SetCurrentDirectory(oss.str().c_str());

        for (int month = 0; month < 12; month++)
        {
            CMonthCalGen mcg(year, month + 1);
            mcg.Generate();
        }

	    CMonthCalGen mcg(year);
	    mcg.GenerateYear();

        RR SetCurrentDirectory(&currentDir[0]);
    }
    return 0;
}


LRESULT CTablegenDlg::OnBrowse(WORD, WORD, HWND, BOOL &)
{
    Ams::BrowseForFolder(*this, m_startFolder, m_startFolder.GetBufferSetLength(_MAX_PATH));
    
    return 0;
}

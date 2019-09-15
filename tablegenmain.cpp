#include "stdafx.h"
#include "tablegendlg.h"
#include "Directories.h"
#include "options.h"
#include "OptionFrameDlg.h"
#include <ams/wtl/DialogFactory.h>
//#define testing

CAppModule _Module;

#ifndef testing

Directories g_dirs("w-suite", "tablegen");

int WINAPI WinMain(HINSTANCE hInst,
                   HINSTANCE hInstPrev,
                   LPSTR szCmdLine,
                   int nCmdShow)
{

    try
    {
		SetProcessDPIAware();
        InitCommonControls(); // for windows XP styles

//        _Module.Init(NULL, hInst);

     
        std::string optionsFilename = g_dirs.GetAppData();
        optionsFilename += "options.dat";
        Options options(optionsFilename);
        options.ReadFromFile();

        OptDialogItem odi[] = {
            "General Options", IDD_GENERALOPTIONS,
            "Your Location", IDD_PLACEOPTIONS,
            "Birthdays and Reminders", IDD_BIRTHDAY,
            "Arse Options", IDD_ARSEOPTIONS
        };

        OptionFrameDialog optDlg(odi, sizeof odi / sizeof OptDialogItem);
        optDlg.DoModal();
        options.WriteToFile();

    //     CTablegenDlg dlg;
    //     dlg.DoModal();
    }
    catch(std::exception e)
    {
        std::stringstream ss;
        ss << e.what();
        ::MessageBox(NULL, ss.str().c_str(), "error", MB_OK);
    }
    catch (Reporter& e)
    {
        std::stringstream ss;
        e.DisplayError(ss);
        ::MessageBox(NULL, ss.str().c_str(), "error", MB_OK);
    }

    _Module.Term();
    return 0;
}
#endif


#ifdef testing
#include "ADate.h"
#include "sun.h"

int WINAPI WinMain(HINSTANCE hInst,
                   HINSTANCE hInstPrev,
                   LPSTR szCmdLine,
                   int nCmdShow)
{
    _Module.Init(NULL, hInst);

    CADate date(1, 6, 2005);
    double rise, set;
    SunRise(rise, set, 67.0 /*lat*/, 0 /*lon*/, date);

    _Module.Term();
    return 0;
}
#endif



// OUTPUTH(HDN_ITEMCHANGINGA);
// OUTPUTH(HDN_ITEMCHANGINGW);
// OUTPUTH(HDN_ITEMCHANGEDA);
// OUTPUTH(HDN_ITEMCHANGEDW);
// OUTPUTH(HDN_ITEMCLICKA);
// OUTPUTH(HDN_ITEMCLICKW);
// OUTPUTH(HDN_ITEMDBLCLICKA);
// OUTPUTH(HDN_ITEMDBLCLICKW);
// OUTPUTH(HDN_DIVIDERDBLCLICKA);
// OUTPUTH(HDN_DIVIDERDBLCLICKW);
// OUTPUTH(HDN_BEGINTRACKA);
// OUTPUTH(HDN_BEGINTRACKW);
// OUTPUTH(HDN_ENDTRACKA);
// OUTPUTH(HDN_ENDTRACKW);
// OUTPUTH(HDN_TRACKA);
// OUTPUTH(HDN_TRACKW);
// OUTPUTH(HDN_GETDISPINFOA);
// OUTPUTH(HDN_GETDISPINFOW);
// OUTPUTH(HDN_BEGINDRAG);
// OUTPUTH(HDN_ENDDRAG);
// OUTPUTH(HDN_FILTERCHANGE);
// OUTPUTH(HDN_FILTERBTNCLICK);
// OUTPUTH(HDN_ITEMCHANGING);
// OUTPUTH(HDN_ITEMCHANGED);
// OUTPUTH(HDN_ITEMCLICK);
// OUTPUTH(HDN_ITEMDBLCLICK);
// OUTPUTH(HDN_DIVIDERDBLCLICK);
// OUTPUTH(HDN_BEGINTRACK);
// OUTPUTH(HDN_ENDTRACK);
// OUTPUTH(HDN_TRACK);
// OUTPUTH(HDN_GETDISPINFO);
// OUTPUTH(HDN_ITEMCHANGING);
// OUTPUTH(HDN_ITEMCHANGED);
// OUTPUTH(HDN_ITEMCLICK);
// OUTPUTH(HDN_ITEMDBLCLICK);
// OUTPUTH(HDN_DIVIDERDBLCLICK);
// OUTPUTH(HDN_BEGINTRACK);
// OUTPUTH(HDN_ENDTRACK);
// OUTPUTH(HDN_TRACK);
// OUTPUTH(HDN_GETDISPINFO);
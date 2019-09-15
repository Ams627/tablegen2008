#include "stdafx.h"
#include "shlobj.h"
#include "Directories.h"

namespace {
    inline void AddSlash(std::string& fname)
    {
        if (*fname.rbegin() != '\\' && *fname.rbegin() != '/')
        {
            fname += '/';
        }
    }
}

Directories::Directories(std::string companyname, std::string progname)
{
    char folderName[_MAX_PATH];
    if SUCCEEDED(SHGetFolderPath(NULL, CSIDL_APPDATA, NULL, SHGFP_TYPE_CURRENT, folderName))
    {
        // Create the directory
        applicationData_ = std::string(folderName);
        AddSlash(applicationData_);
        applicationData_ += companyname;
        AddSlash(applicationData_);
        applicationData_ += progname;
        AddSlash(applicationData_);
        // Recursively Create the Directory: (if it already exists nothing will happen)
        ams::CreateDirectoryRec(applicationData_);
    }
}


Directories::~Directories(void)
{
}



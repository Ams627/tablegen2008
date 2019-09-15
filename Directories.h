#pragma once

class Directories
{
    std::string applicationData_;
public:
    std::string& GetAppData() {return applicationData_;}
    Directories(std::string companyname, std::string progname);
public:
    ~Directories(void);
};

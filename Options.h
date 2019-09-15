#pragma once
#include "resource.h"

class OptionBase
{
    const UINT idd_;
    const GUID windowType_;
    HWND hwnd_;
public:
    std::string name_;
    OptionBase(UINT idd, GUID windowType, char* name): idd_(idd), windowType_(windowType), name_(name){}
    inline void SetHWND(HWND hwnd) {hwnd_ = hwnd;}
    inline HWND GetHWND() {return hwnd_;}

    virtual bool GetBool() {throw std::exception("Invalid call to GetBool");}
    virtual int GetInt() {throw std::exception("Invalid call to GetInt");}

    virtual void Set(bool) {throw std::exception("Invalid call to Set");}
    virtual void Set(int) {throw std::exception("Invalid call to Set");}
    
    //    virtual int SetInt(int) {throw std::exception("Invalid call to SetInt");}

    virtual int GetMin() {throw std::exception("Invalid call to GetMin");}
    virtual int GetMax() {throw std::exception("Invalid call to GetMax");}
    virtual std::string GetString() {throw std::exception("Invalid call to GetString");}

    friend std::ostream& operator<<(std::ostream& str, const OptionBase& ob);

    virtual void Print(std::ostream& o) const {throw std::exception("Invalid call to Print");};

    UINT GetIDD() {return idd_;}
    GUID GetWindowType() {return windowType_;}
    virtual void Read(const std::string s) {}
protected:
};

inline std::ostream& operator<<(std::ostream& str, const OptionBase& ob)
{
    ob.Print(str);
    return str;
}

class BoolOption : public OptionBase {
    bool b_;
    const bool default_;
public:
    BoolOption(UINT idd, GUID guid, bool bDefault, char *name) : OptionBase(idd, guid, name), default_(bDefault), b_(bDefault){}
    void Set(bool b){b_ = b;}
    bool GetBool() {return b_;}
    virtual void Print(std::ostream& o) const {o << b_;}
    virtual void Read(const std::string s) {
        std::stringstream ss(s); ss >> b_;}
};

class IntOption : public OptionBase {
    int i_;
    const int default_;
    const int min_;
    const int max_;
public:
    IntOption(UINT idd, GUID guid, int idefault, int min, int max) :
      OptionBase(idd, guid, ""), default_(idefault), i_(idefault), min_(min), max_(max) {}
    int GetInt() {return i_;}
    int GetMin() {return min_;}
    int GetMax() {return max_;}
    virtual void Set(int i) {i_ = i;}
    virtual void Print(std::ostream& o) const {
        o << i_;
    }
    virtual void Read(const std::string s) {
        std::stringstream ss(s); ss >> i_;}
};

class StringOption: public OptionBase {
    std::string s_;
    std::string default_;
    const int maxlength_;
public:
    std::string GetString() {return s_;}
    void Print(std::ostream& o) const {o << s_;}
    virtual void Read(const std::string s) {
        s_ = s;}

};


typedef std::map<std::string, OptionBase*> OptMap;

class Options
{
    const std::string filename_;
public:
    Options(std::string filename);
    void WriteToFile();
    void ReadFromFile();
public:
    virtual ~Options(void);
};

extern OptMap g_optMap;
#pragma once
#include <fstream>
#include "adate.h"

class CMonthCalGen  
{
public:
	CMonthCalGen(int year, int month = 0);
    void PrintHeader();
	bool Generate();
	bool GenerateMonth(int month);
	bool GenerateMonthXML(int month);
	bool GenerateYear();
	CMonthCalGen();
	virtual ~CMonthCalGen();

private:
    void PrintTD(CADate date, std::string cl);
    void PrintHoursMins(std::ofstream& f, double event);
	int m_month;
    int m_year;
    std::ofstream m_htmlfile;	// main page
	std::ofstream m_xmlfile;	// xml calendar
	std::ofstream m_cssfile;	// style sheet
	std::ofstream m_jsfile;		// javascript stuff
};


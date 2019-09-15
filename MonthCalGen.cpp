#include "stdafx.h"
#include <math.h>
#include <iomanip>
#include <string>
#include <sstream>
#include "MonthCalGen.h"
#include "ADate.h"
#include "sun.h"
#include "moon.h"

CMonthCalGen::CMonthCalGen()
{

}

// Constructor - month = 0 means do whole year
CMonthCalGen::CMonthCalGen(int year, int month /* = 0 */)
{
	if (month != 0)
	{
		m_month = month;
		m_year = year;
		CADate date(1, month, year);
		std::string filename = date.GetShortMonthName();
		std::string htmlname = filename + ".html";
		std::string xmlname = filename + ".xml";
		m_htmlfile.open(htmlname.c_str());
		m_xmlfile.open(xmlname.c_str());
	}
	else // whole year
	{
		std::stringstream sshtml, ssxml;
		sshtml << "y" << year << ".html";
		ssxml << "y" << year << ".xml";
		m_htmlfile.open(sshtml.str().c_str());
		m_xmlfile.open(ssxml.str().c_str());
		m_year = year;
		m_month = 0;
	}
}

CMonthCalGen::~CMonthCalGen()
{

}

using namespace std;


bool CMonthCalGen::Generate()
{
	PrintHeader();

	m_htmlfile << "<body>" << endl;
	for (int m = 0; m < 12; m++)
	{
		CADate tdate(1, m + 1, m_year);
		m_htmlfile << "<a href='" << tdate.GetShortMonthName() << ".html'>" << tdate.GetShortMonthName() << "</a>" << endl;
	}
	GenerateMonth(m_month);
	GenerateMonthXML(m_month);
	m_htmlfile.close();
	m_xmlfile.close();
	return true;
}

bool CMonthCalGen::GenerateMonth(int month)
{
	m_htmlfile << "<table class='monthtable'>" << endl;

	m_htmlfile << "<tr class='monthname'><th colspan=7>" << endl;
	CADate date(1, month, m_year);
	m_htmlfile << date.GetShortMonthName() << " " << date.GetYear() << endl;
	m_htmlfile << "</th></tr>" << endl;

	m_htmlfile << "<tr>" << endl;
	m_htmlfile << "<td class=dayheader>Mon</td>" << endl;
	m_htmlfile << "<td class=dayheader>Tue</td>" << endl;
	m_htmlfile << "<td class=dayheader>Wed</td>" << endl;
	m_htmlfile << "<td class=dayheader>Thu</td>" << endl;
	m_htmlfile << "<td class=dayheader>Fri</td>" << endl;
	m_htmlfile << "<td class=dayheader>Sat</td>" << endl;
	m_htmlfile << "<td class=dayheader>Sun</td>" << endl;
	m_htmlfile << "</tr>" << endl;

	// "round" date to previous monday:
	date.SetPrevious(0);

	for (int row = 0; row < 6 && ((date.GetMonth() <= month) ||
                                  (month == 1 && date.GetMonth() == 12)) ||
								  (month == 12 && date.GetMonth() != 1);
								   row++)
	{
		m_htmlfile << "<tr>";

		for (int col = 0; col < 7; col++)
		{
			if (date.GetMonth() == month) // don't print days from previous or next month
			{
				PrintTD(date, "dayno");
				//				m_htmlfile << "<table id='k" << date.GetDay() << "'><tr>";
				m_htmlfile << "<table><tr>";
				PrintTD(date, "inner");

				m_htmlfile << date.GetDay();
				m_htmlfile << "</td>" << endl;

				// Get Sunrise and sunset times:
				double rise, set, moonrise, moonset;
                SunRise(rise, set, 50.7127, 1.9885, date);
                MoonRise(moonrise, moonset, 50.7127, 1.9885, date);

				if (date.IsInEUDST())
				{
					rise += 1.0 / 24.0;
					set += 1.0 / 24.0;
					moonrise += 1.0 / 24.0;
					moonset += 1.0 / 24.0;
					if (rise > 1.0)
					{
						rise -= 1.0;
					}
					if (set > 1.0)
					{
						set -= 1.0;
					}
					if (moonrise > 1.0)
					{
						moonrise -= 1.0;
					}
					if (moonset > 1.0)
					{
						moonset -= 1.0;
					}
				}


				//PrintTD(date, "srss");
				m_htmlfile << "</tr>\n<tr><td class='srss'>\n";
				m_htmlfile << "sr/ss<br>day<br>mr/ms<br></td>";
				PrintTD(date, "srss2");
				PrintHoursMins(m_htmlfile, rise);
				m_htmlfile <<  "/";
				PrintHoursMins(m_htmlfile, set);
				m_htmlfile << "<br>";
				PrintHoursMins(m_htmlfile, set - rise);
				m_htmlfile << "<br>";
				PrintHoursMins(m_htmlfile, moonrise);
				m_htmlfile << "/";
				PrintHoursMins(m_htmlfile, moonset);
				m_htmlfile << "</td>";
				m_htmlfile << "</tr></table></td>" << endl;
			}
			else
			{
				m_htmlfile << "<td style='background-color:#FFFFFF'></td>" << endl;
			}
			date++;
		}
		m_htmlfile << "</tr>" << endl;
	}

	m_htmlfile << "</table>" << endl;
	return true;
}

bool CMonthCalGen::GenerateMonthXML(int month)
{
	m_xmlfile << "<month>\n<label>\n";
	CADate date(1, month, m_year);
	m_xmlfile << date.GetShortMonthName() << " " << date.GetYear() << endl;
	m_xmlfile << "</label>\n";
	m_xmlfile << "<daynames>\n";
	m_xmlfile << "<dayname>Mon</dayname>\n";
	m_xmlfile << "<dayname>Tue</dayname>\n";
	m_xmlfile << "<dayname>Wed</dayname>\n";
	m_xmlfile << "<dayname>Thu</dayname>\n";
	m_xmlfile << "<dayname>Fri</dayname>\n";
	m_xmlfile << "<dayname>Sat</dayname>\n";
	m_xmlfile << "<dayname>Sun</dayname>\n";
	m_xmlfile << "</daynames>\n";

	date.SetPrevious(0);

	for (int row = 0; row < 6 && ((date.GetMonth() <= month) ||(month == 1 && date.GetMonth() == 12)) ;
		row++)
	{
		for (int col = 0; col < 7; col++)
		{
			if (date.GetMonth() == month) // don't print days from previous or next month
			{
				m_xmlfile << "<day>";
				m_xmlfile << "<dayno>";
				m_xmlfile << date.GetDay();
				m_xmlfile << "</dayno>";
				// Get Sunrise and sunset times:
				double rise, set, moonrise, moonset;
                SunRise(rise, set, 50.7127, -1.9885, date);
                MoonRise(moonrise, moonset, 50.723, -1.9885, date);

				if (date.IsInEUDST())
				{
					rise += 1.0 / 24.0;
					set += 1.0 / 24.0;
					moonrise += 1.0 / 24.0;
					moonset += 1.0 / 24.0;
					if (rise > 1.0)
					{
						rise -= 1.0;
					}
					if (set > 1.0)
					{
						set -= 1.0;
					}
					if (moonrise > 1.0)
					{
						moonrise -= 1.0;
					}
					if (moonset > 1.0)
					{
						moonset -= 1.0;
					}
				}

				m_xmlfile << "<stats>";
				m_xmlfile << "<sr>";
				PrintHoursMins(m_xmlfile, rise);
				m_xmlfile << "</sr>";

				m_xmlfile << "<ss>";
				PrintHoursMins(m_xmlfile, set);
				m_xmlfile << "</ss>";

				m_xmlfile << "<day>";
				PrintHoursMins(m_xmlfile, set - rise);
				m_xmlfile << "</day>";

				m_xmlfile << "<mr>";
				PrintHoursMins(m_xmlfile, moonrise);
				m_xmlfile << "</mr>";

				m_xmlfile << "<ms>";
				PrintHoursMins(m_xmlfile, moonset);
				m_xmlfile << "</ms>";
				m_xmlfile << "</stats></day>\n";
			}
			else
			{
				m_xmlfile << "<day></day>\n";
			}
			date++;
		}
	}
	m_xmlfile << "</month>";
	return true;
}

bool CMonthCalGen::GenerateYear()
{
	PrintHeader();
	m_htmlfile << "<body>\n";
	m_htmlfile << "<table>\n";
	for (int i = 0; i < 12; i++)
	{
		if ((i % 3) == 0)
		{
			m_htmlfile << "<tr>\n";
		}
		CADate da(1, i + 1, m_year);
		m_htmlfile << "<!--" << da.GetShortMonthName() << "-->\n";
		m_htmlfile << "<td>\n";
		GenerateMonth(i + 1);
	}
	m_htmlfile << "</table>\n";
	m_htmlfile.close();
	return true;
}


void CMonthCalGen::PrintHeader()
{
	m_cssfile.open("w-cal.css");
	m_cssfile << ".inner {border:none;padding:0px;margin:0px;}" << endl;
	m_cssfile << "" << endl;
	m_cssfile << "table {" << endl;
	m_cssfile << "font-family:arial, helvetica;" << endl;
	m_cssfile << "font-size:14pt;" << endl;
	m_cssfile << "}" << endl;
	m_cssfile << "" << endl;

	m_cssfile << "td {" << endl;
	m_cssfile << "font-size:20;" << endl;
	m_cssfile << "width: 100px;" << endl;
	m_cssfile << "padding-left: 1px;" << endl;
	m_cssfile << "padding-bottom: 1px;" << endl;
	m_cssfile << "padding-top:1px;" << endl;
	m_cssfile << "padding-right: 1px;" << endl;
	m_cssfile << "vertical-align:top;" << endl;
	m_cssfile << "background-color:#ffffff;" << endl;
	m_cssfile << "}" << endl;
	m_cssfile << "" << endl;

	m_cssfile << ".summer {background-color: #F7F67F;}" << endl;
	m_cssfile << "th {" << endl;
	m_cssfile << "font-size: 20;" << endl;
	m_cssfile << "font-family:arial, helvetica;" << endl;
	m_cssfile << "}" << endl;
	m_cssfile << "" << endl;

	m_cssfile << ".dayheader {" << endl;
	m_cssfile << "font-size:20;" << endl;
	m_cssfile << "padding-bottom: 1px;" << endl;
	m_cssfile << "background-color: #ffffff;" << endl;
	m_cssfile << "text-align: centre;" << endl;
	m_cssfile << "}" << endl;
	m_cssfile << "" << endl;

	m_cssfile << ".monthtable {" << endl;
	m_cssfile << "background-color: pink;" << endl;
	m_cssfile << "border-spacing: 1px;" << endl;
	m_cssfile << "}" << endl;
	m_cssfile << endl;

	m_cssfile << ".monthname {" << endl;
	m_cssfile << "background-color:#ffffff;" << endl;
	m_cssfile << "}" << endl;

	m_cssfile << ".dayno {" << endl;
	m_cssfile << "}" << endl;
	m_cssfile << endl;

	m_cssfile << ".srss {" << endl;
	m_cssfile << "font-size: 10pt;" << endl;
	m_cssfile << "padding-right: 10pt;" << endl;
	m_cssfile << "text-align:left;" << endl;
	m_cssfile << "font-family: times;" << endl;
	m_cssfile << "border:none;" << endl;
	m_cssfile << "}" << endl;

	m_cssfile << ".srss2 {" << endl;
	m_cssfile << "font-size: 10pt;" << endl;
	m_cssfile << "padding-right: 0pt;" << endl;
	m_cssfile << "text-align:left;" << endl;
	m_cssfile << "font-family: times;" << endl;
	m_cssfile << "border:none;" << endl;
	m_cssfile << "}" << endl;

	m_cssfile << "" << endl;
	m_cssfile << ".invisible {" << endl;
	m_cssfile << "position:absolute;" << endl;
	m_cssfile << "visibility:hidden;" << endl;
	m_cssfile << "font-size:10pt;" << endl;
	m_cssfile << "background:#EAF5AF;" << endl;
	m_cssfile << "}" << endl;
	m_cssfile << ".floattable {" << endl;
	m_cssfile << "border:solid 1px black;" << endl;
	m_cssfile << "font-size:10pt;" << endl;
	m_cssfile << "background:#AF5AF0;" << endl;
	m_cssfile << "padding-left:2px;" << endl;
	m_cssfile << "padding-right:2px;" << endl;
	m_cssfile << "padding-top:2px;" << endl;
	m_cssfile << "padding-bottom:2px;" << endl;
	m_cssfile << "width:0px;" << endl;
	m_cssfile << "}" << endl;
	m_cssfile << "" << endl;
	m_cssfile << ".ftd {" << endl;
	m_cssfile << "   border:solid 1px black;" << endl;
	m_cssfile << "}" << endl;
	m_cssfile.close();

	m_htmlfile << "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.01 Transitional//EN\"\n";
    m_htmlfile << "\"http://www.w3.org/TR/html4/loose.dtd\">\n";

	m_htmlfile << "<html>\n";
    m_htmlfile << "<head><title>Generated by W-Calendar</title>\n";
	m_htmlfile << "<LINK REL=StyleSheet HREF=\"w-cal.css\" TYPE=\"text/css\" " << endl;
    m_htmlfile << "<script type=\"text/javascript\" src=\"w-cal.js\"> </script>" << endl;
    m_htmlfile << "</head>" << endl;
}


void CMonthCalGen::PrintHoursMins(std::ofstream& f, double event)
{
    // event represents a fraction of a day and is thus
    // in the range 0 - 1;ax

    int secs = int(floor(86400.0 * event + 0.5));
    int mins = (secs + 30) / 60;
    int hours = mins / 60;
    mins %= 60;

    f << setfill('0') << setw(2) << hours << ":" << setw(2) << mins;
}


// print an opening td tag with class=summer if we are in summer time
// also add the name of another specified class cl
void CMonthCalGen::PrintTD(CADate date, std::string cl)
{
    string classname = cl;
    if (date.IsInEUDST())
    {
        if (!classname.empty())
        {
            classname += " ";
        }
        classname = classname + "summer";
    }

    m_htmlfile << "<td";
    if (classname.empty())
    {
        m_htmlfile << ">" << endl;
    }
    else
    {
        m_htmlfile << " class=\"" << classname << "\">" << endl;
    }
}

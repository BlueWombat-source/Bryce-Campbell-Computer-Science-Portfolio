#pragma once
#ifndef SALESANALYZER_H
#define SALESANALYZER_H
#include <string>
#include <map>

class SalesAnalyzer {
public:
	void DisplayHeading(int t_width = 30, char t_fillChar = '*', std::string t_text = "");
	void DisplayMenuOption(int optionNum);
	void DisplayMenu();
	void ReadSalesLog();
	void WriteToOutputFile();
	void SearchForItem();
	void DisplayFrequency();
	void DisplayHistogram();

private:
	std::map<std::string, int> m_salesCount;
};

#endif


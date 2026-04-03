#include "SalesAnalyzer.h"
#include <iostream>
#include <iomanip>
#include <stdlib.h>
#include <fstream>
#include <string>
#include <map>
#include "conio.h"
#include <Windows.h>

// Displays a line of text centered between fill characters
void SalesAnalyzer::DisplayHeading(int t_width, char t_fillChar, std::string t_text) {
	int padding = (t_width - t_text.size()) / 2;

	std::cout << std::setfill(t_fillChar) << std::setw(padding + t_text.size()) << std::right 
		<< t_text << std::setw(t_width - padding - t_text.size()) << t_fillChar << std::endl;
}

// Displays menu options; Allows for easy insertion of additional menu options
void SalesAnalyzer::DisplayMenuOption(int optionNum) {
	switch (optionNum) {
	case 1:
		std::cout << "[1] Search By Item" << std::endl;
		break;

	case 2:
		std::cout << "[2] Show Sales Per Item" << std::endl;
		break;

	case 3:
		std::cout << "[3] Show Histogram" << std::endl;
		break;

	case 4:
		std::cout << "[4] Exit" << std::endl;
		break;
	}
}

// Displays main menu
void SalesAnalyzer::DisplayMenu() {
	system("cls");
	DisplayHeading(50, '=', " Corner Grocer Sales Analyzer ");
	DisplayHeading(50, '+');
	DisplayHeading(50, '-', " Please Select A Menu Option ");
	std::cout << std::endl;

	for (int i = 1; i <= 4; ++i) {
		DisplayMenuOption(i);
	}

	int menuSelection;
	std::cin >> menuSelection;
	switch (menuSelection) {
	case 1:
		SearchForItem();
		break;
	
	case 2:
		DisplayFrequency();
		break;

	case 3:
		DisplayHistogram();
		break;

	case 4:
		system("cls");
		DisplayHeading(50, '*', " Program Exited ");
		break;
	}
}

// Reads input file and stores data into map container
void SalesAnalyzer::ReadSalesLog() {
	std::ifstream salesLog;
	std::string temp;

	system("cls");
	salesLog.open("CS210_Project_Three_Input_File.txt");
	if (!salesLog.is_open()) {
		std::cout << "Failed to open input file" << std::endl;
	}
	else {
		salesLog >> temp;
		while (!salesLog.fail()) {
			if (m_salesCount.count(temp) == 1) {
				m_salesCount.at(temp) += 1;
			}
			else {
				m_salesCount.emplace(temp, 1);
			}
			salesLog >> temp;
		}
		if (salesLog.eof()) {
			std::cout << "End of file reached" << std::endl;
		}
		std::cout << "Closing file" << std::endl;
		salesLog.close();
		WriteToOutputFile();
	}
}

// Writes item name and number of times sold to frequency.dat
void SalesAnalyzer::WriteToOutputFile() {
	std::ofstream salesData;

	salesData.open("frequency.dat");

	if (salesData.is_open()) {
		for (auto pair : m_salesCount) {
			salesData << pair.first << " " << pair.second << std::endl;
		}
	}
	else {
		std::cout << "Output File Failed To Open" << std::endl;
	}
	std::cout << "Closing Output File" << std::endl;
	salesData.close();
}

/* Gets user input and searches for specified item in map container
Displays total sales of item */
void SalesAnalyzer::SearchForItem() {
	std::string searchItem;
	std::string headingString = " Total Sales For ";

	system("cls");
	DisplayHeading(50, '+', " Please Enter An Item To Search For ");
	std::cin >> searchItem;

	// Matches letter case to keys in map, making the search not case sensitive
	for (int i = 0; i < searchItem.size(); ++i) {
		searchItem[i] = std::tolower(searchItem[i]);
	}
	searchItem[0] = toupper(searchItem[0]);

	// Validates user input, ensuring the item entered exists in the file
	while (m_salesCount.count(searchItem) == 0) {
		system("cls");
		DisplayHeading(50, '*', " Item Not Found ");
		DisplayHeading(50, '+', " Please Enter An Item To Search For ");
		std::cin >> searchItem;

		// Matches letter case to keys in map, making the search not case sensitive
		for (int i = 0; i < searchItem.size(); ++i) {
			searchItem[i] = std::tolower(searchItem[i]);
		}
		searchItem[0] = toupper(searchItem[0]);
	}
	system("cls");
	DisplayHeading(50, '=', headingString.append(searchItem) + " ");
	std::cout << searchItem << ": " << m_salesCount.at(searchItem) << std::endl;
	std::cout << "Press Any Key To Continue..." << std::endl;
	// Gets character from input and ignores it, sending user to menu upon entering any key
	_getch();
	DisplayMenu();
}

// Displays every item and the amount of times it was sold
// Green = Sold 5 or more times, Yellow = Sold between 5 and 3 times, Red = Sold less than 3 times
void SalesAnalyzer::DisplayFrequency() {
	HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);

	system("cls");
	DisplayHeading(50, '+', " Total Sales Per Item ");
	DisplayHeading(50, '+');
	for (auto pair : m_salesCount) {
		std::cout << std::setw(15) << std::setfill(' ')
			<< pair.first << ": ";
		// Changes text color depending on number of times the item has been sold
		if (pair.second >= 5) {
			SetConsoleTextAttribute(h, FOREGROUND_GREEN);
		}
		else if (pair.second >= 3) {
			SetConsoleTextAttribute(h, FOREGROUND_RED | FOREGROUND_GREEN);
		}
		else {
			SetConsoleTextAttribute(h, FOREGROUND_RED);
		}
		std::cout << pair.second << std::endl;
		SetConsoleTextAttribute(h, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
	}

	std::cout << "Press Any Key To Continue..." << std::endl;
	// Gets character from input and ignores it, sending user to menu upon entering any key
	_getch();
	DisplayMenu();
}

// Displays histogram of items and frequency of sale
// Green = Sold 5 or more times, Yellow = Sold between 5 and 3 times, Red = Sold less than 3 times
void SalesAnalyzer::DisplayHistogram() {
	HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);

	system("cls");
	DisplayHeading(50, '+', " Total Sales Per Item ");
	DisplayHeading(50, '+');
	for (auto pair : m_salesCount) {
		std::cout << std::setw(15) << std::setfill(' ') << pair.first << ": ";
		for (int i = 0; i < pair.second; ++i) {
			// Changes text color depending on number of times the item has been sold
			if (pair.second >= 5) {
				SetConsoleTextAttribute(h, FOREGROUND_GREEN);
			}
			else if (pair.second >= 3) {
				SetConsoleTextAttribute(h, FOREGROUND_RED | FOREGROUND_GREEN);
			}
			else {
				SetConsoleTextAttribute(h, FOREGROUND_RED);
			}
			std::cout << '*';
		}
		SetConsoleTextAttribute(h, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
		std::cout << std::endl;
	}
	std::cout << "Press Any Key To Continue..." << std::endl;
	// Gets character from input and ignores it, sending user to menu upon entering any key
	_getch();
	DisplayMenu();
}
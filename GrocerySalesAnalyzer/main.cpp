// Bryce Campbell
// A program to analzye saLes records

#include <iostream>
#include "SalesAnalyzer.h"

int main() {

	SalesAnalyzer analyzer;
	analyzer.ReadSalesLog();
	analyzer.DisplayMenu();

	return 0;
}
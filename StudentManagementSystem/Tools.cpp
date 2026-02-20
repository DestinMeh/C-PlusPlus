#include "Tools.h"
#include<cstdio>
#include<iostream>

int Tools::validateNum(int size) {
	int num;
	bool pass = false;

	do {
		if (!(std::cin >> num)) {
			std::cin.clear();
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			std::cout << "Please input a valid number\n";
			continue;
		}

		if (num < 1 || num > size) {
			std::cout << "Please input a number between 1 and " << size << "\n";
			continue;
		}

		pass = true;
	} while (pass == true);

	return num;
}

void Tools::menu() {
	std::cout << "Main Menu\n"
		<< "1. Students\n"
		<< "2. Subjects\n"
		<< "3. Exit Program\n"
		<< "Choose a number: ";
}

void Tools::clearScreen() {
	std::cout << "\033[2J\033[1;1H";
}


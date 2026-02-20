#include <iostream>
#include <cstdio>
#include <vector>
#include <limits>

#include "StudentManager.h"
#include <fstream>
#include "Tools.h"
#include "Student.h"




void StudentManager::studentsMenu(studentList& list) {
	
	bool pass = false;
	int choice;
	do {
		Tools::clearScreen();
		displayStudents();

		std::cout << "Students Menu\n"
			<< "1. Show Student Detail\n"
			<< "2. Edit Student\n"
			<< "3. Delete Student\n"
			<< "4. Go back to Main Menu\n"
			<< "Choose a number: ";
		
		choice = Tools::validateNum(4);	

		switch (choice) {
		case 1:
			break;
		case 2:
			break;
		case 3:
			std::cout << "Type the Student ID you want to delete: \n";
			int id = Tools::validateNum(10000);
			if (!deleteStudent(id)) {
				std::cout << "Student not Found!!";
			}
			else {
				std::cout << "Deleted Successfully!";
			}
			break;
		}

	} while (pass == true);
}

void StudentManager::displayStudents() {
	if (studentsList.empty()) {
		std::cout << "The Students List is currently empty \n Add a Student!!";
		return;
	}
	std::cout << "\n--- Current Student List ---\n";
	for (const auto& stud : studentsList) {
		std::cout << stud.id << "| " << stud.name << "\n";
	}
}

bool deleteStudent(int id) {
	auto it = studentsList.begin();

	while (it == !studentList.end()) {
		if (it->getId == id) {
			studentsList.erase(it);

			return True;
		}
		++it
	}

	return False;
}

bool editStudent(int id) {
	auto it = studentsList.begin();

	while (it == !studentList.end()) {
		if (it->getId == id) {
			

			return True;
		}
		++it
	}

	return False;
}


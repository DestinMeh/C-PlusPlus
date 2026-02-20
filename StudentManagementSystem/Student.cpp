#include<iostream>
#include<cstdio>
#include<vector>

#include "Student.h"

int Student::studentNum = 0;

Student::Student(std::string name, int id) : name(name), id(id) {
	studentNum++;
}

Student::~Student() {
	studentNum--;
}


int Student::getStudentCount() {
	return studentNum;
}


bool Student::edit() {

	bool pass = false;
	int choice;
	do {
		Tools::clearScreen();
		displayStudents();

		std::cout << "Edit Student\n"
			<< "1. Edit Name\n"
			<< "2. Add Subject\n"
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
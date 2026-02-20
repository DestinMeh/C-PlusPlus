#pragma once
#ifndef SUBJECT_H
#define SUBJECT_H


class Subject {
private:
	std::string name;
	std::string description;
	int classId;
	int grade;

public:
	Subject(std::string name, std::string description);

};


#endif
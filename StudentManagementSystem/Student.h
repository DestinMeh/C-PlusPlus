#pragma once
#ifndef STUDENTMANAGER_H
#define STUDENT

#include<iostream>
#include<cstdio>
#include<vector>

#include"Subject.h"
#include"SubjectManager.h"



class Student {
private:
	std::string name;
	int id;
	std::vector<Subject> subjectList;
	static int studentNum;


public:
	Student(std::string name, int id);
	virtual ~Student();
	static int getStudentCount();
	bool edit();
	
	int getId() const { return id; }
	std::string getName() const { return name; }
};

#endif

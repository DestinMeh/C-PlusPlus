#pragma once
#ifndef STUDENTMANAGER_H
#define STUDENTMANAGER_H

#include<vector>
#include"Student.h"

class StudentManager {
private:
    
    std::vector<Student> studentsList;
    std::string fileName;

public:
    
    StudentManager();
    void addStudent(const Student& s);
    void saveToFile();
    void studentsMenu()
    bool deleteStudent(int id);
    bool editStudent(int id);
    void StudentManager::displayStudents(); 

    const std::vector<Student>& getList() const { return studentsList; }
    std::string getFileName() const { return fileName; }
};

#endif
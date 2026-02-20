#include<cstdio>
#include<string>

class Subject {
private:
	std::string name;
	std::string description;
	int classId;
	int grade;

public:

	Subject(std::string name, std::string description, int classId) :
		name(name),
		description(description),
		classId(classId) {
	}
};
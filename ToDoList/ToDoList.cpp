#include <cstdio>
#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <limits>

struct Task {
    std::string description;
    bool isDone = false;
};

using List = std::vector<Task>;


void isEmpty() {
    std::cout << "The List is Empty. Add an Item\n";
}

void displayList(const List& list) {
    if (list.empty()) return isEmpty();

    int index = 1;
    std::cout << "\n\n Your List \n";
    for (const auto& item : list) {
        std::cout << index << " "
            << (item.isDone ? "[x]" : "[o]") << " "
            << item.description << "\n";

        index++;
    }

    std::cout << "\n\n";
}

void initializeFile(List& list) {
    std::ifstream inFile("output.txt");

    if (inFile.is_open()) {
        std::string line;
        int isDone;
        std::string description;
        while (inFile >> isDone) {
            inFile.ignore();

            std::getline(inFile, description);

            Task item;
            item.description = description;
            item.isDone = (isDone == 1);
            list.push_back(item);
        }
    }
    else {
        std::cout << "There is no saved file. Create a fresh list!";
        return;
    }

    inFile.close();
    std::cout << "File is succesfully loaded\n";
}

void writeFile(const List& list) {
    std::ofstream outFile("output.txt");
    if (outFile.is_open()) {
        for (const auto& item : list) {
            outFile << (item.isDone ? 1 : 0) << " " << item.description << "\n";
        }
        outFile << std::endl;
        outFile.close();
    }
}

void addItem(List& list) {
    std::cout << "Add an Item - write the description: \n";
    std::string item;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::getline(std::cin, item);
    list.push_back(Task{ item });
}

int validateNum(int size) {
    int num;
    bool success = false;
    do {
        if (!(std::cin >> num)) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Please enter a valid number: ";
            continue;
        }
        if (num < 1 || num > size) {
            std::cout << "Please enter a number between 1 and " << size << ": ";
            continue;
        }
        success = true;
    } while (!success);

    return num;
}

void removeItem(List& list) {
    if (list.empty()) return isEmpty();

    displayList(list);
    std::cout << "Input the number of the item you want to remove: \n";
    bool isTrue = false;
    do {
        int num;
        num = validateNum(list.size());
        int index = num - 1;

        list.erase(list.begin() + index);
        std::cout << "The item is succesfully removed!\n";
        isTrue = true;
    } while (!isTrue);
}

void displayMenu() {
    std::cout << "1. Display List\n"
        << "2. Add Item\n"
        << "3. Remove Item\n"
        << "4. Edit Item\n"
        << "5. Exit\n"
        << "Input choice: ";
}

void markItem(List& list) {
    if (list.empty()) return isEmpty();

    displayList(list);
    std::cout << "Input the number of the item you want to change mark: \n";
    bool isTrue = false;
    do {
        int num;
        num = validateNum(list.size());
        int index = num - 1;

        list[index].isDone = (list[index].isDone ? false : true);

        std::cout << "The item is succesfully changed mark!\n";
        isTrue = true;
    } while (!isTrue);
}



int main() {
    List taskList;
    initializeFile(taskList);
    std::cout << "Welcome!! This is a To do list Application\n";
    bool on = false;
    do {
        displayMenu();
        int choice;
        choice = validateNum(5);

        switch (choice) {
        case 1:
            displayList(taskList);
            break;
        case 2:
            addItem(taskList);
            break;
        case 3:
            removeItem(taskList);
            break;
        case 4:
            markItem(taskList);
            break;
        case 5:
            on = true;
            writeFile(taskList);
        }
    } while (!on);

    return 0;
}


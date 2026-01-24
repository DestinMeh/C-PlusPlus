#include<cstdio>
#include<cstdlib>
#include<random>
#include<vector>
#include<iostream>
#include<limits>
#include<ctime>

bool check_guess(int user_guess, int number_to_guess) {
    if (user_guess < number_to_guess) {
        std::cout << "Guess higher!\n";
        return false;
    }
    else if (user_guess > number_to_guess) {
        std::cout << "Too high! Guess Lower! lmao\n";
        return false;
    }
    else {
        std::cout << "Congratulations, this game is done!\n";
        return true;
    }

}

int main() {
    std::random_device rd;
    std::mt19937 gen(rd() ^ static_cast<unsigned int>(std::time(nullptr)));
    std::uniform_int_distribution<> distrib(1, 100);

    int number_to_guess = distrib(gen);
    std::vector<int> guesses = {};

    bool guessed = false;
    std::cout << "Welcome to the Random Number Guesser!\n" <<
        "The game is about guessing a number between 1 and 100. Can you guess it?\n" <<
        "Let's begin!\n\n" << std::endl;

    do {
        int user_guess;
        std::cout << "Enter your guess: ";

        if (!(std::cin >> user_guess)) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Invalid input. Please enter a number betwee 1 and 100." << std::endl;
            continue;
        }
        guesses.push_back(user_guess);

        guessed = check_guess(user_guess, number_to_guess);
    } while (guessed == false);

    std::cout << "You took " << guesses.size() << " attempts to guess the number.\n";
    for (int g : guesses) {
        std::cout << g << " ";
    }

    std::cout << "Do you want to play again? (y/n): ";
    char play_again;
    std::cin >> play_again;
    if (play_again == 'y' || play_again == 'Y') {
        main();
    }
    else {
        std::cout << "Thank you for playing! Goodbye!\n";
    }

    return 0;
}


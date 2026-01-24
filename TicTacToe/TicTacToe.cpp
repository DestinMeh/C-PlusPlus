#include <cstdio>
#include <iostream>
#include <array>
#include <random>
#include <ctime>

using Board = std::array<std::array<char, 3>, 3>;


char random_starter() {
    std::random_device rd;
    std::mt19937 gen(rd() ^ static_cast<unsigned int>(std::time(nullptr)));
    std::bernoulli_distribution coinflip(0.5);

    return coinflip(gen) ? 'x' : 'o';
}

void display_game(const Board* game) {
    for (const auto& row : *game) {
        for (char val : row) {
            std::cout << "(" << val << ") ";
        }
        std::cout << "\n";
    }
}

void input_choice(Board* game, char user) {
    bool checker = false;
    int r, c;
    std::cout << "Input your choice: (ex: 1 1 for top left): ";
    do {

        if (!(std::cin >> r >> c)) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Invalid input! Please type a number: ";
            continue;
        }


        if (r < 1 || r > 3 || c < 1 || c > 3) {
            std::cout << "Out of bounds! Choose 1, 2, or 3.: ";
            continue;
        }

        r--;
        c--;

        if ((*game)[r][c] == 'x' || (*game)[r][c] == 'o') {
            std::cout << "This spot is already filled, select an empty spot: ";
        }
        else {
            (*game)[r][c] = user;
            checker = true;
        }

    } while (checker == false);
}

bool check_win_state(const Board* game, char user) {
    if (game == nullptr) return false;

    for (int i = 0; i < 3; i++) {
        if ((*game)[i][0] == user && (*game)[i][1] == user && (*game)[i][2] == user) return true;
        if ((*game)[0][i] == user && (*game)[1][i] == user && (*game)[2][i] == user) return true;
    }

    if ((*game)[0][0] == user && (*game)[1][1] == user && (*game)[2][2] == user) return true;
    if ((*game)[0][2] == user && (*game)[1][1] == user && (*game)[2][0] == user) return true;
    return false;
}

int main() {
    Board gameboard = { {
        {' ',' ',' '},
        {' ',' ',' '},
        {' ',' ',' '}
    } };
    Board* gamePtr = &gameboard;
    int countMoves = 0;

    std::cout << "Hello to the Tic Tac Toe Game\n" << "The rules of the game is to get 3 in a row\n"
        << "To input your choice, write down the row first and the column (ex. 1 2 for top middle)\n\n"
        << "Let's start the game" << std::endl;


    bool finished = false;
    char player = random_starter();


    do {
        std::cout << "Its player " << player << "'s turn\n";
        display_game(gamePtr);
        input_choice(gamePtr, player);
        finished = check_win_state(gamePtr, player);

        if (finished == true) {
            display_game(gamePtr);
            std::cout << "Congratulationss!!, Player " << player << " won the game";
            break;
        }

        if (player == 'x') player = 'o';
        else player = 'x';

        countMoves++;

        if (countMoves == 9) {
            display_game(gamePtr);
            std::cout << "Its a Draw!!";
            break;
        }

    } while (finished == false);


}


#include <iostream>
#include <vector>
#include <string>
#include "chess/board.hpp" // Placeholder for chess library
#include "movegeneration.hpp" // Placeholder for move generation
#include "evaluate.hpp" // Placeholder for evaluation

std::string render(const chess::Board& board) {
    std::vector<std::string> asciiPieces = {
        "R", "N", "B", "Q", "K", "P",
        "r", "n", "b", "q", "k", "p",
        "."
    };

    std::string boardString = board.toString(); // Assuming a method to get the board as a string
    std::vector<std::string> lines;
    std::string line;

    std::istringstream iss(boardString);
    while (std::getline(iss, line)) {
        std::string translatedLine;
        for (char ch : line) {
            auto it = std::find(asciiPieces.begin(), asciiPieces.end(), std::string(1, ch));
            if (it != asciiPieces.end()) {
                translatedLine += *it;
            } else {
                translatedLine += ch;
            }
        }
        lines.push_back(translatedLine);
    }

    std::vector<std::string> ranks = {"8", "7", "6", "5", "4", "3", "2", "1"};

    if (board.getTurn() == chess::BLACK) {
        std::reverse(lines.begin(), lines.end());
        std::reverse(ranks.begin(), ranks.end());
    }

    std::string display;
    for (size_t i = 0; i < ranks.size(); ++i) {
        display += "  " + ranks[i] + " " + lines[i] + "\n";
    }
    display += "    a b c d e f g h";

    return display;
}

void aiVsAi() {
    chess::Board board;
    int depth = 3; // Default depth

    while (!board.isGameOver()) {
        std::cout << render(board) << std::endl;
        chess::Move move = nextMove(depth, board, false);
        board.push(move);

        std::cout << "Move played: " << move.toString() << std::endl;
    }

    std::cout << render(board) << std::endl;

    std::string result = board.result();
    std::string winner;
    if (result == "1-0") {
        winner = "White";
    } else if (result == "0-1") {
        winner = "Black";
    } else {
        winner = "Draw";
    }

    std::cout << "\nResult: " << result << " (" << winner << ")" << std::endl;

    std::cout << "Press enter to exit..." << std::endl;
    std::cin.ignore(); // Wait for user input before exiting
}

int main() {
    aiVsAi();
    return 0;
}

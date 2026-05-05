#include <iostream>
#include <string>
#include <vector>
#include <stdexcept>
#include <chess/board.hpp> // Example placeholder for a chess library
#include <chess/move.hpp>
#include <chess/engine.hpp> // Example placeholder for chess engine logic

// Function prototypes
void start();
std::string render(const chess::Board& board);
chess::Move getMove(const chess::Board& board);
int getDepth();

void start() {
    chess::Board board;
    char side;
    std::cout << "Start as [w]hite or [b]lack:\n";
    std::cin >> side;
    bool userSide = (side == 'w') ? chess::WHITE : chess::BLACK;

    if (userSide == chess::WHITE) {
        std::cout << render(board) << std::endl;
        board.push(getMove(board));
    }

    while (!board.isGameOver()) {
        board.push(chess::nextMove(getDepth(), board, false));
        std::cout << render(board) << std::endl;
        board.push(getMove(board));
    }

    std::cout << "\nResult: [w] " << board.result() << " [b]" << std::endl;
}

std::string render(const chess::Board& board) {
    // ASCII piece mapping
    std::unordered_map<char, char> asciiPieces = {
        {'R', 'R'}, {'N', 'N'}, {'B', 'B'}, {'Q', 'Q'}, {'K', 'K'}, {'P', 'P'},
        {'r', 'r'}, {'n', 'n'}, {'b', 'b'}, {'q', 'q'}, {'k', 'k'}, {'p', 'p'},
        {'.', '.'}
    };

    // Generate the board string
    std::vector<std::string> boardString = board.toString(); // Assuming a method to get board as string lines

    std::vector<std::string> translatedBoard;
    for (const auto& line : boardString) {
        std::string translatedLine;
        for (char ch : line) {
            translatedLine += asciiPieces[ch];
        }
        translatedBoard.push_back(translatedLine);
    }

    // Rank labels
    std::vector<std::string> ranks = {"8", "7", "6", "5", "4", "3", "2", "1"};
    
    // Reverse ranks if the turn is black
    if (board.getTurn() == chess::BLACK) {
        std::reverse(translatedBoard.begin(), translatedBoard.end());
        std::reverse(ranks.begin(), ranks.end());
    }

    // Construct display output
    std::string display;
    for (size_t i = 0; i < ranks.size(); ++i) {
        display += "  " + ranks[i] + " " + translatedBoard[i] + "\n";
    }
    display += "    a b c d e f g h\n";

    return display;
}

chess::Move getMove(const chess::Board& board) {
    std::string move;
    std::cout << "\nYour move (e.g. " << board.getLegalMoves().front() << "):\n";
    std::cin >> move;

    for (const auto& legalMove : board.getLegalMoves()) {
        if (move == legalMove.toString()) {
            return legalMove;
        }
    }

    return getMove(board); // Retry if move is illegal
}

int getDepth() {
    int depth = 3; // Default value
    if (auto depthStr = std::getenv("DEPTH")) {
        try {
            depth = std::stoi(depthStr);
        } catch (const std::invalid_argument& e) {
            std::cerr << "Invalid depth argument. Using default value.\n";
        }
    }
    return std::max(1, depth);
}

int main() {
    try {
        start();
    } catch (const std::exception& e) {
        std::cerr << "An error occurred: " << e.what() << std::endl;
    }
    return 0;
}

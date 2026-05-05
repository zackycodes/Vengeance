#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>
#include <cctype>
#include <algorithm>
#include "chess/board.hpp" // Placeholder for chess library
#include "movegeneration.hpp" // Placeholder for move generation

int getDepth() {
    int depth = 3; // Default depth
    std::string input;
    std::cout << "Enter depth (default 3): ";
    std::getline(std::cin, input);

    if (!input.empty()) {
        try {
            depth = std::stoi(input);
            if (depth < 1) {
                depth = 1;
            }
        } catch (...) {
            // In case of any conversion error, use default depth
        }
    }

    return depth;
}

void command(int depth, chess::Board& board, const std::string& msg) {
    std::string trimmedMsg = msg;
    trimmedMsg.erase(std::remove(trimmedMsg.begin(), trimmedMsg.end(), ' '), trimmedMsg.end());
    std::vector<std::string> tokens;
    std::istringstream iss(msg);
    std::string token;

    while (iss >> token) {
        tokens.push_back(token);
    }

    if (msg == "quit") {
        std::exit(0);
    }

    if (msg == "isready") {
        std::cout << "readyok" << std::endl;
        return;
    }

    if (msg == "ucinewgame") {
        return;
    }

    if (tokens[0] == "position") {
        if (tokens.size() < 2) {
            return;
        }

        // Set starting position
        if (tokens[1] == "startpos") {
            board.reset();
            size_t movesStart = 2;
            // Apply moves
            if (tokens.size() <= movesStart || tokens[movesStart] != "moves") {
                return;
            }

            for (size_t i = movesStart + 1; i < tokens.size(); ++i) {
                board.pushUci(tokens[i]);
            }
        } else if (tokens[1] == "fen") {
            std::string fen;
            for (size_t i = 2; i < 8; ++i) {
                fen += tokens[i] + " ";
            }
            board.setFen(fen);
            size_t movesStart = 8;
            // Apply moves
            if (tokens.size() <= movesStart || tokens[movesStart] != "moves") {
                return;
            }

            for (size_t i = movesStart + 1; i < tokens.size(); ++i) {
                board.pushUci(tokens[i]);
            }
        } else {
            return;
        }
    }

    if (msg == "d") {
        // Non-standard command, but supported by Stockfish and helps debugging
        std::cout << board << std::endl;
        std::cout << board.getFen() << std::endl;
    }

    if (msg.substr(0, 2) == "go") {
        std::string bestMove = nextMove(depth, board);
        std::cout << "bestmove " << bestMove << std::endl;
        return;
    }
}

void talk() {
    chess::Board board;
    int depth = getDepth();

    std::string msg;
    while (true) {
        std::getline(std::cin, msg);
        command(depth, board, msg);
    }
}

int main() {
    talk();
    return 0;
}

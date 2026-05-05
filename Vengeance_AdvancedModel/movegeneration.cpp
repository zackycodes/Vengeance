#include <iostream>
#include <vector>
#include <unordered_map>
#include <limits>
#include <chrono>
#include <memory>
#include "chess/board.hpp" // Placeholder for a chess library
#include "evaluate.hpp"    // Placeholder for evaluation functions

// Define constants
const int MATE_SCORE = 1000000000;
const int MATE_THRESHOLD = 999000000;

std::unordered_map<std::string, double> debug_info;

chess::Move nextMove(int depth, chess::Board& board, bool debug = true) {
    debug_info.clear();
    debug_info["nodes"] = 0;
    auto t0 = std::chrono::high_resolution_clock::now();

    chess::Move move = minimaxRoot(depth, board);

    auto t1 = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = t1 - t0;
    debug_info["time"] = elapsed.count();

    if (debug) {
        std::cout << "info nodes: " << debug_info["nodes"]
                  << ", time: " << debug_info["time"] << " seconds" << std::endl;
    }
    
    return move;
}

std::vector<chess::Move> getOrderedMoves(chess::Board& board) {
    bool end_game = checkEndGame(board);

    auto orderer = [&board, &end_game](const chess::Move& move) {
        return moveValue(board, move, end_game);
    };

    std::vector<chess::Move> moves = board.getLegalMoves();
    std::sort(moves.begin(), moves.end(), [&orderer, &board](const chess::Move& a, const chess::Move& b) {
        return orderer(a) > orderer(b);
    });

    if (board.getTurn() == chess::WHITE) {
        std::reverse(moves.begin(), moves.end());
    }

    return moves;
}

chess::Move minimaxRoot(int depth, chess::Board& board) {
    bool maximize = (board.getTurn() == chess::WHITE);
    double best_move_value = maximize ? -std::numeric_limits<double>::infinity() : std::numeric_limits<double>::infinity();
    chess::Move best_move;

    std::vector<chess::Move> moves = getOrderedMoves(board);

    for (const chess::Move& move : moves) {
        board.push(move);
        double value = board.canClaimDraw() ? 0.0 : minimax(depth - 1, board, -std::numeric_limits<double>::infinity(), std::numeric_limits<double>::infinity(), !maximize);
        board.pop();

        if ((maximize && value > best_move_value) || (!maximize && value < best_move_value)) {
            best_move_value = value;
            best_move = move;
        }
    }

    return best_move;
}

double minimax(int depth, chess::Board& board, double alpha, double beta, bool is_maximizing_player) {
    debug_info["nodes"] += 1;

    if (board.isCheckmate()) {
        return is_maximizing_player ? -MATE_SCORE : MATE_SCORE;
    } else if (board.isGameOver()) {
        return 0.0;
    }

    if (depth == 0) {
        return evaluateBoard(board);
    }

    double best_move_value = is_maximizing_player ? -std::numeric_limits<double>::infinity() : std::numeric_limits<double>::infinity();
    std::vector<chess::Move> moves = getOrderedMoves(board);

    for (const chess::Move& move : moves) {
        board.push(move);
        double current_value = minimax(depth - 1, board, alpha, beta, !is_maximizing_player);

        if (current_value > MATE_THRESHOLD) {
            current_value -= 1;
        } else if (current_value < -MATE_THRESHOLD) {
            current_value += 1;
        }

        if (is_maximizing_player) {
            best_move_value = std::max(best_move_value, current_value);
            alpha = std::max(alpha, best_move_value);
        } else {
            best_move_value = std::min(best_move_value, current_value);
            beta = std::min(beta, best_move_value);
        }

        board.pop();

        if (beta <= alpha) {
            break;
        }
    }

    return best_move_value;
}

int main() {
    // Example usage
    chess::Board board;
    int depth = 3; // Example depth
    try {
        chess::Move best_move = nextMove(depth, board);
        std::cout << "Best move: " << best_move.toString() << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "An error occurred: " << e.what() << std::endl;
    }
    return 0;
}

#include <iostream>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <stdexcept>
#include "chess/board.hpp" // Placeholder for chess library
#include "evaluate.hpp"    // Placeholder for evaluation functions

// Piece values
const std::unordered_map<chess::PieceType, int> pieceValue = {
    {chess::PAWN, 100},
    {chess::ROOK, 500},
    {chess::KNIGHT, 320},
    {chess::BISHOP, 330},
    {chess::QUEEN, 900},
    {chess::KING, 20000}
};

// Piece-square tables
const std::vector<int> pawnEvalWhite = {
    0, 0, 0, 0, 0, 0, 0, 0,
    5, 10, 10, -20, -20, 10, 10, 5,
    5, -5, -10, 0, 0, -10, -5, 5,
    0, 0, 0, 20, 20, 0, 0, 0,
    5, 5, 10, 25, 25, 10, 5, 5,
    10, 10, 20, 30, 30, 20, 10, 10,
    50, 50, 50, 50, 50, 50, 50, 50,
    0, 0, 0, 0, 0, 0, 0, 0
};
const std::vector<int> pawnEvalBlack(pawnEvalWhite.rbegin(), pawnEvalWhite.rend());

const std::vector<int> knightEval = {
    -50, -40, -30, -30, -30, -30, -40, -50,
    -40, -20, 0, 0, 0, 0, -20, -40,
    -30, 0, 10, 15, 15, 10, 0, -30,
    -30, 5, 15, 20, 20, 15, 5, -30,
    -30, 0, 15, 20, 20, 15, 0, -30,
    -30, 5, 10, 15, 15, 10, 5, -30,
    -40, -20, 0, 5, 5, 0, -20, -40,
    -50, -40, -30, -30, -30, -30, -40, -50
};

const std::vector<int> bishopEvalWhite = {
    -20, -10, -10, -10, -10, -10, -10, -20,
    -10, 5, 0, 0, 0, 0, 5, -10,
    -10, 10, 10, 10, 10, 10, 10, -10,
    -10, 0, 10, 10, 10, 10, 0, -10,
    -10, 5, 5, 10, 10, 5, 5, -10,
    -10, 0, 5, 10, 10, 5, 0, -10,
    -10, 0, 0, 0, 0, 0, 0, -10,
    -20, -10, -10, -10, -10, -10, -10, -20
};
const std::vector<int> bishopEvalBlack(bishopEvalWhite.rbegin(), bishopEvalWhite.rend());

const std::vector<int> rookEvalWhite = {
    0, 0, 0, 5, 5, 0, 0, 0,
    -5, 0, 0, 0, 0, 0, 0, -5,
    -5, 0, 0, 0, 0, 0, 0, -5,
    -5, 0, 0, 0, 0, 0, 0, -5,
    -5, 0, 0, 0, 0, 0, 0, -5,
    -5, 0, 0, 0, 0, 0, 0, -5,
    5, 10, 10, 10, 10, 10, 10, 5,
    0, 0, 0, 0, 0, 0, 0, 0
};
const std::vector<int> rookEvalBlack(rookEvalWhite.rbegin(), rookEvalWhite.rend());

const std::vector<int> queenEval = {
    -20, -10, -10, -5, -5, -10, -10, -20,
    -10, 0, 0, 0, 0, 0, 0, -10,
    -10, 0, 5, 5, 5, 5, 0, -10,
    -5, 0, 5, 5, 5, 5, 0, -5,
    0, 0, 5, 5, 5, 5, 0, -5,
    -10, 5, 5, 5, 5, 5, 0, -10,
    -10, 0, 5, 0, 0, 0, 0, -10,
    -20, -10, -10, -5, -5, -10, -10, -20
};

const std::vector<int> kingEvalWhite = {
    20, 30, 10, 0, 0, 10, 30, 20,
    20, 20, 0, 0, 0, 0, 20, 20,
    -10, -20, -20, -20, -20, -20, -20, -10,
    20, -30, -30, -40, -40, -30, -30, -20,
    -30, -40, -40, -50, -50, -40, -40, -30,
    -30, -40, -40, -50, -50, -40, -40, -30,
    -30, -40, -40, -50, -50, -40, -40, -30,
    -30, -40, -40, -50, -50, -40, -40, -30
};
const std::vector<int> kingEvalBlack(kingEvalWhite.rbegin(), kingEvalWhite.rend());

const std::vector<int> kingEvalEndGameWhite = {
    50, -30, -30, -30, -30, -30, -30, -50,
    -30, -30, 0, 0, 0, 0, -30, -30,
    -30, -10, 20, 30, 30, 20, -10, -30,
    -30, -10, 30, 40, 40, 30, -10, -30,
    -30, -10, 30, 40, 40, 30, -10, -30,
    -30, -10, 20, 30, 30, 20, -10, -30,
    -30, -20, -10, 0, 0, -10, -20, -30,
    -50, -40, -30, -20, -20, -30, -40, -50
};
const std::vector<int> kingEvalEndGameBlack(kingEvalEndGameWhite.rbegin(), kingEvalEndGameWhite.rend());

float evaluatePiece(const chess::Piece& piece, chess::Square square, bool endGame) {
    std::vector<int> mapping;

    switch (piece.getType()) {
        case chess::PAWN:
            mapping = (piece.getColor() == chess::WHITE) ? pawnEvalWhite : pawnEvalBlack;
            break;
        case chess::KNIGHT:
            mapping = knightEval;
            break;
        case chess::BISHOP:
            mapping = (piece.getColor() == chess::WHITE) ? bishopEvalWhite : bishopEvalBlack;
            break;
        case chess::ROOK:
            mapping = (piece.getColor() == chess::WHITE) ? rookEvalWhite : rookEvalBlack;
            break;
        case chess::QUEEN:
            mapping = queenEval;
            break;
        case chess::KING:
            mapping = (piece.getColor() == chess::WHITE)
                ? (endGame ? kingEvalEndGameWhite : kingEvalWhite)
                : (endGame ? kingEvalEndGameBlack : kingEvalBlack);
            break;
        default:
            throw std::invalid_argument("Unknown piece type");
    }

    return mapping[square];
}

float evaluateCapture(const chess::Board& board, const chess::Move& move) {
    if (board.isEnPassant(move)) {
        return pieceValue.at(chess::PAWN);
    }

    const chess::Piece* toPiece = board.getPieceAt(move.getToSquare());
    const chess::Piece* fromPiece = board.getPieceAt(move.getFromSquare());

    if (!toPiece || !fromPiece) {
        throw std::invalid_argument("Expected pieces at both squares");
    }

    return pieceValue.at(toPiece->getType()) - pieceValue.at(fromPiece->getType());
}

float moveValue(const chess::Board& board, const chess::Move& move, bool endGame) {
    if (move.getPromotion() != chess::NO_PIECE) {
        return (board.getTurn() == chess::WHITE) ? std::numeric_limits<float>::infinity() : -std::numeric_limits<float>::infinity();
    }

    const chess::Piece* piece = board.getPieceAt(move.getFromSquare());
    if (!piece) {
        throw std::invalid_argument("Expected a piece at from square");
    }

    float fromValue = evaluatePiece(*piece, move.getFromSquare(), endGame);
    float toValue = evaluatePiece(*piece, move.getToSquare(), endGame);
    float positionChange = toValue - fromValue;

    float captureValue = board.isCapture(move) ? evaluateCapture(board, move) : 0;

    float moveValue = captureValue + positionChange;
    return (board.getTurn() == chess::WHITE) ? moveValue : -moveValue;
}

float evaluateBoard(const chess::Board& board) {
    float total = 0;
    bool endGame = checkEndGame(board);

    for (chess::Square square : chess::SQUARES) {
        const chess::Piece* piece = board.getPieceAt(square);
        if (piece) {
            float pieceScore = pieceValue.at(piece->getType()) + evaluatePiece(*piece, square, endGame);
            total += (piece->getColor() == chess::WHITE) ? pieceScore : -pieceScore;
        }
    }

    return total;
}

bool checkEndGame(const chess::Board& board) {
    int queens = 0;
    int minors = 0;

    for (chess::Square square : chess::SQUARES) {
        const chess::Piece* piece = board.getPieceAt(square);
        if (piece) {
            if (piece->getType() == chess::QUEEN) {
                ++queens;
            } else if (piece->getType() == chess::BISHOP || piece->getType() == chess::KNIGHT) {
                ++minors;
            }
        }
    }

    return queens == 0 || (queens == 2 && minors <= 1);
}

import chess

# Values
piece_value = {
    chess.PAWN: 100,
    chess.ROOK: 500,
    chess.KNIGHT: 320,
    chess.BISHOP: 330,
    chess.QUEEN: 900,
    chess.KING: 20000
}

# Piece-Square Tables
pawnEvalWhite = [
    0, 0, 0, 0, 0, 0, 0, 0,
    5, 10, 10, -20, -20, 10, 10, 5,
    5, -5, -10, 0, 0, -10, -5, 5,
    0, 0, 0, 20, 20, 0, 0, 0,
    5, 5, 10, 25, 25, 10, 5, 5,
    10, 10, 20, 30, 30, 20, 10, 10,
    50, 50, 50, 50, 50, 50, 50, 50,
    0, 0, 0, 0, 0, 0, 0, 0
]
pawnEvalBlack = list(reversed(pawnEvalWhite))

knightEval = [
    -50, -40, -30, -30, -30, -30, -40, -50,
    -40, -20, 0, 0, 0, 0, -20, -40,
    -30, 0, 10, 15, 15, 10, 0, -30,
    -30, 5, 15, 20, 20, 15, 5, -30,
    -30, 0, 15, 20, 20, 15, 0, -30,
    -30, 5, 10, 15, 15, 10, 5, -30,
    -40, -20, 0, 5, 5, 0, -20, -40,
    -50, -40, -30, -30, -30, -30, -40, -50
]

bishopEvalWhite = [
    -20, -10, -10, -10, -10, -10, -10, -20,
    -10, 5, 0, 0, 0, 0, 5, -10,
    -10, 10, 10, 10, 10, 10, 10, -10,
    -10, 0, 10, 10, 10, 10, 0, -10,
    -10, 5, 5, 10, 10, 5, 5, -10,
    -10, 0, 5, 10, 10, 5, 0, -10,
    -10, 0, 0, 0, 0, 0, 0, -10,
    -20, -10, -10, -10, -10, -10, -10, -20
]
bishopEvalBlack = list(reversed(bishopEvalWhite))

rookEvalWhite = [
    0, 0, 0, 5, 5, 0, 0, 0,
    -5, 0, 0, 0, 0, 0, 0, -5,
    -5, 0, 0, 0, 0, 0, 0, -5,
    -5, 0, 0, 0, 0, 0, 0, -5,
    -5, 0, 0, 0, 0, 0, 0, -5,
    -5, 0, 0, 0, 0, 0, 0, -5,
    5, 10, 10, 10, 10, 10, 10, 5,
    0, 0, 0, 0, 0, 0, 0, 0
]
rookEvalBlack = list(reversed(rookEvalWhite))

queenEval = [
    -20, -10, -10, -5, -5, -10, -10, -20,
    -10, 0, 0, 0, 0, 0, 0, -10,
    -10, 0, 5, 5, 5, 5, 0, -10,
    -5, 0, 5, 5, 5, 5, 0, -5,
    0, 0, 5, 5, 5, 5, 0, -5,
    -10, 5, 5, 5, 5, 5, 0, -10,
    -10, 0, 5, 0, 0, 0, 0, -10,
    -20, -10, -10, -5, -5, -10, -10, -20
]

kingEvalWhite = [
    20, 30, 10, 0, 0, 10, 30, 20,
    20, 20, 0, 0, 0, 0, 20, 20,
    -10, -20, -20, -20, -20, -20, -20, -10,
    20, -30, -30, -40, -40, -30, -30, -20,
    -30, -40, -40, -50, -50, -40, -40, -30,
    -30, -40, -40, -50, -50, -40, -40, -30,
    -30, -40, -40, -50, -50, -40, -40, -30,
    -30, -40, -40, -50, -50, -40, -40, -30
]
kingEvalBlack = list(reversed(kingEvalWhite))

kingEvalEndGameWhite = [
    50, -30, -30, -30, -30, -30, -30, -50,
    -30, -30, 0, 0, 0, 0, -30, -30,
    -30, -10, 20, 30, 30, 20, -10, -30,
    -30, -10, 30, 40, 40, 30, -10, -30,
    -30, -10, 30, 40, 40, 30, -10, -30,
    -30, -10, 20, 30, 30, 20, -10, -30,
    -30, -20, -10, 0, 0, -10, -20, -30,
    -50, -40, -30, -20, -20, -30, -40, -50
]
kingEvalEndGameBlack = list(reversed(kingEvalEndGameWhite))


def move_value(board: chess.Board, move: chess.Move, endgame: bool) -> float:
    if move.promotion is not None:
        return float("inf") if board.turn == chess.WHITE else -float("inf")

    piece = board.piece_at(move.from_square)
    if piece is None:
        raise ValueError(f"Expected a piece at {move.from_square}")

    from_value = evaluate_piece(piece, move.from_square, endgame)
    to_value = evaluate_piece(piece, move.to_square, endgame)
    position_change = to_value - from_value

    capture_value = evaluate_capture(board, move) if board.is_capture(move) else 0

    move_value = capture_value + position_change
    return move_value if board.turn == chess.WHITE else -move_value


def evaluate_capture(board: chess.Board, move: chess.Move) -> float:
    if board.is_en_passant(move):
        return piece_value[chess.PAWN]
    
    to_piece = board.piece_at(move.to_square)
    from_piece = board.piece_at(move.from_square)
    
    if to_piece is None or from_piece is None:
        raise ValueError(f"Expected pieces at both {move.to_square} and {move.from_square}")

    return piece_value[to_piece.piece_type] - piece_value[from_piece.piece_type]


def evaluate_piece(piece: chess.Piece, square: chess.Square, end_game: bool) -> int:
    piece_type = piece.piece_type
    if piece_type == chess.PAWN:
        mapping = pawnEvalWhite if piece.color == chess.WHITE else pawnEvalBlack
    elif piece_type == chess.KNIGHT:
        mapping = knightEval
    elif piece_type == chess.BISHOP:
        mapping = bishopEvalWhite if piece.color == chess.WHITE else bishopEvalBlack
    elif piece_type == chess.ROOK:
        mapping = rookEvalWhite if piece.color == chess.WHITE else rookEvalBlack
    elif piece_type == chess.QUEEN:
        mapping = queenEval
    elif piece_type == chess.KING:
        mapping = kingEvalEndGameWhite if piece.color == chess.WHITE and end_game else kingEvalWhite
        mapping = kingEvalEndGameBlack if piece.color == chess.BLACK and end_game else kingEvalBlack

    return mapping[square]


def evaluate_board(board: chess.Board) -> float:
    total = 0
    end_game = check_end_game(board)

    for square in chess.SQUARES:
        piece = board.piece_at(square)
        if piece:
            piece_score = piece_value[piece.piece_type] + evaluate_piece(piece, square, end_game)
            total += piece_score if piece.color == chess.WHITE else -piece_score

    return total


def check_end_game(board: chess.Board) -> bool:
    queens = sum(1 for square in chess.SQUARES if (piece := board.piece_at(square)) and piece.piece_type == chess.QUEEN)
    minors = sum(1 for square in chess.SQUARES if (piece := board.piece_at(square)) and piece.piece_type in {chess.BISHOP, chess.KNIGHT})

    return queens == 0 or (queens == 2 and minors <= 1)


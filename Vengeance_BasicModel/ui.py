import chess
import argparse
from movegeneration import next_move

# START FROM SCRATCH 
#DAY1- start and render
#DAY2- get move, get depth
#DAY3- p-progress in state; new maximum available is 0
#DAY4- keyboard interrupts
#DAY5- bam! done finished by deadline

def start():
    board = chess.Board()
    user_side = (
        chess.WHITE if input("Start as [w]hite or [b]lack:\n").lower() == "w" else chess.BLACK
    )

    if user_side == chess.WHITE:
        print(render(board))
        board.push(get_move(board))

    while not board.is_game_over():
        board.push(next_move(get_depth(), board, debug=False))
        print(render(board))
        board.push(get_move(board))

    print(f"\nResult: [w] {board.result()} [b]")

def render(board: chess.Board) -> str:
    """
    print a side-relative chess board using ASCII characters.
    """
    # mapping of chess pieces to ASCII symbols
    ascii_pieces = {
        "R": "R", "N": "N", "B": "B", "Q": "Q", "K": "K", "P": "P",
        "r": "r", "n": "n", "b": "b", "q": "q", "k": "k", "p": "p",
        ".": "."
    }
    
    # Generate the board string
    board_string = str(board).split('\n')

    # Translate the board string into ASCII symbols
    translated_board = [
        ''.join(ascii_pieces.get(char, char) for char in line)
        for line in board_string
    ]

    # Generate the rank labels
    ranks = ["8", "7", "6", "5", "4", "3", "2", "1"]
    
    # Reverse ranks if the turn is black
    if board.turn == chess.BLACK:
        translated_board.reverse()
        ranks.reverse()

    # Construct the display output
    display = [f"  {rank} {line}" for rank, line in zip(ranks, translated_board)]
    display.append("    a b c d e f g h")

    return "\n".join(display)

def get_move(board: chess.Board) -> chess.Move:
    """
    Legality
    """
    move = input(f"\nYour move (e.g. {list(board.legal_moves)[0]}):\n")

    for legal_move in board.legal_moves:
        if move == str(legal_move):
            return legal_move
    return get_move(board)

def get_depth() -> int: # Ok i used chatgpt for syntax
    parser = argparse.ArgumentParser()
    parser.add_argument("--depth", default=3, help="provide an integer (default: 3)")
    args = parser.parse_args()
    return max([1, int(args.depth)])

if __name__ == "__main__":
    try:
        start()
    except KeyboardInterrupt: # Ok i used chatgpt for syntax
        pass


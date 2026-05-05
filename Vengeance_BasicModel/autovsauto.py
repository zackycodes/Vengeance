import chess
from movegeneration import next_move
from evaluate import check_end_game

def ai_vs_ai():
    board = chess.Board()
    depth = 3 #3 

    while not board.is_game_over():
        print(render(board))
        move = next_move(depth, board, debug=False)
        board.push(move)
        
        
        print(f"Move played: {move}")

    print(render(board))
    
    result = board.result()
    if result == "1-0":
        winner = "White"
    elif result == "0-1":
        winner = "Black"
    else:
        winner = "Draw"
    
    print(f"\nResult: {result} ({winner})")

    
    input("press enter to exit bruv I will add the functionality to 'play again' later im too lazy to code while loops rn")

def render(board: chess.Board) -> str:
    ascii_pieces = {
        "R": "R", "N": "N", "B": "B", "Q": "Q", "K": "K", "P": "P",
        "r": "r", "n": "n", "b": "b", "q": "q", "k": "k", "p": "p",
        ".": "."
    }
    
    
    board_string = str(board).split('\n')

    
    translated_board = [
        ''.join(ascii_pieces.get(char, char) for char in line)
        for line in board_string
    ]

    
    ranks = ["8", "7", "6", "5", "4", "3", "2", "1"]
    
    
    if board.turn == chess.BLACK:
        translated_board.reverse()
        ranks.reverse()

    
    display = [f"  {rank} {line}" for rank, line in zip(ranks, translated_board)]
    display.append("    a b c d e f g h")

    return "\n".join(display)

if __name__ == "__main__":
    ai_vs_ai()

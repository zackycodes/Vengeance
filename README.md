# Vengeance
A modular chess engine implemented in C++, Python that combines classic search algorithms with a sophisticated evaluation system. This engine supports interactive play, automated AI-vs-AI matches, and a command-based communication interface.

## Core Features
* __Search Engine__: Employs the Minimax algorithm with Alpha-Beta pruning to efficiently explore move trees
* __Move Optimization__: Implements Move Ordering through the getOrderedMoves function, which prioritizes moves based on their potential to improve pruning efficiency
* __Dynamic Evaluation__:
  * Calculates board state by combining material weights and Piece-Square Tables (PST)
  * Includes Endgame Detection (checkEndGame) that triggers specialized positioning logic for the King

* __Flexible Interfaces__:
  *__Interactive UI__: Play as White or Black in a terminal environment
  * __AI vs. AI Mode__: Watch the engine play against itself to test different search depths
  * __UCI-Style Communication__: A talk() function that allows for structured command processing and depth configuration

## Piece-Square Tables (PST)
The engine utilizes specialized tables to reward pieces for occupying strategically advantageous squares:
* __Pawns__: Incentivized to control the center and advance toward the 8th rank
* __Knights & Bishops__: Positioned to control the board center and avoid the edges
* __Rooks__: Rewarded for occupying the 7th rank and central files
* __King Safety__: Transition from defensive "Safety" tables in the mid-game to aggressive "Endgame" tables that encourage the King to centralize and assist in checkmating


## Technical Highlights
__Recursive Search__: The minimax function tracks "nodes" to provide debug information regarding search complexity
__Checkmate Recognition__: Uses a MATE_SCORE of 1,000,000,000 to identify and prioritize winning lines
__ASCII Rendering__: Includes a render function to display the board state in a human-readable ASCII format in the terminal


## How to Run
Configuration;
The default search depth is set to 3, but this can be adjusted via user input or environment variables (DEPTH) depending on the interface used

* __Standard Play__: Run the compiled binary to enter the interactive start() loop, where you can select your side and input moves in standard notation
* __Command Mode__: The engine can be initialized via the talk() function to receive direct string-based commands

## Notes
Have Fun!

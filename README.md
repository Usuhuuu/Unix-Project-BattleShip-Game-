# Unix-Project-BattleShip-Game
###22101191우수흐바야르
###22101219테리겔

Boat Battle Game is a two-player turn-based strategy game where players draw boats on a grid and take turns shooting at each other's boards. The game continues until one player destroys all the opponent's boats or after a set number of rounds.

## Table of Contents

- [Introduction](#introduction)
- [Code Structure](#code-structure)
- [Program features and descriptions](#game-initialization)
- [Running the Game](#running-the-game)
- [Follow On-Screen Instructions](#follow-on-screen-instructions)
- [Game Outcome](#game-outcome)

## Introduction <a name="introduction"></a>

Objective: Implement a two-player Battleship game in C using sockets for communication.
Players: Player 1 and Player 2.
Features: Drawing boats, taking turns, tracking hits and misses, and determining the winner.

## Code Structure <a name="code-structure"></a>
### Header Files
- `stdio.h, stdlib.h`: Standard input/output and standard library functions.
- `unistd.h: Symbolic` constants and types related to system calls.
- `string.h`: String manipulation functions.
- `sys/socket.h`: Socket-related functions.
- `netinet/in.h`: Internet address family and structures.


### Constants
- `BOARD_SIZE`: Size of the game board (8x8).
- `BOAT_NUMBER`: Number of boats each player has.
- `PORT`: Communication port for socket connections.

### Structs
- `BoatLocation`: Stores boat information (row, column, size, whether destroyed).
- `ShotResult`: Stores the result of a shot (hit, miss, destroyed, boat index).
- <img width="419" alt="Screenshot 2023-12-11 at 3 55 36 PM" src="https://github.com/Usuhuuu/Unix-Project-BattleShip-Game-/assets/101633545/b1cdda35-8be8-47ae-8006-4a3850b7f58f">



### 2. Program features and descriptions <a name="game-initialization"></a>:

#### Game Initialization:

- `initializeBoard`: Initializes the game boards.
- `printBoard`: Prints the current state of the board.
- <img width="424" alt="Screenshot 2023-12-11 at 3 56 39 PM" src="https://github.com/Usuhuuu/Unix-Project-BattleShip-Game-/assets/101633545/2280cb64-382f-40fd-ac35-619f1c829327">


#### Drawing Boats:

- `drawBoat`: Allows players to draw boats on their boards.
- `displayBoards`: Displays the player's and enemy's boards.

#### Game Logic:

- `updateBoard` and `processShot`: Update the board based on a shot result.
- `getShotLocation`: Prompts the user to input a shot location.

#### Turn Implementation:

- `player1Turn` and `player2Turn`: Implement turns for Player 1 and Player 2.
- `countRemainingBoats`: Counts the number of remaining boats for a player.

#### Main Game Loop:

- `playGame`: Main game loop where players take turns until a win condition is met.

## Running the Game <a name="running-the-game"></a>
```bash
gcc -o player2 player2.c -lsocket -std=c99
gcc -o player1 player1.c -lsocket -std=c99
```
```bash
./player1
./player2
```
these code must open on different terminal.


## Follow On-Screen Instructions <a name="follow-on-screen-instructions"></a>

- Input boat locations when prompted.
- Take turns shooting at each other's boards.

## Game Outcome <a name="game-outcome"></a>
- Our game is a standard turn-based Battleship game where players take shots at each other's boards, and the state of the boards is updated after each turn. The game continues until one player sinks all the ships of the other player.
- The game ends based on boat destruction or the number of rounds.
- The winner is determined, and the result is displayed.


## 1. Player's Board (Your Board):

Represented as an 8x8 grid.
Each cell on the player's board can be in one of two states:
‘point’: Represents water. This is the default state for unoccupied cells.
'X': Represents a hit. This symbol is used to mark cells where the opponent has successfully targeted and hit one of the player's ships.
![Player1](https://github.com/Usuhuuu/Unix-Project-BattleShip-Game-/assets/101633545/5fd75e6b-1eaf-4834-9227-585f7b28d898)
<img width="1512" alt="Player2" src="https://github.com/Usuhuuu/Unix-Project-BattleShip-Game-/assets/101633545/f611eec8-bb12-4a2f-8415-934a22dd7ec4">



 ## 2. Opponent's Board (Enemy Board):

Also represented as an 8x8 grid.
Each cell on the opponent's board can be in one of two states:
‘point’: Represents water. This is the default state for unoccupied cells and cells where the opponent has not yet targeted.
'X': Represents a hit. This symbol is used to mark cells where the player has successfully targeted and hit one of the opponent's ships.

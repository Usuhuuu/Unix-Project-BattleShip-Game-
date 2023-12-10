#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define BOARD_SIZE 8
#define BOAT_NUMBER 4
#define PORT 22101

typedef struct
{
    int row;
    int col;
    int size;
    int isDestroyed;
} BoatLocation;

typedef struct
{
    char result;
    int boatIndex;
} ShotResult;

char board[BOARD_SIZE][BOARD_SIZE];

void initializeBoard(char board[BOARD_SIZE][BOARD_SIZE])
{
    for (int i = 0; i < BOARD_SIZE; ++i)
    {
        for (int j = 0; j < BOARD_SIZE; ++j)
        {
            board[i][j] = '.';
        }
    }
}

void printBoard(char board[BOARD_SIZE][BOARD_SIZE])
{
    printf("  A B C D E F G H\n");

    for (int i = 0; i < BOARD_SIZE; ++i)
    {
        printf("%d ", i + 1);

        for (int j = 0; j < BOARD_SIZE; ++j)
        {
            printf("%c ", board[i][j]);
        }
        printf("\n");
    }
}

void drawBoat(BoatLocation boats[], int size, int playerNumber, char board[BOARD_SIZE][BOARD_SIZE])
{
    for (int i = 0; i < size; ++i)
    {
        do
        {
            printf("Player%d Please enter the location of boat #%d (format: %%d:H1:H2): ",
                   playerNumber, i + 1);

            char input[20]; // Adjust the size as needed
            if (scanf(" %19s", input) != 1)
            {
                // Handle invalid input (not a string)
                printf("Invalid input. Please enter the location in the specified format.\n");
                while (getchar() != '\n')
                    ;
                continue;
            }

            // Parse input and extract boat number and coordinates
            int boatNumber;
            char startCol, endCol;
            int startRow, endRow;
            if (sscanf(input, "%d:%c%d:%c%d", &boatNumber, &startCol, &startRow, &endCol, &endRow) != 5)
            {
                // Handle invalid input (format mismatch)
                printf("Invalid format. Please enter the location in the specified format.\n");
                while (getchar() != '\n')
                    ;
                continue;
            }

            // Convert row and column to 0-based indices
            char col = startCol - 'A';
            char endColIdx = endCol - 'A';
            int row = startRow - 1;
            int endRowIdx = endRow - 1;

            // Validate the indices
            if (row < 0 || row >= BOARD_SIZE || col < 0 || col >= BOARD_SIZE ||
                endRowIdx < 0 || endRowIdx >= BOARD_SIZE || endColIdx < 0 || endColIdx >= BOARD_SIZE)
            {
                printf("Invalid row or column indices. Please enter valid indices.\n");
                continue;
            }

            // Update the board and print it
            char boatSymbol = '0' + boatNumber;
            for (int r = row; r <= endRowIdx; ++r)
            {
                for (int c = col; c <= endColIdx; ++c)
                {
                    board[r][c] = boatSymbol;
                }
            }

            // Print the updated board
            printf("Player%d's Board After Drawing Boat #%d:\n", playerNumber, i + 1);
            printBoard(board);
            printf("\n");

            boats[i].row = row;
            boats[i].col = col;                  // Update the column index
            boats[i].size = endColIdx - col + 1; // Store the size of the boat

        } while (boats[i].row < 0 || boats[i].row >= BOARD_SIZE || boats[i].col < 0 || boats[i].col >= BOARD_SIZE);
    }
}

void displayBoards(char playerBoard[BOARD_SIZE][BOARD_SIZE], char enemyBoard[BOARD_SIZE][BOARD_SIZE])
{
    printf("Your Board:\n");
    printBoard(playerBoard);
    printf("\nEnemy Board:\n");
    printBoard(enemyBoard);
    printf("\n");
}

void updateBoard(char board[BOARD_SIZE][BOARD_SIZE], BoatLocation shot, char result, BoatLocation boats[], int size)
{
    // Check if the shot hit a boat
    int hitBoatIndex = -1;
    for (int i = 0; i < size; ++i)
    {
        if (boats[i].row == shot.row && boats[i].col == shot.col && boats[i].isDestroyed == 0)
        {
            hitBoatIndex = i;
            break;
        }
    }

    if (hitBoatIndex != -1)
    {
        boats[hitBoatIndex].isDestroyed = 1; // Mark the boat as destroyed
        board[shot.row][shot.col] = 'D';     // Use 'D' for destroyed boat
    }
    else
    {
        // Check if the shot is a miss
        if (result == 'M')
        {
            board[shot.row][shot.col] = 'X'; // Use 'X' for miss
        }
        else
        {
            board[shot.row][shot.col] = 'O'; // Use 'O' for hit
        }
    }
}

ShotResult processShot(BoatLocation shot, BoatLocation boats[], int size)
{
    ShotResult result;

    // Initialize result
    result.result = 'M'; // Default to miss
    result.boatIndex = -1;

    // Check if the shot hit a boat
    for (int i = 0; i < size; ++i)
    {
        if (!boats[i].isDestroyed && boats[i].row == shot.row && boats[i].col == shot.col)
        {
            // Mark the boat as destroyed
            boats[i].isDestroyed = 1;
            result.result = 'D'; // 'D' for destroyed
            result.boatIndex = i;
            break;
        }
        else if (boats[i].row <= shot.row && shot.row <= (boats[i].row + boats[i].size - 1) &&
                 boats[i].col <= shot.col && shot.col <= (boats[i].col + boats[i].size - 1))
        {
            // The boat was hit but not destroyed
            result.result = 'O'; // 'O' for hit
            result.boatIndex = i;
            break;
        }
    }

    return result;
}

int checkWin(BoatLocation boats[], int size)
{
    int allDestroyed = 1;

    for (int i = 0; i < size; ++i)
    {
        if (boats[i].isDestroyed == 0)
        {
            allDestroyed = 0;
            break;
        }
    }

    return allDestroyed;
}

void getShotLocation(BoatLocation *shot)
{
    do
    {
        printf("Enter the location of shoot (format: A1): ");
        char input[3];
        if (scanf(" %2s", input) != 1)
        {
            printf("invalid input, Please enter the location of specified format. \n");
            while (getchar() != '\n')
                ;
            continue;
        }
        char col;
        int row;
        if (sscanf(input, "%c%d", &col, &row) != 2)
        {
            printf("Invalid format. Please enter the location in the specified format.\n");
            while (getchar() != '\n')
                ;
            continue;
        }

        shot->row = row - 1;
        shot->col = col - 'A';

        if (shot->row < 0 || shot->row >= BOARD_SIZE || shot->col < 0 || shot->col >= BOARD_SIZE)
        {
            printf("Invalid row or column indices. Please enter valid indices.\n");
            continue;
        }
    } while (shot->row < 0 || shot->row >= BOARD_SIZE || shot->col < 0 || shot->col >= BOARD_SIZE);
}

void player2Turn(int client_socket, char player2Board[BOARD_SIZE][BOARD_SIZE], char player2EnemyBoard[BOARD_SIZE][BOARD_SIZE], BoatLocation player2Boats[BOAT_NUMBER])
{
    char turnDone[256] = "TURN_DONE";
    printf("Player 2 is waiting for Player 1's shot...\n");
    displayBoards(player2Board, player2EnemyBoard);

    BoatLocation player1Shot;
    BoatLocation player2Shot;

    ssize_t recv_result = recv(client_socket, &player1Shot, sizeof(player1Shot), 0);
    if (recv_result == -1 || recv_result == 0)
    {
        perror("Error receiving shot from Player 1");
        close(client_socket);
        exit(EXIT_FAILURE);
    }

    ShotResult player1Result = processShot(player1Shot, player2Boats, BOAT_NUMBER);

    updateBoard(player2Board, player2Shot, player1Result.result, player2Boats, BOARD_SIZE);
    ssize_t send_result = send(client_socket, &player1Result, sizeof(player1Result), 0);
    if (send_result == -1)
    {
        perror("Error sending result to Player 1");
        close(client_socket);
        exit(EXIT_FAILURE);
    }

    // Wait for Player 1 to complete their turn
    printf("Player 2 is waiting for Player 1 to complete the turn...\n");
    ssize_t recv_turn_done = recv(client_socket, turnDone, sizeof(turnDone), 0);
    if (recv_turn_done == -1 || recv_turn_done == 0)
    {
        perror("Error receiving turn completion signal from Player 1");
        close(client_socket);
        exit(EXIT_FAILURE);
    }
    printf("Player 2, it's your turn:\n");
    getShotLocation(&player2Shot);
    send(client_socket, &player2Shot, sizeof(player2Shot), 0);

    ShotResult player2Result;
    recv(client_socket, &player2Result, sizeof(player2Result), 0);

    // Update Player 1's enemy board based on the result
    updateBoard(player2EnemyBoard, player2Shot, player2Result.result, player2Boats, BOAT_NUMBER);

    if (player2Result.result == 'D')
    {
        printf("Player 1: You destroyed an enemy boat!\n");
    }
    else if (player2Result.result == 'O')
    {
        printf("Player 1: You hit an enemy boat!\n");
    }
    else
    {
        printf("Player 1: You missed!\n");
    }

    // Signal to Player 2 that Player 1 has completed the turn

    send(client_socket, turnDone, sizeof(turnDone), 0);
}

void playGame(int client_socket)
{
    char player2Board[BOARD_SIZE][BOARD_SIZE];
    char player2EnemyBoard[BOARD_SIZE][BOARD_SIZE];

    initializeBoard(player2Board);
    initializeBoard(player2EnemyBoard);
    BoatLocation player1Boats[BOAT_NUMBER];
    BoatLocation player2Boats[BOAT_NUMBER];
    drawBoat(player2Boats, BOAT_NUMBER, 2, player2Board);

    printf("Player 2 is waiting for Player 1 to start the game...\n");

    char confirmation[] = "READY";
    send(client_socket, confirmation, sizeof(confirmation), 0);
    char startGameSignal[10];
    recv(client_socket, &startGameSignal, sizeof(startGameSignal), 0);

    if (strcmp(startGameSignal, "READY") != 0)
    {
        // Handle unexpected signal
        printf("Unexpected signal from Player 1. Exiting the game.\n");
        close(client_socket);
        return;
    }

    int gameOver2 = 0;
    int gameStarted = 0;

    while (!gameOver2)
    {
        player2Turn(client_socket, player2Board, player2EnemyBoard, player1Boats);

        // Notify Player 2 to start their turn
    }

    close(client_socket);
}

int main()
{
    int client_socket;
    struct sockaddr_in server_addr;
    client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket == -1)
    {
        perror("socket create error");
        exit(EXIT_FAILURE);
    }
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    if (connect(client_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1)
    {
        perror("connection to player 1 error");
        close(client_socket);
        exit(EXIT_FAILURE);
    }
    printf("connected to player 1\n");
    printBoard(board);

    playGame(client_socket);

    close(client_socket);

    return 0;
}

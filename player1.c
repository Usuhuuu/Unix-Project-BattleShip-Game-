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
    char row;
    int col;
    int isDestroyed;
} BoatLocation;

typedef struct
{
    int boatIndex;
    char result;
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

BoatLocation convertInputToCoordinates(char row, int col)
{
    BoatLocation location;
    location.row = row - 'A';
    location.col = col - 1;
    return location;
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
            boats[i].col = col; // Update the column index

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
        else if (boats[i].row == shot.row && boats[i].col == shot.col)
        {
            // The boat was already destroyed
            result.result = 'O'; // 'O' for hit
            result.boatIndex = i;
            break;
        }
    }

    return result;
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
int checkWin(BoatLocation boats[], int size)
{
    // Check if all boats are destroyed
    for (int i = 0; i < size; ++i)
    {
        if (boats[i].isDestroyed == 0)
        {
            return 0; // Not all boats are destroyed
        }
    }
    return 1; // All boats are destroyed
}

void player1Turn(int client_socket, char player1Board[BOARD_SIZE][BOARD_SIZE], char player1EnemyBoard[BOARD_SIZE][BOARD_SIZE], BoatLocation player1Boats[BOAT_NUMBER])
{
    char turnDone[] = "TURN_DONE";
    printf("Player 1, it's your turn:\n");
    displayBoards(player1Board, player1EnemyBoard);
    int firstRound = 1;
    if (firstRound >= 1)
    {
        recv(client_socket, turnDone, sizeof(turnDone), 0);
    }
    else
    {
        // Player 1 takes a shot
        BoatLocation player1Shot;
        getShotLocation(&player1Shot);

        // Send Player 1's shot to Player 2
        ssize_t send_result = send(client_socket, &player1Shot, sizeof(player1Shot), 0);
        if (send_result == -1)
        {
            perror("error sending shot from Player2");
            close(client_socket);
            exit(EXIT_FAILURE);
        }

        // Receive the result of Player 1's shot from Player 2
        ShotResult player1Result;
        ssize_t recv_result = recv(client_socket, &player1Result, sizeof(player1Result), 0);

        if (recv_result == -1)
        {
            perror("error recieving from player2");
            close(client_socket);
            exit(EXIT_FAILURE);
        }
        // Update Player 1's enemy board based on the result
        updateBoard(player1EnemyBoard, player1Shot, player1Result.result, player1Boats, BOAT_NUMBER);

        if (player1Result.result == 'D')
        {
            printf("Player 1: You destroyed an enemy boat!\n");
        }
        else if (player1Result.result == 'O')
        {
            printf("Player 1: You hit an enemy boat!\n");
        }
        else
        {
            printf("Player 1: You missed!\n");
        }

        // Signal to Player 2 that Player 1 has completed the turn

        ssize_t send_turn_done = send(client_socket, turnDone, sizeof(turnDone), 0);
        if (send_turn_done == -1 || send_turn_done == 0)
        {
            perror("Error receiving turn completion signal from Player 1");
            close(client_socket);
            exit(EXIT_FAILURE);
        }
        if (strcmp(turnDone, "TURN_DONE") != 0)
        {
            // Handle unexpected signal
            printf("Unexpected signal from Player 1. Exiting the game.\n");
            close(client_socket);
            exit(EXIT_FAILURE);
        }

        // Now it's Player 1's turn to wait for Player 2's shot
        printf("Player 1 is waiting for Player 2's shot...\n");
        BoatLocation player2Shot;
        recv(client_socket, &player2Shot, sizeof(player2Shot), 0);

        // Process Player 2's shot and update the result accordingly
        ShotResult player2Result = processShot(player2Shot, player1Boats, BOAT_NUMBER);
        updateBoard(player1Board, player2Shot, player2Result.result, player1Boats, BOARD_SIZE);

        if (player2Result.result == 'D')
        {
            printf("Player 2: You destroyed an enemy boat!\n");
        }
        else if (player2Result.result == 'O')
        {
            printf("Player 2: You hit an enemy boat!\n");
        }
        else
        {
            printf("Player 2: You missed!\n");
        }

        // Send the result of Player 2's shot back to Player 2
        send(client_socket, &player2Result, sizeof(player2Result), 0);

        // Wait for Player 2 to complete their turn
        printf("Player 1 is waiting for Player 2 to complete the turn...\n");
        recv(client_socket, turnDone, sizeof(turnDone), 0);
        if (strcmp(turnDone, "TURN_DONE") != 0)
        {
            // Handle unexpected signal
            printf("Unexpected signal from Player 1. Exiting the game.\n");
            close(client_socket);
            return;
        }
    }

    firstRound++;
}

void playGame(int client_socket)
{
    char player1Board[BOARD_SIZE][BOARD_SIZE];
    char player1EnemyBoard[BOARD_SIZE][BOARD_SIZE];

    initializeBoard(player1Board);
    initializeBoard(player1EnemyBoard);

    // Draw boats for Player 1
    BoatLocation player1Boats[BOAT_NUMBER];
    BoatLocation player2Boats[BOAT_NUMBER];
    drawBoat(player1Boats, BOAT_NUMBER, 1, player1Board);

    printf("Player 1 is waiting for Player 2 to be ready...\n");
    char confirmation[] = "READY";
    send(client_socket, confirmation, sizeof(confirmation), 0);
    char notification[10];
    recv(client_socket, &notification, sizeof(notification), 0);
    if (strcmp(notification, "READY") != 0)
    {
        // Handle unexpected notification
        printf("Unexpected notification from Player 2. Exiting the game.\n");
        close(client_socket);
        return;
    }

    int gameOver1 = 0;
    while (!gameOver1)
    {
        player1Turn(client_socket, player1Board, player1EnemyBoard, player1Boats);

        // Notify Player 2 to start their turn
    }

    close(client_socket);
}

int main()
{
    int server_socket, client_socket;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_len = sizeof(client_addr);

    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1)
    {
        perror("error creating socket");
        exit(EXIT_FAILURE);
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1)
    {
        perror("error bind socket");
        exit(EXIT_FAILURE);
    }

    if (listen(server_socket, 1) == -1)
    {
        perror("Error listen for connect");
        exit(EXIT_FAILURE);
    }

    printf("player 1 is waiting for player 2 to connect\n");
    client_socket = accept(server_socket, (struct sockaddr *)&client_addr, &client_len);
    if (client_socket == -1)
    {
        perror("Error accepting connection");
        close(server_socket);
        exit(EXIT_FAILURE);
    }
    printBoard(board);

    printf("Player 2 connected\n");
    playGame(client_socket);
    close(server_socket);

    return 0;
}

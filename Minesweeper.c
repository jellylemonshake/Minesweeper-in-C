#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

#define MINE -1
#define HIDDEN -2
#define FLAG -3
#define MAX_SIZE 12  // Maximum board size

// Function prototypes
void initializeBoards(int size, int** board, int** displayBoard);
void placeMines(int size, int** board, int numMines, int firstRow, int firstCol);
void calculateNumbers(int size, int** board);
void revealEmpty(int size, int** board, int** displayBoard, int row, int col);
void printBoard(int size, int** displayBoard);
bool checkWin(int size, int** board, int** displayBoard, int numMines);

// Function to initialize the game boards
void initializeBoards(int size, int** board, int** displayBoard) {
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            board[i][j] = 0;
            displayBoard[i][j] = HIDDEN;
        }
    }
}

// Function to randomly place mines on the board
void placeMines(int size, int** board, int numMines, int firstRow, int firstCol) {
    int placedMines = 0;
    while (placedMines < numMines) {
        int row = rand() % size;
        int col = rand() % size;
        if (board[row][col] != MINE && (row != firstRow || col != firstCol)) {
            board[row][col] = MINE;
            placedMines++;
        }
    }
}

// Function to calculate numbers based on adjacent mines
void calculateNumbers(int size, int** board) {
    int dx[] = {-1, -1, -1, 0, 0, 1, 1, 1};
    int dy[] = {-1, 0, 1, -1, 1, -1, 0, 1};
    
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            if (board[i][j] == MINE) continue;
            
            int mineCount = 0;
            for (int k = 0; k < 8; k++) {
                int newRow = i + dx[k];
                int newCol = j + dy[k];
                
                if (newRow >= 0 && newRow < size && newCol >= 0 && newCol < size && 
                    board[newRow][newCol] == MINE) {
                    mineCount++;
                }
            }
            board[i][j] = mineCount;
        }
    }
}

// Function to recursively reveal empty cells
void revealEmpty(int size, int** board, int** displayBoard, int row, int col) {
    if (row < 0 || row >= size || col < 0 || col >= size || 
        displayBoard[row][col] != HIDDEN) {
        return;
    }
    
    displayBoard[row][col] = board[row][col];
    
    if (board[row][col] == 0) {
        for (int i = -1; i <= 1; i++) {
            for (int j = -1; j <= 1; j++) {
                revealEmpty(size, board, displayBoard, row + i, col + j);
            }
        }
    }
}

// Function to display the current game state
void printBoard(int size, int** displayBoard) {
    printf("\n    ");
    for (int col = 0; col < size; col++) {
        printf(" %2d ", col + 1);
    }
    printf("\n   +");
    for (int col = 0; col < size; col++) {
        printf("---+");
    }
    printf("\n");
    
    for (int row = 0; row < size; row++) {
        printf("%2d |", row + 1);
        for (int col = 0; col < size; col++) {
            if (displayBoard[row][col] == HIDDEN) {
                printf("   |");
            } else if (displayBoard[row][col] == FLAG) {
                printf("  F|");
            } else if (displayBoard[row][col] == MINE) {
                printf("  *|");
            } else if (displayBoard[row][col] == 0) {
                printf("  0|");  // Empty space for revealed cells with no adjacent mines
            } else {
                printf(" %2d|", displayBoard[row][col]);
            }
        }
        printf("\n   +");
        for (int col = 0; col < size; col++) {
            printf("---+");
        }
        printf("\n");
    }
}

// Function to check if the game is won
bool checkWin(int size, int** board, int** displayBoard, int numMines) {
    int hiddenCount = 0;
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            if (displayBoard[i][j] == HIDDEN || displayBoard[i][j] == FLAG) {
                if (board[i][j] != MINE) {
                    return false;
                }
                hiddenCount++;
            }
        }
    }
    return hiddenCount == numMines;
}

int main() {
    srand(time(0));
    int choice, size, difficulty, numMines;
    bool firstMove = true;
    
    // Get board size
    printf("Choose Minesweeper board size: 1. 4x4 2. 8x8 3. 12x12: ");
    scanf("%d", &choice);
    if (choice == 1) size = 4;
    else if (choice == 2) size = 8;
    else if (choice == 3) size = 12;
    else {
        printf("Invalid choice.\n");
        return 1;
    }
    
    // Get difficulty
    printf("Select difficulty level: 1. Easy 2. Medium 3. Hard: ");
    scanf("%d", &difficulty);
    if (difficulty == 1) numMines = (size * size) / 6;
    else if (difficulty == 2) numMines = (size * size) / 4;
    else if (difficulty == 3) numMines = (size * size) / 3;
    else {
        printf("Invalid choice.\n");
        return 1;
    }
    
    // Allocate memory for boards
    int** board = (int**)malloc(size * sizeof(int*));
    int** displayBoard = (int**)malloc(size * sizeof(int*));
    for (int i = 0; i < size; i++) {
        board[i] = (int*)malloc(size * sizeof(int));
        displayBoard[i] = (int*)malloc(size * sizeof(int));
    }
    
    initializeBoards(size, board, displayBoard);
    
    bool gameOver = false;
    
    while (!gameOver) {
        printBoard(size, displayBoard);
        
        int row, col;
        char action;
        
        printf("\nEnter row and column (1-%d): ", size);
        scanf("%d %d", &row, &col);
        row--; col--; // Convert to 0-based indexing
        
        if (row < 0 || row >= size || col < 0 || col >= size) {
            printf("Invalid coordinates! Try again.\n");
            continue;
        }
        
        printf("Enter action (R for reveal, F for flag): ");
        scanf(" %c", &action);
        
        if (action == 'F' || action == 'f') {
            if (displayBoard[row][col] == HIDDEN) {
                displayBoard[row][col] = FLAG;
            } else if (displayBoard[row][col] == FLAG) {
                displayBoard[row][col] = HIDDEN;
            }
        } else if (action == 'R' || action == 'r') {
            if (displayBoard[row][col] == FLAG) {
                printf("Unflag the cell first!\n");
                continue;
            }
            
            if (firstMove) {
                placeMines(size, board, numMines, row, col);
                calculateNumbers(size, board);
                firstMove = false;
            }
            
            if (board[row][col] == MINE) {
                // Game over - reveal all mines
                for (int i = 0; i < size; i++) {
                    for (int j = 0; j < size; j++) {
                        if (board[i][j] == MINE) {
                            displayBoard[i][j] = MINE;
                        }
                    }
                }
                printBoard(size, displayBoard);
                printf("\nGame Over! You hit a mine!\n");
                gameOver = true;
            } else {
                revealEmpty(size, board, displayBoard, row, col);
                if (checkWin(size, board, displayBoard, numMines)) {
                    printBoard(size, displayBoard);
                    printf("\nCongratulations! You've won!\n");
                    gameOver = true;
                }
            }
        }
    }
    
    // Free allocated memory
    for (int i = 0; i < size; i++) {
        free(board[i]);
        free(displayBoard[i]);
    }
    free(board);
    free(displayBoard);
    
    return 0;
}
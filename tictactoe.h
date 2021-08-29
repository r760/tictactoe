#include <limits.h>
#include <ncurses.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

char board[3][3], defaultCharacter = '?', aiPlayer = 'X', realPlayer = 'O';
bool aiTurn = true, stay = true;

/*
 * Utility Functions
 */

/**
 * set all board pieces to c
 */
void set(char c) {
  int y, x;
  for (y = 0; y < 3; y++) {
    for (x = 0; x < 3; x++) {
      board[y][x] = c;
    }
  }
}

/**
 * return INT_MAX if ai player wins
 * return INT_MIN if real player wins
 * return 0 if tie
 */
int getScore() {
  int y, x;
  bool foundWinner = false;

  // horizontal check
  // (y,0) (y,1) (y,2)

  for (y = 0; y < 3; y++) {
    foundWinner = board[y][0] != defaultCharacter &&
                  board[y][0] == board[y][1] && board[y][1] == board[y][2];
    if (foundWinner) {
      if (board[y][0] == aiPlayer) {
        return INT_MAX;
      } else {
        return INT_MIN;
      }
    }
  }

  // vertical check
  // (0,x) (1,x) (2,x)

  for (int x = 0; x < 3; x++) {
    foundWinner = board[0][x] != defaultCharacter &&
                  board[0][x] == board[1][x] && board[1][x] == board[2][x];
    if (foundWinner) {
      if (board[0][x] == aiPlayer) {
        return INT_MAX;
      } else {
        return INT_MIN;
      }
    }
  }

  // diagonal check
  // (0,0) (1,1) (2,2)
  // (0,2) (1,1) (2,0)

  foundWinner = (board[0][0] != defaultCharacter &&
                 board[0][0] == board[1][1] && board[1][1] == board[2][2]) ||
                (board[0][2] != defaultCharacter &&
                 board[0][2] == board[1][1] && board[1][1] == board[2][0]);

  if (foundWinner) {
    if (board[1][1] == aiPlayer) {
      return INT_MAX;
    } else {
      return INT_MIN;
    }
  }
  return 0;
}

/**
 * return true if the game is over
 * return false otherwise
 */
bool gameOver() {
  int score = getScore();
  if (score == INT_MIN || score == INT_MAX) {
    return true;
  } else {
    int y, x;
    for (y = 0; y < 3; y++) {
      for (x = 0; x < 3; x++) {
        if (board[y][x] == defaultCharacter) {
          return false;
        }
      }
    }
    return true;
  }
}

/**
 * return the min between a and b
 */
int min(int a, int b) {
  if (a <= b) {
    return a;
  } else {
    return b;
  }
}

/**
 * return the max between a and b
 */
int max(int a, int b) {
  if (a >= b) {
    return a;
  } else {
    return b;
  }
}

/*
 * IO Functions
 */

void print(char *temp, int y, int x) {
  int i = 0;
  while (temp[i] != '\0') {
    mvaddch(y, x++, temp[i++]);
  }
}

void printBindings() {
  char *bindings[7] = {"Commands:",        "q - Quit\n",      "h - Move left\n",
                       "l - Move right\n", "j - Move down\n", "k - Move Up\n",
                       "e - Place \'O\'\n"};

  int i, y = 2, x = 25;
  for (i = 0; i < 7; i++) {
    if (i == 1) {
      x++;
    }
    print(bindings[i], y++, x);
  }
}

void printBoard() {
  int y, temp;
  temp = 2;
  for (y = 0; y < 3; y++) {
    move(temp, 5);
    printw("+---+---+---+\n");
    move(temp + 1, 5);
    printw("| %c | %c | %c |\n", board[y][0], board[y][1], board[y][2]);
    temp += 2;
  }
  move(temp, 5);
  printw("+---+---+---+\n");
}

void gameOverHandler() {
  char c, *status;
  int score = getScore();
  if (score >= 1) {
    status = "Game over: You lose!\n";
  } else if (score <= -1) {
    status = "Game over: You win!\n";
  } else {
    status = "Game over: Tie!\n";
  }

  while (true) {
    clear();
    printBoard();
    print("Commands:\n", 2, 25);
    print("q - Quit\n", 3, 26);
    print("n - New game\n", 4, 26);
    print(status, 10, 5);
    move(10, 25);
    refresh();

    c = getch();
    if (c == 'q') {
      stay = false;
      break;
    } else if (c == 'n') {
      set(defaultCharacter);
      break;
    }
  }
}
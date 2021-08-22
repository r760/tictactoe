#include <limits.h>
#include <ncurses.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

char board[3][3], DEFAULT_CHARACTER = '?';
bool ai_turn = true, stay = true;

/*
 Utility Functions
*/

/**
  set all board pieces to c
*/
void set(char c) {
  int i, j, length;
  for (i = 0; i < 3; i++) {
    for (j = 0; j < 3; j++) {
      board[i][j] = c;
    }
  }
}

/**
  return 100 if player X wins
  return -100 if player O wins
  return 0 if tie
*/
int getScore() {
  int i;
  bool foundWinner = false;
  int score = 0;

  // horizontal check
  // (i,0) (i,1) (i,2)

  for (i = 0; foundWinner == false && i < 3; i++) {
    foundWinner = board[i][0] != DEFAULT_CHARACTER &&
                  board[i][0] == board[i][1] && board[i][1] == board[i][2];
    if (foundWinner) {
      if (board[i][0] == 'X')
        score = 100;
      else
        score = -100;
    }
  }

  // vertical check
  // (0,i) (1,i) (2,i)

  for (int i = 0; foundWinner == false && i < 3; i++) {
    foundWinner = board[0][i] != DEFAULT_CHARACTER &&
                  board[0][i] == board[1][i] && board[1][i] == board[2][i];
    if (foundWinner) {
      if (board[0][i] == 'X')
        score = 100;
      else
        score = -100;
    }
  }

  // diagonal check
  // (0,0) (1,1) (2,2)
  // (0,2) (1,1) (2,0)

  if (foundWinner == false) {
    foundWinner = board[0][0] != DEFAULT_CHARACTER &&
                  board[0][0] == board[1][1] && board[1][1] == board[2][2];
    if (foundWinner == false)
      foundWinner = board[0][2] != DEFAULT_CHARACTER &&
                    board[0][2] == board[1][1] && board[1][1] == board[2][0];

    if (foundWinner) {
      if (board[1][1] == 'X')
        score = 100;
      else
        score = -100;
    }
  }

  return score;
}

/**
  return true if the game is over
  return false otherwise
*/
bool isGameOver() {
  bool gameOver = getScore() != 0;
  if (gameOver == false) {
    int i, j;
    gameOver = true;
    for (i = 0; i < 3; i++) {
      for (j = 0; j < 3; j++) {
        gameOver = gameOver && (board[i][j] != DEFAULT_CHARACTER);
      }
    }
  }
  return gameOver;
}

/*
  IO Functions
*/

void print(char *temp, int y, int x) {
  int i = 0;
  while (temp[i] != '\0') {
    mvaddch(y, x++, temp[i++]);
  }
}

void printBindings() {
  char *bindings[7] = {"Command:",         "q - Quit\n",      "h - Move left\n",
                       "l - Move right\n", "j - Move down\n", "k - Move Up\n",
                       "e - Place \'O\'\n"};

  int i, y = 2, x = 25;
  for (i = 0; i < 7; i++) {
    if (i == 1)
      x++;
    print(bindings[i], y++, x);
  }
}

void printBoard() {
  int i = 0;
  move(2, 5);
  printw("+---+---+---+\n");
  move(3, 5);
  printw("| %c | %c | %c |\n", board[i][0], board[i][1], board[i][2]);
  i++;
  move(4, 5);
  printw("+---+---+---+\n");
  move(5, 5);
  printw("| %c | %c | %c |\n", board[i][0], board[i][1], board[i][2]);
  i++;
  move(6, 5);
  printw("+---+---+---+\n");
  move(7, 5);
  printw("| %c | %c | %c |\n", board[i][0], board[i][1], board[i][2]);
  move(8, 5);
  printw("+---+---+---+\n");
}

void gameStatus() {
  char c;
  int score = getScore();
  char *status;
  if (score >= 1)
    status = "Game over: You lose!\n";
  else if (score <= -1)
    status = "Game over: You win!\n";
  else
    status = "Game over: Tie!\n";

  while (true) {
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
      set(DEFAULT_CHARACTER);
      break;
    }
  }
}
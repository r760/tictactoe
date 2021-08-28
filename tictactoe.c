#include "tictactoe.h"

/**
  the minimax algorithm
*/
int minimax(int depth, bool maximizingPlayer) {
  if (gameOver()) {
    int score = getScore();

    if (score <= -1) {
      score += depth;
    } else if (score >= 1) {
      score -= depth;
    }

    return score;
  } else {
    int value, y, x;

    if (maximizingPlayer) {
      value = INT_MIN;

      for (y = 0; y < 3; y++) {
        for (x = 0; x < 3; x++) {
          if (board[y][x] == DEFAULT_CHARACTER) {
            board[y][x] = 'X';
            int temp = minimax(depth + 1, false);
            board[y][x] = DEFAULT_CHARACTER;
            value = max(value, temp);
          }
        }
      }

      return value;
    } else {
      value = INT_MAX;

      for (y = 0; y < 3; y++) {
        for (x = 0; x < 3; x++) {
          if (board[y][x] == DEFAULT_CHARACTER) {
            board[y][x] = 'O';
            int temp = minimax(depth + 1, true);
            board[y][x] = DEFAULT_CHARACTER;
            value = min(value, temp);
          }
        }
      }

      return value;
    }
  }
}

/**
  using the minimax algorithm,
  the ai places X intelligently
*/
void ai() {
  int value, y, x, yFin, xFin;
  value = INT_MIN;
  yFin = -1;
  xFin = -1;

  for (y = 0; y < 3; y++) {
    for (x = 0; x < 3; x++) {
      if (board[y][x] == DEFAULT_CHARACTER) {
        board[y][x] = 'X';
        int temp = minimax(0, true);

        if (temp > value) {
          yFin = y;
          xFin = x;
          value = temp;
        }
        board[y][x] = DEFAULT_CHARACTER;
      }
    }
  }
  board[yFin][xFin] = 'X';
}

int main() {
  set(DEFAULT_CHARACTER);
  initscr();

  int y = 3, x = 7;

  while (stay) {
    if (aiTurn) {
      ai();
      if (gameOver()) {
        gameOverHandler();
        if (stay) {
          ai();
        }
      }
      aiTurn = false;
    }

    clear();
    printBoard();
    printBindings();
    move(y, x);
    refresh();

    if (stay) {
      char c = getch();
      if (c == 'q') {
        stay = false;
      } else if ((c == 'l') && x < 7 + 4 + 4) {
        x += 4;
      } else if ((c == 'h') && x > 7) {
        x -= 4;
      } else if ((c == 'j') && y < 3 + 2 + 2) {
        y += 2;
      } else if ((c == 'k') && y > 3) {
        y -= 2;
      } else if ((c == 'e') &&
                 board[(y - 3) / 2][(x - 7) / 4] == DEFAULT_CHARACTER) {
        board[(y - 3) / 2][(x - 7) / 4] = 'O';
        if (gameOver()) {
          gameOverHandler();
        }
        if (stay) {
          aiTurn = true;
        }
      }
    }
  }

  endwin();
  return 0;
}
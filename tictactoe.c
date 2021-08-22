#include "tictactoe.h"

/**
  return the min between a and b
  */
int min(int a, int b) {
  if (a <= b)
    return a;
  else
    return b;
}

/**
  return the max between a and b
 */
int max(int a, int b) {
  if (a >= b)
    return a;
  else
    return b;
}

/**
  the minimax algorithm
*/
int minimax(int depth, bool maximizingPlayer) {
  if (isGameOver()) {
    int temp = getScore();

    if (temp <= -1) {
      temp += depth;
    } else if (temp >= 1) {
      temp -= depth;
    }

    return temp;
  } else {
    int value, i, j;
    if (maximizingPlayer) {
      value = INT_MIN;
      for (i = 0; i < 3; i++) {
        for (j = 0; j < 3; j++) {
          if (board[i][j] == DEFAULT_CHARACTER) {
            board[i][j] = 'X';
            int temp = minimax(depth + 1, false);
            board[i][j] = DEFAULT_CHARACTER;
            value = max(value, temp);
          }
        }
      }
      return value;
    } else {
      value = INT_MAX;
      for (i = 0; i < 3; i++) {
        for (j = 0; j < 3; j++) {
          if (board[i][j] == DEFAULT_CHARACTER) {
            board[i][j] = 'O';
            int temp = minimax(depth + 1, true);
            board[i][j] = DEFAULT_CHARACTER;
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
void aiTurn() {
  int value, i, j, k, l;
  value = INT_MIN;
  k = -1;
  l = -1;
  for (i = 0; i < 3; i++) {
    for (j = 0; j < 3; j++) {
      if (board[i][j] == DEFAULT_CHARACTER) {
        board[i][j] = 'X';
        int temp = minimax(0, true);
        if (temp > value) {
          k = i;
          l = j;
          value = temp;
        }
        board[i][j] = DEFAULT_CHARACTER;
      }
    }
  }
  board[k][l] = 'X';
}

int main() {
  set(DEFAULT_CHARACTER);
  initscr();

  int i = 3, j = 7;

  while (stay) {
    if (ai_turn == true) {
      // ai turn
      aiTurn(DEFAULT_CHARACTER);

      // is the game over?
      if (isGameOver(DEFAULT_CHARACTER) == true) {
        // exit
        gameStatus();
        if (stay)
          aiTurn(DEFAULT_CHARACTER);
      }
      ai_turn = false;
    }

    clear();
    printBoard();
    printBindings();
    move(i, j);
    refresh();

    if (stay) {
      char c = getch();
      if (c == 'q') {
        stay = false;
      } else if ((c == 'l') && j < 7 + 4 + 4) {
        j += 4;
      } else if ((c == 'h') && j > 7) {
        j -= 4;
      } else if ((c == 'j') && i < 3 + 2 + 2) {
        i += 2;
      } else if ((c == 'k') && i > 3) {
        i -= 2;
      } else if (c == 'e') {
        if (board[(i - 3) / 2][(j - 7) / 4] == DEFAULT_CHARACTER) {
          // player turn
          board[(i - 3) / 2][(j - 7) / 4] = 'O';

          // is the game over?
          if (isGameOver(DEFAULT_CHARACTER) == true)
            gameStatus();

          if (stay)
            ai_turn = true;
        }
      }
    }
  }

  endwin();
  return 0;
}
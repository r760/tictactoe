#include <limits.h>
#include <ncurses.h>
#include <stdbool.h>
#include <string.h>

#define ROWS 3
#define COLS 3

#define ai_char      'X'
#define player_char  'O'
#define default_char '?'

#define UP_KEY(c)     (c == 'k' || c == 'w')
#define DOWN_KEY(c)   (c == 'j' || c == 's')
#define LEFT_KEY(c)   (c == 'h' || c == 'a')
#define RIGHT_KEY(c)  (c == 'l' || c == 'd')

/**
 * print in-game keys (or controls)
 */
void nprint_keys()
{
    int size = 7;

    char *keys[] = {
        "Keys:",
        "q - Quit\n",
        "h (or a) - Move left\n",
        "l (or d) - Move right\n",
        "j (or s) - Move down\n",
        "k (or w) - Move up\n",
        "e - Place \'O\'\n"
    };

    for (int i = 0, y = 2, x = 25; i < size; i++, y++) {
        if (i == 0) {
            mvprintw(y, x, "%s", keys[i]);
        } else {
            mvprintw(y, x + 1, "%s", keys[i]);
        }
    }
}

/**
 * print keys (or controls) when game is over
 */
void nprint_game_over_keys()
{
    int size = 3;

    char *keys[] = {
        "Keys:",
        "q - Quit\n",
        "n - New game\n"
    };

    for (int i = 0, y = 2, x = 25; i < size; i++, y++) {
        if (i == 0) {
            mvprintw(y, x, "%s", keys[i]);
        } else {
            mvprintw(y, x + 1, "%s", keys[i]);
        }
    }
}

/**
 * print board
 */
void nprint_board(char board[ROWS][COLS])
{
    int y;
    for (y = 0; y < ROWS; y++) {
        mvprintw(2 * (y + 1), 5, "+---+---+---+\n");
        mvprintw(2 * (y + 1) + 1, 5, "| %c | %c | %c |\n", board[y][0], board[y][1], board[y][2]);
    }
    mvprintw(2 * (y + 1), 5, "+---+---+---+\n");
}

/**
 * print score
 */
void nprint_score(int *ai_win_count, int *player_win_count)
{
    mvprintw(9, 4, "AI (%d) - YOU (%d)\n", *ai_win_count, *player_win_count);
}

/**
 * set board pieces
 */
void set(char board[ROWS][COLS], char c)
{
    for (int row = 0; row < ROWS; row++) {
        for (int col = 0; col < COLS; col++) {
            board[row][col] = c;
        }
    }
}

/**
 * get game status
 *
 * return INT_MAX if ai player has won
 * return INT_MIN if player has won
 * return 0 if ai and player has tied
 */
int game_status(char board[ROWS][COLS])
{
    bool found_winner = false;

    // horizontal check
    // (row,0) (row,1) (row,2)

    for (int row = 0; row < ROWS; row++) {
        found_winner = 
            board[row][0] != default_char &&
            board[row][0] == board[row][1] &&
            board[row][1] == board[row][2];

        if (found_winner) {
            if (board[row][0] == ai_char) {
                return INT_MAX;
            } else {
                return INT_MIN;
            }
        }
    }

    // vertical check
    // (0,col) (1,col) (2,col)

    for (int col = 0; col < COLS; col++) {
        found_winner =
            board[0][col] != default_char &&
            board[0][col] == board[1][col] &&
            board[1][col] == board[2][col];

        if (found_winner) {
            if (board[0][col] == ai_char) {
                return INT_MAX;
            } else {
                return INT_MIN;
            }
        }
    }

    // diagonal check
    // (0,0) (1,1) (2,2)
    // (0,2) (1,1) (2,0)

    found_winner =
        (
         board[0][0] != default_char &&
         board[0][0] == board[1][1] &&
         board[1][1] == board[2][2]
        )
        ||
        (
         board[0][2] != default_char &&
         board[0][2] == board[1][1] &&
         board[1][1] == board[2][0]
        );

    if (found_winner) {
        if (board[1][1] == ai_char) {
            return INT_MAX;
        } else {
            return INT_MIN;
        }
    }
    return 0;
}

/**
 * check if the game is over
 *
 * return true if the game is over
 * return false if the game is not over
 */
bool game_over(char board[ROWS][COLS])
{
    int status = game_status(board);
    if (status == INT_MIN || status == INT_MAX) {
        return true;
    } else {
        for (int row = 0; row < ROWS; row++) {
            for (int col = 0; col < COLS; col++) {
                if (board[row][col] == default_char) {
                    return false;
                }
            }
        }
        return true;
    }
}

/**
 * the game is over, ask the user if they want to quit or continue
 */
void game_over_handler(char board[ROWS][COLS], int *ai_win_count, int *player_win_count, bool *stay)
{
    char c, *str;
    int status = game_status(board);

    if (status >= 1) {
        str = "Game over: You lose!\n";
        *ai_win_count = *ai_win_count + 1;
    } else if (status <= -1) {
        str = "Game over: You win!\n";
        *player_win_count = *player_win_count + 1;
    } else {
        str = "Game over: Tie!\n";
    }

    while (true) {
        clear();
        nprint_score(ai_win_count, player_win_count);
        nprint_board(board);
        nprint_game_over_keys();
        mvprintw(6, 25, "%s", str);
        move(6, strlen(str) + 25 - 1);
        refresh();

        c = getch();
        if (c == 'q') {
            *stay = false;
            break;
        } else if (c == 'n') {
            set(board, default_char);
            break;
        }
    }
}

/**
 * the minimax algorithm
 */
int minimax(char board[ROWS][COLS], int *y, int *x, int depth, bool maximizing_player)
{
    if (game_over(board)) {
        int status = game_status(board);

        if (status <= -1) {
            status += depth;
        } else if (status >= 1) {
            status -= depth;
        }

        return status;
    } else {
        int value, row, col;

        if (maximizing_player) {
            value = INT_MIN;

            for (row = 0; row < ROWS; row++) {
                for (col = 0; col < COLS; col++) {
                    if (board[row][col] == default_char) {
                        board[row][col] = ai_char;
                        int temp = minimax(board, y, x, depth + 1, false);
                        board[row][col] = default_char;
                        if (temp > value) {
                            value = temp;
                            *y = row;
                            *x = col;
                        }
                    }
                }
            }

            return value;
        } else {
            value = INT_MAX;

            for (row = 0; row < ROWS; row++) {
                for (col = 0; col < COLS; col++) {
                    if (board[row][col] == default_char) {
                        board[row][col] = player_char;
                        int temp = minimax(board, y, x, depth + 1, true);
                        board[row][col] = default_char;
                        if (temp < value) {
                            value = temp;
                        }
                    }
                }
            }

            return value;
        }
    }
}

/**
 * the ai places 'X' intelligently on the board
 */
void ai(char board[ROWS][COLS])
{
    int y = -1, x = -1;
    minimax(board, &y, &x, 0, true);
    if (y >= 0 && y <= ROWS && x >= 0 && x <= COLS) {
        board[y][x] = ai_char;
    }
}

int main(void)
{
    char board[ROWS][COLS];
    bool ai_turn = false, stay = true;
    int ai_win_count = 0, player_win_count = 0, y = 3, x = 7;

    set(board, default_char);

    initscr();

    while (stay) {
        if (ai_turn) {
            ai(board);
            if (game_over(board)) {
                game_over_handler(board, &ai_win_count, &player_win_count, &stay);
            }
            ai_turn = false;
        }

        clear();
        nprint_score(&ai_win_count, &player_win_count);
        nprint_board(board);
        nprint_keys();
        move(y, x);
        refresh();

        if (stay) {
            char c = getch();
            if (c == 'q') {
                stay = false;
            } else if (RIGHT_KEY(c) && x < 15) {
                x += 4;
            } else if (LEFT_KEY(c) && x > 7) {
                x -= 4;
            } else if (DOWN_KEY(c) && y < 7) {
                y += 2;
            } else if (UP_KEY(c) && y > 3) {
                y -= 2;
            } else if (c == 'e' && board[(y - 3) / 2][(x - 7) / 4] == default_char) {
                board[(y - 3) / 2][(x - 7) / 4] = player_char;
                if (game_over(board)) {
                    game_over_handler(board, &ai_win_count, &player_win_count, &stay);
                    ai_turn = false;
                } else if (stay) {
                    ai_turn = true;
                }
            }
        }
    }

    endwin();
    return 0;
}

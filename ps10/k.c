#include <stdbool.h>
#include "k.h"

static int value(char c) {
    return 1 << (c - 'A' + 1);
}

bool is_game_won(const struct game game) {
    for (int i = 0; i < SIZE; i++)
        for (int j = 0; j < SIZE; j++)
            if (game.board[i][j] == 'K')
                return true;
    return false;
}

bool is_move_possible(const struct game game) {
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            if (game.board[i][j] == ' ')
                return true;

            if (j < SIZE-1 && game.board[i][j] == game.board[i][j+1])
                return true;

            if (i < SIZE-1 && game.board[i][j] == game.board[i+1][j])
                return true;
        }
    }
    return false;
}

bool update(struct game *game, int dy, int dx) {
    if ((dx != 0 && dy != 0) || (dx == 0 && dy == 0))
        return false;

    bool changed = false;

    for (int i = 0; i < SIZE; i++) {
        char line[SIZE];
        int idx = 0;

        // чтение линии
        for (int j = 0; j < SIZE; j++) {
            char c;
            if (dx == 1) c = game->board[i][SIZE-1-j];
            else if (dx == -1) c = game->board[i][j];
            else if (dy == 1) c = game->board[SIZE-1-j][i];
            else c = game->board[j][i];

            if (c != ' ')
                line[idx++] = c;
        }

        // merge
        for (int j = 0; j < idx-1; j++) {
            if (line[j] == line[j+1]) {
                line[j]++;
                game->score += value(line[j]);
                for (int k = j+1; k < idx-1; k++)
                    line[k] = line[k+1];
                idx--;
                changed = true;
            }
        }

        // запись обратно
        for (int j = 0; j < SIZE; j++) {
            char val = (j < idx) ? line[j] : ' ';

            if (dx == 1) {
                if (game->board[i][SIZE-1-j] != val) changed = true;
                game->board[i][SIZE-1-j] = val;
            }
            else if (dx == -1) {
                if (game->board[i][j] != val) changed = true;
                game->board[i][j] = val;
            }
            else if (dy == 1) {
                if (game->board[SIZE-1-j][i] != val) changed = true;
                game->board[SIZE-1-j][i] = val;
            }
            else {
                if (game->board[j][i] != val) changed = true;
                game->board[j][i] = val;
            }
        }
    }

    if (changed)
        add_random_tile(game);

    return changed;
}
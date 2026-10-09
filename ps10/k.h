#ifndef K_H
#define K_H

#include <stdbool.h>

#define SIZE 4

struct game {
    char board[SIZE][SIZE];
    int score;
};

bool is_game_won(const struct game game);
bool is_move_possible(const struct game game);
bool update(struct game *game, int dy, int dx);

// эта функция уже есть в костре (НЕ РЕАЛИЗУЙ ЕЁ)
void add_random_tile(struct game *game);

#endif
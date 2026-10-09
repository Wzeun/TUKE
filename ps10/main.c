#include <stdio.h>
#include "k.h"
#include "ui.h"
#include "hof.h"

int main() {
    struct game game = {0};

    for (int i = 0; i < SIZE; i++)
        for (int j = 0; j < SIZE; j++)
            game.board[i][j] = ' ';

    add_random_tile(&game);
    add_random_tile(&game);

    while (is_move_possible(game) && !is_game_won(game)) {
        render(&game);

        char c;
        scanf(" %c", &c);

        if (c == 'w') update(&game, -1, 0);
        if (c == 's') update(&game, 1, 0);
        if (c == 'a') update(&game, 0, -1);
        if (c == 'd') update(&game, 0, 1);
    }

    struct player list[10];
    int size = load(list);
    if (size < 0) size = 0;

    struct player p;
    scanf("%s", p.name);
    p.score = game.score;

    add_player(list, &size, p);
    save(list, size);

    return 0;
}
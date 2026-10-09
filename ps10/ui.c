#include <stdio.h>
#include "ui.h"

void render(struct game *game) {
    printf("\nScore: %d\n", game->score);

    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            printf("| %c ", game->board[i][j]);
        }
        printf("|\n");
    }
}
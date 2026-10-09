#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "game.h"
#include "ui.h"
#include "board.h"

int main() {
    int rows = 0;
    int columns = 0;
    int mines = 0;
    int error = 0;
    char answer[100];

    Game *game = create_game();
    read_player_name(game);

    while (1) {

    // Načítanie herných parametrov
    do {
        error = 0;

        printf("Enter rows (%d-%d): ", MIN_ROW_COUNT, MAX_ROW_COUNT);
        if (scanf("%d", &rows) != 1) {
            printf("Invalid input!\n");
            int i;
            while ((i = getchar()) != '\n' && i != EOF);
            continue;
        }

        printf("Enter columns (%d-%d): ", MIN_COLUMN_COUNT, MAX_COLUMN_COUNT);
        if (scanf("%d", &columns) != 1) {
            printf("Invalid input!\n");
            int i;
            while ((i = getchar()) != '\n' && i != EOF);
            continue;
        }

        printf("Enter mines (1-%d): ", rows * columns - 1);
        if (scanf("%d", &mines) != 1) {
            printf("Invalid input!\n");
            int i;
            while ((i = getchar()) != '\n' && i != EOF);
            continue;
        }

        if (rows == 1 && columns == 1) {
            printf("Board cannot be 1x1!\n");
            error = 1;
        }

    } while (rows < MIN_ROW_COUNT || rows > MAX_ROW_COUNT ||
             columns < MIN_COLUMN_COUNT || columns > MAX_COLUMN_COUNT ||
             mines < 1 || mines > rows * columns - 1 ||
             error == 1);

        game->board = create_board(rows, columns, mines);
        game->game_state = PLAYING;
        game->player->score = 1;
  
        play_game(game);

        printf("Chces hrat znova? (yes/no): ");
        scanf(" %9s", answer);

        // Ak odpoved nie je y, ye, yes ani ich verzia s velkym pismenom
        if (strcmp(answer, "y") != 0 &&
            strcmp(answer, "ye") != 0 &&
            strcmp(answer, "yes") != 0 &&
            strcmp(answer, "Y") != 0 &&
            strcmp(answer, "Ye") != 0 &&
            strcmp(answer, "Yes") != 0 &&
            strcmp(answer, "YE") != 0 &&
            strcmp(answer, "YES") != 0) {
            destroy_game(game);
            break;
        }

        destroy_board(game->board);
        game->board = NULL;
    }

    return 0;
}
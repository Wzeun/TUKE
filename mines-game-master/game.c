#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include "game.h"


/**
 * Change players score.
 * Adds value of opened tile to the score.
 * If game is solved/failed then multiples/divide the score by 2.
 */
void update_player_score(Game *game, int input_row, int input_column) {

    if (game->game_state == FAILED) {
        game->player->score /= 2;
    } else {
        game->player->score +=
                game->board->tiles[input_row][input_column]->value;
    }
    if (game->game_state == SOLVED) {
        game->player->score *= 2;
    }
}


/**
 * Open neighbour Tile if value of the clue is 0.
 * Checks main and mixed directions of the opened Tile.
 */
void open_neighbour_tiles(Game *game, int row, int column) {
    assert(game != NULL);
    // iterates every possible direction of opened Tile
    for (int d_row = -1; d_row <= 1; d_row++) {
        for (int d_column = -1; d_column <= 1; d_column++) {
            int a_row = row + d_row;
            int a_column = column + d_column;
            if (a_row >= 0 && a_row < game->board->row_count && a_column >= 0
                && a_column < game->board->column_count) {
                open_tile(game, a_row, a_column);
            }
        }
    }
}


/**
 * Create and allocate pointers of a Board, Player and Game.
 * Sets Game state to PLAYING.
 * @return pointer to the Game
 */
Game *create_game() {
    Game *game = (Game *) calloc(1, sizeof(Game));
    Player *player = (Player *) calloc(1, sizeof(Player));
    game->player = player;
    game->player->score = 1;
    game->game_state = PLAYING;
    return game;
}


/**
 * Open selected Tile(s) according to input coordinates.
 * Checks if input coordinates are in the correct range if Game state is playing.
 * On first move, places mines ensuring the clicked cell is safe.
 * Can change Game state to solved or failed.
 */
void open_tile(Game *game, int input_row, int input_column) {
    if (game->game_state != PLAYING
        || !is_input_data_correct(game->board, input_row, input_column)) {
        return;
    }

    if (game->board->tiles[input_row][input_column]->tile_state == CLOSED) {
        if (!are_mines_placed(game->board)) {
            place_mines_safe(game->board, input_row, input_column);
        }

        game->board->tiles[input_row][input_column]->tile_state = OPEN;

        if (game->board->tiles[input_row][input_column]->is_mine) {
            game->game_state = FAILED;
            open_all_mines(game->board);
            update_player_score(game, input_row, input_column);
        } else if (game->board->tiles[input_row][input_column]->value == 0) {
            open_neighbour_tiles(game, input_row, input_column);
            if (is_game_solved(game->board)) {
                game->game_state = SOLVED;
            }
            update_player_score(game, input_row, input_column);
        } else {
            if (is_game_solved(game->board)) {
                game->game_state = SOLVED;
            }
            update_player_score(game, input_row, input_column);
        }
    }
}



/**
 * Free pointers of the Player and Game.
 */
void destroy_game(Game *game) {
    assert(game != NULL);
    destroy_board(game->board);
    free(game->player);
    free(game);
}

/**
 * Функция переключает состояние клетки между CLOSED и MARKED.
  * Реализует проверку на максимальное количество флажков.
   */
   void mark_tile(Game *game, int row, int column) {
     assert(game != NULL);

    // Проверка: игра должна идти, а координаты должны быть в пределах поля
    if (game->game_state != PLAYING || !is_input_data_correct(game->board, row, column)) {
        return;
    }

    Tile *tile = game->board->tiles[row][column];

    // Если клетка уже помечена — снимаем пометку
    if (tile->tile_state == MARKED) {
        tile->tile_state = CLOSED;
    }
    // Если клетка закрыта — пытаемся поставить пометку
    else if (tile->tile_state == CLOSED) {
        int marked_count = 0;

        // Считаем, сколько флажков уже стоит на поле
        for (int r = 0; r < game->board->row_count; r++) {
            for (int c = 0; c < game->board->column_count; c++) {
                if (game->board->tiles[r][c]->tile_state == MARKED) {
                    marked_count++;
                }
            }
       }

       // Ставим флажок, только если не превысили лимит мин
       if (marked_count < game->board->mine_count) {
           tile->tile_state = MARKED;
       } else {
           // Сообщение пользователю (интерфейс на словацком)
           printf("Nemôžete označiť viac mín, ako je ich celkový počet (%d)!\n",
                  game->board->mine_count);
        }
    }
}

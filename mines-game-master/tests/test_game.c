#include "greatest.h"
#include "../board.h"
#include "../game.h"

TEST open_mine() {
    Game *game = create_game();
    Board *board = create_board(10, 10, 5);
    game->board = board;
    for (int row = 0; row < game->board->row_count; row++) {
        for (int column = 0; column < game->board->column_count; column++) {
            if(game->board->tiles[row][column]->is_mine){
                open_tile(game, row, column);
            }
        }
    }
    ASSERT_EQ(FAILED, game->game_state);
    destroy_game(game);
    PASS();
}

TEST open_tile_with_bad_coordinates() {
    Game *game = create_game();
    Board *board = create_board(10, 10, 5);
    game->board = board;
    open_tile(game, -1, -1);
    open_tile(game, game->board->row_count, game->board->column_count);

    for (int row = 0; row < game->board->row_count; row++) {
        for (int column = 0; column < game->board->column_count; column++) {
                    ASSERT_EQ(CLOSED, game->board->tiles[row][column]->tile_state);
        }
    }
    destroy_game(game);
    PASS();
}

TEST count_score_after_move() {
    Game *game = create_game();
    Board *board = create_board(10, 10, 70);
    game->board = board;
    bool is_closed_tile_open = false;
    int score = 1;
    while (!is_closed_tile_open) {
        int row = rand() % game->board->row_count;
        int column = rand() % game->board->column_count;
        if (!game->board->tiles[row][column]->is_mine) {
            open_tile(game, row, column);
            score += game->board->tiles[row][column]->value;
            ASSERT_EQ(score, game->player->score);
            is_closed_tile_open = true;
        }
    }

    destroy_game(game);
    PASS();
}

TEST count_score_when_game_failed() {
    Game *game = create_game();
    Board *board = create_board(10, 10, 30);
    game->board = board;
    game->player->score = 30;
    for (int row = 0; row < game->board->row_count; row++) {
        for (int column = 0; column < game->board->column_count; column++) {
            if (game->board->tiles[row][column]->is_mine) {
                open_tile(game, row, column);
                ASSERT_EQ(15, game->player->score);
                break;
            }
        }
    }

    destroy_game(game);
    PASS();
}

TEST create_new_game_after_finish() {
    Game *game = create_game();
    Board *board = create_board(9, 9, 9);
    game->board = board;

    game->game_state = FAILED;

    destroy_game(game);

    Game *new_game = create_game();
    Board *new_board = create_board(9, 9, 9);
    new_game->board = new_board;

    ASSERT(new_game != NULL);
    ASSERT(new_game->board != NULL);
    ASSERT_EQ(PLAYING, new_game->game_state);

    destroy_game(new_game);
    PASS();
}

SUITE (test_game) {
    RUN_TEST(open_mine);
    RUN_TEST(open_tile_with_bad_coordinates);
    RUN_TEST(count_score_after_move);
    RUN_TEST(count_score_when_game_failed);
    RUN_TEST(create_new_game_after_finish);
}

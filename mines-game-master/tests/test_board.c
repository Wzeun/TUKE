#include "greatest.h"
#include "../board.h"
#include "../game.h"

TEST verify_with_high_coordinates() {
    Board *board = create_board(10, 10, 5);
    ASSERT_FALSE(is_input_data_correct(board, 6, 11));
    PASS();
}

TEST verify_with_low_coordinates() {
    Board *board = create_board(10, 10, 5);
    ASSERT_FALSE(is_input_data_correct(board, -1, 4));
    PASS();
}

TEST verify_with_correct_coordinates() {
    Board *board = create_board(10, 10, 5);
    ASSERT(is_input_data_correct(board, 0, 9));
    PASS();
}

TEST is_game_solved_at_the_start() {
    Game *game = create_game();
    Board *board = create_board(10, 10, 5);
    game->board = board;

    bool is_solved = is_game_solved(game->board);
    ASSERT_FALSE(is_solved);
    destroy_game(game);
    PASS();
}

TEST is_game_solved_if_solved() {
    Game *game = create_game();
    Board *board = create_board(10, 10, 5);
    game->board = board;

    for (int row = 0; row < game->board->row_count; row++) {
        for (int column = 0; column < game->board->column_count; column++) {
            if (!game->board->tiles[row][column]->is_mine) {
                open_tile(game, row, column);
            }
        }
    }
    ASSERT(is_game_solved(game->board));
    destroy_game(game);
    PASS();
}

TEST open_mines_after_win() {
    Game *game = create_game();
    Board *board = create_board(10, 10, 5);
    game->board = board;

    for (int row = 0; row < game->board->row_count; row++) {
        for (int column = 0; column < game->board->column_count; column++) {
            if (!game->board->tiles[row][column]->is_mine) {
                open_tile(game, row, column);
            }
        }
    }
    open_all_mines(game->board);
    for (int row = 0; row < game->board->row_count; row++) {
        for (int column = 0; column < game->board->column_count; column++) {
            if (game->board->tiles[row][column]->is_mine) {
                ASSERT_EQ(MARKED, game->board->tiles[row][column]->tile_state);
            }
        }
    }
    destroy_game(game);
    PASS();
}

TEST create_board_valid_input() {
    Board *board = create_board(3, 3, 3);
    ASSERT(board != NULL);
    ASSERT_EQ(3, board->row_count);
    ASSERT_EQ(3, board->column_count);
    ASSERT_EQ(3, board->mine_count);
    destroy_board(board);
    PASS();
}

SUITE (test_board) {
    RUN_TEST(verify_with_high_coordinates);
    RUN_TEST(verify_with_low_coordinates);
    RUN_TEST(verify_with_correct_coordinates);
    RUN_TEST(is_game_solved_at_the_start);
    RUN_TEST(is_game_solved_if_solved);
    RUN_TEST(open_mines_after_win);
    RUN_TEST(create_board_valid_input);
}

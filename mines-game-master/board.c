#include <assert.h>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include "board.h"


/**
 * Check if mine is on the current Tile.
 * @return true if Tile has mine on or false otherwise
 */
bool is_mine_on(Board *board, int row, int column) {
    assert(board != NULL);
    return row >= 0 && row < board->row_count && column >= 0
           && column < board->column_count
           && board->tiles[row][column]->is_mine;
}


/**
 * Count number of mines interacted with Tile.
 * @return count of mines
 */
int count_neighbour_mines(Board *board, int row, int column) {
 assert(board != NULL);

    // iterates with interacting tiles and counts clue value
    int count = 0;
    for (int drow = -1; drow <= 1; drow++) {
        for (int dcolumn = -1; dcolumn <= 1; dcolumn++) {
            if (is_mine_on(board, row + drow, column + dcolumn)) {
                count++;
            }
        }
    }
    return count;
}


/**
 * Set values to tiles according to neighbour mines count.
 * If Tile is a mine then value is set to -1.
 */
void set_tile_values(Board *board) {
    assert(board != NULL);

    for (int row = 0; row < board->row_count; row++) {
        for (int column = 0; column < board->column_count; column++) {

            if (board->tiles[row][column]->is_mine) {
                board->tiles[row][column]->value = -1;
            } else {
                board->tiles[row][column]->value =
                        count_neighbour_mines(board, row, column);
            }
        }
    }
}


/**
 * When Game is won all mines are shown as marked.
 */
void mark_all_mines(Board *board) {
    assert(board != NULL);
    for (int row = 0; row < board->row_count; row++) {
        for (int column = 0; column < board->column_count; column++) {
            if (board->tiles[row][column]->tile_state == CLOSED) {
                board->tiles[row][column]->tile_state = MARKED;
            }
        }
    }
}


/**
 * Generate random value from 0 to upper range of the border.
 * @return random number in range 0 to upper_range param
 */
int generate_random_coordinates(int upper_range) {
    return rand() % upper_range;
}


/**
 * Generate random coordinates to row and column according to mine count value.
 * Randomly sets mines to the Board pointer
 */
void set_mines_randomly(Board *board) {
    assert(board != NULL);

    int board_mine_count = 0;
    while (board_mine_count != board->mine_count) {
        int random_row = generate_random_coordinates(board->row_count);
        int random_column = generate_random_coordinates(board->column_count);

        if (board->tiles[random_row][random_column]->is_mine == false) {

            board->tiles[random_row][random_column]->is_mine = true;
            board_mine_count++;
        }
    }
}


/**
 * Create and allocate pointer of the Board.
 * Mines are not placed until first click (first-click protection).
 * @return pointer of the Board
 */
Board *create_board(int row_count, int column_count, int mine_count) {
    //Correct input check
    assert(row_count > 0 && column_count > 0 && mine_count >= 0);
    assert(row_count <= MAX_ROW_COUNT && column_count <= MAX_COLUMN_COUNT);
    assert(mine_count <= row_count * column_count);

    Board *board = (Board *) calloc(1, sizeof(Board));
    board->row_count = row_count;
    board->column_count = column_count;
    board->mine_count = mine_count;
    board->mines_placed = false;

    //Create empty board
    for (int row = 0; row < board->row_count; row++) {
        for (int column = 0; column < board->column_count; column++) {
            board->tiles[row][column] = (Tile *) calloc(1, sizeof(Tile));
            board->tiles[row][column]->tile_state = CLOSED;
            board->tiles[row][column]->is_mine = false;
            board->tiles[row][column]->value = 0;
        }
    }
    if (mine_count > 0) {
        set_mines_randomly(board);
        set_tile_values(board);
        board->mines_placed = true;
    }

    return board;
}


/**
 * Free the Board and each Tile.
 */
void destroy_board(Board *board) {
    assert(board != NULL);

    for (int row = 0; row < board->row_count; row++) {
        for (int column = 0; column < board->column_count; column++) {
            free(board->tiles[row][column]);
        }
    }
    free(board);
}


/**
 * Check if Game is solved.
 * @return false if Board consists of any Tile which is closed and has clue value, else true
 */
bool is_game_solved(Board *board) {
    assert(board != NULL);
    for (int row = 0; row < board->row_count; row++) {
        for (int column = 0; column < board->column_count; column++) {
            if (board->tiles[row][column]->tile_state == CLOSED
                && !board->tiles[row][column]->is_mine) {
                return false;
            }
        }
    }
    mark_all_mines(board);
    return true;
}


/**
 * Check if input row and column are within correct range.
 * @return true if input coordinates are within the range, false otherwise
 */
bool is_input_data_correct(Board *board, int input_row, int input_column) {
    assert(board != NULL);
    return input_row >= 0 && board->row_count > input_row
           && input_column >= 0 && board->column_count > input_column;
}


/**
 * If Game is lost all mines are shown.
 */
void open_all_mines(Board *board) {
    assert(board != NULL);
    for (int row = 0; row < board->row_count; row++) {
        for (int column = 0; column < board->column_count; column++) {
            if (board->tiles[row][column]->tile_state == CLOSED
                && board->tiles[row][column]->is_mine) {
                board->tiles[row][column]->tile_state = OPEN;
            }
        }
    }
}


/**
 * Check if a cell is in the safe zone (the clicked cell and its neighbors).
 */
static bool is_in_safe_zone(int row, int column, int safe_row, int safe_column) {
    return row >= safe_row - 1 && row <= safe_row + 1
           && column >= safe_column - 1 && column <= safe_column + 1;
}


/**
 * Place mines randomly while ensuring the safe zone is mine-free.
 * The safe zone consists of the clicked cell and all 8 neighbors.
 */

void place_mines_safe(Board *board, int safe_row, int safe_column) {
    assert(board != NULL);
    assert(!board->mines_placed);

    int mine_count = 0;
    while (mine_count != board->mine_count) {
        int random_row = generate_random_coordinates(board->row_count);
        int random_column = generate_random_coordinates(board->column_count);

        if (board->tiles[random_row][random_column]->is_mine == false
            && !is_in_safe_zone(random_row, random_column, safe_row, safe_column)) {
            board->tiles[random_row][random_column]->is_mine = true;
            mine_count++;
        }
    }
    board->mines_placed = true;
    set_tile_values(board);
}




/**
 * Check if mines have been placed on the board.
 * @return true if mines are placed, false otherwise
 */
bool are_mines_placed(Board *board) {
    assert(board != NULL);
    return board->mines_placed;
}

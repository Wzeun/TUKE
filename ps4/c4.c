#include <stdio.h>
#include "c4.h"

#include <stdio.h>

void set_cell(char *cell, char val) {
    *cell = val;
}

void fill_row(char *row, short cols, char val);
void fill_row(char *row, short cols, char val) {
    if (cols <= 0) return;
    set_cell(row, val);
    fill_row(row + 1, cols - 1, val);
}

void fill_board_row(char board[], short cols, short current_col, char val);
void fill_board_row(char board[], short cols, short current_col, char val) {
    if (current_col >= cols) return;
    set_cell(board + current_col, val);
    fill_board_row(board, cols, current_col + 1, val);
}

void fill_board(char board[], short rows, short cols, short current_row, char val);
void fill_board(char board[], short rows, short cols, short current_row, char val) {
    if (current_row >= rows) return;
    fill_board_row(board + current_row * cols, cols, 0, val);
    fill_board(board, rows, cols, current_row + 1, val);
}

void initialize_board(int rows, int cols, char board[rows][cols]) {
    fill_board((char *)board, rows, cols, 0, '.');
}

#include <stdio.h>

int is_in_range(int val, int min, int max) {
    return (val > min - 1 && val < max + 1) ? 1 : 0;
}

int is_cell_empty(char cell) {
    return (cell == '.') ? 1 : 0;
}

int check_column_bounds(int cols, int col) {
    return is_in_range(col, 1, cols);
}

int check_top_cell_empty(int rows, int cols, const char board[rows][cols], int col) {
    return is_cell_empty(board[0][col - 1]);
}

int is_valid_move(int rows, int cols, const char board[rows][cols], int col) {
    int in_bounds = check_column_bounds(cols, col);
    int empty_cell = check_top_cell_empty(rows, cols, board, col);
    return (in_bounds && empty_cell) ? 1 : 0;
}

#include <stdio.h>

int is_valid_move(int rows, int cols, const char board[rows][cols], int col);

int is_out_of_bounds(int index, int limit) {
    return index >= limit ? 1 : 0;
}

int find_drop_row(int rows, int cols, const char board[rows][cols], int col, int current_row) {
    if (is_out_of_bounds(current_row, rows))
        return current_row - 1;
    if (!is_cell_empty(board[current_row][col - 1]))
        return current_row - 1;
    return find_drop_row(rows, cols, board, col, current_row + 1);
}

int place_piece(int rows, int cols, char board[rows][cols], int row, int col, char player_piece) {
    board[row][col] = player_piece;
    return row;
}

int drop_piece(int rows, int cols, char board[rows][cols], int col, char player_piece) {
    if (!is_valid_move(rows, cols, board, col))
        return -1;

    int drop_row = find_drop_row(rows, cols, board, col, 0);
    return place_piece(rows, cols, board, drop_row, col - 1, player_piece);
}

#include <stdio.h>

int is_in_bounds(int val, int limit) {
    return (val >= 0 && val < limit) ? 1 : 0;
}

int check_line_horizontal(const int rows, const int cols, const char board[rows][cols], int row, int col, char player_piece, int count, int index) {
    if (index >= cols) return 0;
    count = (board[row][index] == player_piece) ? count + 1 : 0;
    return (count > 3) ? 1 : check_line_horizontal(rows, cols, board, row, col, player_piece, count, index + 1);
}

int check_line_vertical(const int rows, const int cols, const char board[rows][cols], int row, int col, char player_piece, int count, int index) {
    if (index >= rows) return 0;
    count = (board[index][col] == player_piece) ? count + 1 : 0;
    return (count > 3) ? 1 : check_line_vertical(rows, cols, board, row, col, player_piece, count, index + 1);
}

int check_line_diag1(const int rows, const int cols, const char board[rows][cols], int row, int col, char player_piece, int count, int offset) {
    int r0 = row;
    int c0 = col;
    while (r0 > 0 && c0 > 0) { r0--; c0--; }
    int r = r0 + offset;
    int c = c0 + offset;
    if (!is_in_bounds(r, rows) || !is_in_bounds(c, cols)) return 0;
    count = (board[r][c] == player_piece) ? count + 1 : 0;
    if (count > 3) return 1;
    return check_line_diag1(rows, cols, board, row, col, player_piece, count, offset + 1);
}

int check_line_diag2(const int rows, const int cols, const char board[rows][cols], int row, int col, char player_piece, int count, int offset) {
    int r0 = row;
    int c0 = col;
    while (r0 > 0 && c0 < cols - 1) { r0--; c0++; }
    int r = r0 + offset;
    int c = c0 - offset;
    if (!is_in_bounds(r, rows) || !is_in_bounds(c, cols)) return 0;
    count = (board[r][c] == player_piece) ? count + 1 : 0;
    if (count > 3) return 1;
    return check_line_diag2(rows, cols, board, row, col, player_piece, count, offset + 1);
}

int check_win(int rows, int cols, const char board[rows][cols], int row, int col, char player_piece) {
    return check_line_horizontal(rows, cols, board, row, col, player_piece, 0, 0) ||
           check_line_vertical(rows, cols, board, row, col, player_piece, 0, 0) ||
           check_line_diag1(rows, cols, board, row, col, player_piece, 0, 0) ||
           check_line_diag2(rows, cols, board, row, col, player_piece, 0, 0);
}

int check_cell(const int rows, const int cols, const char board[rows][cols], int r, int c);

int check_row(const int rows, const int cols, const char board[rows][cols], int r, int c) {
    if (c >= cols) return 1;
    return check_cell(rows, cols, board, r, c) ? check_row(rows, cols, board, r, c + 1) : 0;
}

int check_cell(const int rows, const int cols, const char board[rows][cols], int r, int c) {
    return (board[r][c] != '.') ? 1 : 0;
}

int check_board(const int rows, const int cols, const char board[rows][cols], int r) {
    if (r >= rows) return 1;
    return check_row(rows, cols, board, r, 0) ? check_board(rows, cols, board, r + 1) : 0;
}

int is_board_full(int rows, int cols, const char board[rows][cols]) {
    return check_board(rows, cols, board, 0);
}

#include <stdio.h>
#include <stdlib.h>
#include "ballsortpuzzle.h"
#include <time.h>
#include <unistd.h>

#include <stdlib.h>
#include <time.h>

#include <string.h>
#include <time.h>

void generator(int rows, int columns, char field[rows][columns])
{
    srand((unsigned)time(NULL));
    char *wwe = "@+*^#$&!/ASDFGQWERTYNBMK";

    int ddf[2];
    ddf[0] = rand() % columns;
    do
    {
        ddf[1] = rand() % columns;
    } while (ddf[1] == ddf[0]);

    for (int r = 0; r < rows; r++)
    {
        for (int c = 0; c < columns; c++)
        {
            field[r][c] = 32;
        }
    }

    if (columns < 3)
        return;

    int used = columns - 2;
    int wwe_len = (int)strlen(wwe);
    size_t total = (size_t)rows * (size_t)used;

    char *pool = malloc(total ? total : 1);
    if (!pool)
        return;

    for (int s = 0; s < used; ++s)
    {
        char ch = wwe[s % wwe_len];
        for (int r = 0; r < rows; ++r)
        {
            pool[(size_t)s * (size_t)rows + (size_t)r] = ch;
        }
    }

    for (size_t i = total; i > 1; --i)
    {
        size_t j = (size_t)(rand() % (int)i);
        char tmp = pool[i - 1];
        pool[i - 1] = pool[j];
        pool[j] = tmp;
    }

    size_t p = 0;
    for (int r = 0; r < rows; ++r)
    {
        for (int c = 0; c < columns; ++c)
        {
            if (c == ddf[0] || c == ddf[1])
                continue;
            field[r][c] = pool[p++];
        }
    }

    free(pool);
}

void down_possible(int rows, int cols, char f[rows][cols], int x, int y)
{
    if (--x == --y || x < 0 || y < 0)
        return;
    for (int i = 0; i < rows; i++)
    {
        if (f[i][x] != 32)
        {
            char s = f[i][x];
            int j = 0;
            for (; j < rows; j++)
            {
                if (f[j][y] != 32)
                {
                    if (f[j][y] == s && j != 0)
                    {
                        f[j - 1][y] = s;
                        f[i][x] = 32;
                        return;
                    }
                    else
                        return;
                }
            }
            f[rows - 1][y] = s;
            f[i][x] = 32;
            return;
        }
    }
}

#include <stdbool.h>

static bool column_same(const int rows, char col[], char expected)
{
    for (int i = 0; i < rows; i++)
        if (col[i] != expected)
            return false;
    return true;
}

static void extract_column(const int rows, const int columns, int col_index, char field[rows][columns], char out_col[rows])
{
    for (int i = 0; i < rows; i++)
        out_col[i] = field[i][col_index];
}

bool check(const int rows, const int columns, char field[rows][columns])
{
    for (int c = 0; c < columns; c++)
    {
        char column[rows];
        extract_column(rows, columns, c, field, column);
        if (!column_same(rows, column, column[0]))
            return false;
    }
    return true;
}
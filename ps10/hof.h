#ifndef HOF_H
#define HOF_H

#include <stdbool.h>

struct player {
    char name[50];
    int score;
};

int load(struct player list[]);
bool save(struct player list[], int size);
bool add_player(struct player list[], int* size, const struct player player);

#endif
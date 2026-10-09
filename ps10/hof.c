#include <stdio.h>
#include <stdbool.h>
#include "hof.h"

int load(struct player list[]) {
    FILE *f = fopen("score", "r");
    if (!f) return -1;

    int count = 0;
    while (count < 10 && fscanf(f, "%s %d", list[count].name, &list[count].score) == 2)
        count++;

    fclose(f);
    return count;
}

bool save(struct player list[], int size) {
    FILE *f = fopen("score", "w");
    if (!f) return false;

    for (int i = 0; i < size; i++)
        fprintf(f, "%s %d\n", list[i].name, list[i].score);

    fclose(f);
    return true;
}

bool add_player(struct player list[], int* size, const struct player player) {
    int pos = 0;

    while (pos < *size && list[pos].score > player.score)
        pos++;

    if (pos >= 10)
        return false;

    if (*size < 10)
        (*size)++;

    for (int i = *size - 1; i > pos; i--)
        list[i] = list[i - 1];

    list[pos] = player;
    return true;
}
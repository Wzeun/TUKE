#include "hof.h"
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>


/**
 * Load hall of fame from the file
 * @return the number of loaded entries or 0, if there was error in loading
 */
int read_scores(char *filename, Player *list_of_players) {
    assert(filename != NULL);
    assert(list_of_players != NULL);
    FILE *file = fopen(filename, "r");

    if (file == NULL) {
        return 0;
    }
    int index_of_player = 0;
    while (fscanf(file, "%s %d", list_of_players[index_of_player].name,
                  &list_of_players[index_of_player].score) != EOF
           && index_of_player < PLAYERS_LIMIT) {

        index_of_player++;
    }

    fclose(file);
    return index_of_player;
}


/**
 * Save the hall of fame array to the file
 */
void save_scores(char *filename, Player *list_of_players, const int size) {
    assert(filename != NULL);
    assert(list_of_players != NULL);
    FILE *file = fopen(filename, "w");

    if (file == NULL) { return; }
    for (int i = 0; i < size; i++) {
        if (i < PLAYERS_LIMIT) {
            fprintf(file, "%s %d\n", list_of_players[i].name, list_of_players[i].score);
        }
    }
    fclose(file);
}


/**
 * Add Player to the hall of fame array
 * @return true, if Player (entry) was added to the list, false otherwise
 */
bool add_player_to_list(Player list_of_players[], int *size_of_list, const Player player) {
    assert(list_of_players != NULL && size_of_list != NULL);

    if (*size_of_list < PLAYERS_LIMIT) {
        // adds player on the end of the list
        list_of_players[*size_of_list] = player;
        *size_of_list = *size_of_list + 1;
    } else {
        return false;
    }

    return true;
}

#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdlib.h>

#include "ui.h"
#include "view.h"
#include "str_utils.h"

#define HOF_FILENAME "score.txt"

/* private functions */
void game_loop(Game *game);
void print_play_field(Game *game, int input_row, int input_column);
void print_score(Player *list_of_players, int number_of_all_players);
void print_input_rules();


/**
 * Ask a player for his name and store in the game.
 */
void read_player_name(Game *game) {
    assert(game != NULL);
    printf("Zadajte vaše meno prosím: \n");
    char name_of_player[MAX_PLAYER_LENGTH];
    fgets(name_of_player, MAX_PLAYER_LENGTH, stdin);
    str_remove_spaces(name_of_player);
    strcpy(game->player->name, name_of_player);
}


/**
 * Handle whole process of the Game
 */
void play_game(Game *game) {
    assert(game != NULL);

    Player list_of_players[PLAYERS_LIMIT];
    int size_of_list = read_scores(HOF_FILENAME, list_of_players);
    if (size_of_list > 0) {
        print_score(list_of_players, size_of_list);
    }
    game_loop(game);

    if (game->game_state == SOLVED) {
        printf("Gratulujem %s. Riešenie je správne!\n", game->player->name);
    } else {
        printf("Ľutujem %s. Riešenie je nesprávne!\n", game->player->name);
    }
    printf("Vaše skóre je: %d\n", game->player->score);

    bool is_player_added_to_list =
            add_player_to_list(list_of_players, &size_of_list, *game->player);

    if (is_player_added_to_list) {
        save_scores(HOF_FILENAME, list_of_players, size_of_list);
    }
}


/**
 * Handles players input process
 * Loops until game state is playing
 */
//Основной игровой цикл, адаптированный под ввод команд символами.

 void game_loop(Game *game) {
     assert(game != NULL);
     char command; // Хранит символ команды: 'o' (open) или 'm' (mark)
     int row, col;
     int items_read;

     do {
      print_play_field(game, -1, -1);

      // Подсказка пользователю на словацком
      printf("Zadajte príkaz (o r s - otvoriť, m r s - označiť): ");

      // Считываем формат: [символ] [число] [число]
      items_read = scanf(" %c %d %d", &command, &row, &col);

      if (items_read == 3) {
           // Учитываем, что пользователь вводит координаты с 1, а в массиве они с 0
            if (command == 'o') {
              open_tile(game, row - 1, col - 1);
            } else if (command == 'm') {
              mark_tile(game, row - 1, col - 1);
            } else {
              printf("Neplatný príkaz! Použite 'o' alebo 'm'.\n");
       }
    } else {
       // Очистка буфера ввода при некорректном формате (например, если ввели текст)
       scanf("%*[^\n]");
       printf("Chyba: Použite formát 'prikaz riadok stlpec'.\n");
     }

    } while (game->game_state == PLAYING);

    print_play_field(game, -1, -1);
}

void print_score(Player *list_of_players, int number_of_all_players) {
    char *text = view_hof(list_of_players, number_of_all_players);
    printf("%s", text);
    free(text);
}


void print_play_field(Game *game, int input_row, int input_column) {
    char *field = view_play_field(game->board, input_row, input_column);
    printf("\n%s\n", field);
    free(field);
}


void print_input_rules() {
    printf("Zadajte 'o' pre otvorenie alebo 'm' pre oznacenie, riadok a stlpec. Napr. o 2 3, m 5 9\n");
}

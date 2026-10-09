#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <ctype.h>
#include "hangman.h"

#define WORDLIST_FILENAME "words.txt"

int get_word(char secret[]) {
    FILE* fp = fopen(WORDLIST_FILENAME, "rb");
    if (fp == NULL) {
        fprintf(stderr, "No such file or directory: %s\n", WORDLIST_FILENAME);
        return 1;
    }

    struct stat st;
    stat(WORDLIST_FILENAME, &st);
    long int size = st.st_size;

    do {
        long int random = (rand() % size) + 1;
        fseek(fp, random, SEEK_SET);
        int result = fscanf(fp, "%*s %20s", secret);
        if (result != EOF)
            break;
    } while (1);

    fclose(fp);
    return 0;
}

int is_word_guessed(const char secret[], const char letters_guessed[]) {
    for (int i = 0; secret[i] != '\0'; i++) {
        int found = 0;
        for (int j = 0; letters_guessed[j] != '\0'; j++) {
            if (secret[i] == letters_guessed[j]) {
                found = 1;
                break;
            }
        }
        if (!found) return 0;
    }
    return 1;
}

void get_guessed_word(const char secret[], const char letters_guessed[], char guessed_word[]) {
    for (int i = 0; secret[i] != '\0'; i++) {
        int found = 0;
        for (int j = 0; letters_guessed[j] != '\0'; j++) {
            if (secret[i] == letters_guessed[j]) {
                guessed_word[i] = secret[i];
                found = 1;
                break;
            }
        }
        if (!found) guessed_word[i] = '_';
    }
    guessed_word[strlen(secret)] = '\0';
}

void get_available_letters(const char letters_guessed[], char available_letters[]) {
    char alphabet[] = "abcdefghijklmnopqrstuvwxyz";
    int index = 0;
    for (int i = 0; alphabet[i] != '\0'; i++) {
        int already_guessed = 0;
        for (int j = 0; letters_guessed[j] != '\0'; j++) {
            if (alphabet[i] == letters_guessed[j]) {
                already_guessed = 1;
                break;
            }
        }
        if (!already_guessed) {
            available_letters[index++] = alphabet[i];
        }
    }
    available_letters[index] = '\0';
}

void hangman(const char secret[]) {
    char letters_guessed[26] = "";
    char guessed_word[30];
    int guesses_left = 8;

    printf("Welcome to the game, Hangman!\n");
    printf("I am thinking of a word that is %lu letters long.\n", strlen(secret));
    printf("-------------\n");

    while (guesses_left > 0) {
        printf("You have %d guesses left.\n", guesses_left);
        char available_letters[30];
        get_available_letters(letters_guessed, available_letters);
        printf("Available letters: %s\n", available_letters);

        printf("Please guess a letter: ");
        char guess;
        scanf(" %c", &guess);
        guess = tolower(guess);

        if (strchr(letters_guessed, guess) != NULL) {
            printf("Oops! You've already guessed that letter: ");
        }
        else {
            strncat(letters_guessed, &guess, 1);
            if (strchr(secret, guess) != NULL) {
                printf("Good guess: ");
            }
            else {
                printf("Oops! That letter is not in my word: ");
                guesses_left--;
            }
        }

        get_guessed_word(secret, letters_guessed, guessed_word);
        printf("%s\n", guessed_word);
        printf("-------------\n");

        if (is_word_guessed(secret, letters_guessed)) {
            printf("Congratulations, you won!\n");
            return;
        }
    }

    printf("Sorry, you ran out of guesses. The word was %s.\n", secret);
}

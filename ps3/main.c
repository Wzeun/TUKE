#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "hangman.h"
#include "morse.h"

void run_hangman() {
    char secret[16];
    if (get_word(secret) != 0) {
        return;
    }
    hangman(secret);
}

void run_morse_code() {
    char output[150];

    text_to_morse("Hello", output);
    printf("Text to Morse: %s\n", output);

    morse_to_text(".... . .-.. .-.. ---", output);
    printf("Morse to Text: %s\n", output);

    if (is_morse_code_valid(".... . .-.. .-.. ---")) {
        printf("Code is valid!\n");
    }
    else {
        printf("Code is invalid!\n");
    }
}

int main() {
    srand(time(NULL)); 

    printf("Choose an option:\n");
    printf("1. Play Hangman\n");
    printf("2. Test Morse Code functions\n");

    int choice;
    scanf("%d", &choice);

    switch (choice) {
    case 1:
        run_hangman();
        break;
    case 2:
        run_morse_code();
        break;
    default:
        printf("Invalid choice!\n");
        break;
    }

    return 0;
}

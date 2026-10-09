#include <string.h>
#include <ctype.h>
#include "morse.h"

void text_to_morse(const char text[], char output[]) {
    const char* morse_table[36] = {
        ".-", "-...", "-.-.", "-..", ".", "..-.", "--.", "....", "..", ".---",
        "-.-", ".-..", "--", "-.", "---", ".--.", "--.-", ".-.", "...", "-",
        "..-", "...-", ".--", "-..-", "-.--", "--..",
        "-----", ".----", "..---", "...--", "....-", ".....",
        "-....", "--...", "---..", "----."
    };
    const char alphabet[36] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";

    output[0] = '\0';
    for (int i = 0; text[i] != '\0'; i++) {
        if (text[i] == ' ') {
            strcat(output, " ");
        }
        else {
            for (int j = 0; j < 36; j++) {
                if (toupper(text[i]) == alphabet[j]) {
                    strcat(output, morse_table[j]);
                    if (text[i + 1] != '\0' && text[i + 1] != ' ') {
                        strcat(output, " ");
                    }
                    break;
                }
            }
        }
    }
}

void morse_to_text(const char morse[], char output[]) {
    const char* morse_table[36] = {
        ".-", "-...", "-.-.", "-..", ".", "..-.", "--.", "....", "..", ".---",
        "-.-", ".-..", "--", "-.", "---", ".--.", "--.-", ".-.", "...", "-",
        "..-", "...-", ".--", "-..-", "-.--", "--..",
        "-----", ".----", "..---", "...--", "....-", ".....",
        "-....", "--...", "---..", "----."
    };
    const char alphabet[36] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";

    char morse_letter[10];
    int output_index = 0;
    int morse_index = 0;

    for (int i = 0; morse[i] != '\0'; i++) {
        if (morse[i] == ' ') {
            morse_letter[morse_index] = '\0';
            for (int j = 0; j < 36; j++) {
                if (strcmp(morse_letter, morse_table[j]) == 0) {
                    output[output_index++] = alphabet[j];
                    break;
                }
            }
            morse_index = 0;
        }
        else {
            morse_letter[morse_index++] = morse[i];
        }
    }
    morse_letter[morse_index] = '\0';
    for (int j = 0; j < 36; j++) {
        if (strcmp(morse_letter, morse_table[j]) == 0) {
            output[output_index++] = alphabet[j];
            break;
        }
    }
    output[output_index] = '\0';
}

int is_morse_code_valid(const char morse[]) {
    const char* morse_table[36] = {
        ".-", "-...", "-.-.", "-..", ".", "..-.", "--.", "....", "..", ".---",
        "-.-", ".-..", "--", "-.", "---", ".--.", "--.-", ".-.", "...", "-",
        "..-", "...-", ".--", "-..-", "-.--", "--..",
        "-----", ".----", "..---", "...--", "....-", ".....",
        "-....", "--...", "---..", "----."
    };

    char morse_letter[10];
    int morse_index = 0;

    for (int i = 0; morse[i] != '\0'; i++) {
        if (morse[i] != '.' && morse[i] != '-' && morse[i] != ' ') {
            return 0;
        }
        if (morse[i] == ' ' || morse[i + 1] == '\0') {
            if (morse[i] != ' ') morse_letter[morse_index++] = morse[i];
            morse_letter[morse_index] = '\0';
            int valid = 0;
            for (int j = 0; j < 36; j++) {
                if (strcmp(morse_letter, morse_table[j]) == 0) {
                    valid = 1;
                    break;
                }
            }
            if (!valid) return 0;
            morse_index = 0;
        }
        else {
            morse_letter[morse_index++] = morse[i];
        }
    }
    return 1;
}

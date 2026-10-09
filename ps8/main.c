#include <stdio.h>
#include <stdlib.h>
#include "bmp.h"
#include "playfair.h"
// main 
int main() {

    const char *key = "KEY";
    const char *text = "HELLO WORLD";

    unsigned char *bmp_enc = bmp_encrypt(key, text);
    char *bmp_dec = bmp_decrypt(key, bmp_enc);

    printf("BMP encrypted: ");
    for (int i = 0; bmp_enc[i] != '\0'; i++) {
        printf("%02X ", bmp_enc[i]);
    }
    printf("\n");
    
    printf("BMP decrypted: %s\n", bmp_dec);

    free(bmp_enc);
    free(bmp_dec);

    char *pf_enc = playfair_encrypt(key, text);
    char *pf_dec = playfair_decrypt(key, pf_enc);

    printf("Playfair encrypted: %s\n", pf_enc);
    printf("Playfair decrypted: %s\n", pf_dec);

    free(pf_enc);
    free(pf_dec);

    return 0;
}
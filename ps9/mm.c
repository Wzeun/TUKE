#include <stdio.h>

int main() {
    char s[1000001];
    scanf("%s", s);

    int mod = 0;

    for (int i = 0; s[i] != '\0'; i++) {
        mod = (mod + (s[i] - '0')) % 9;
    }

    
    if (s[0] == '0' && s[1] == '\0') {
        printf("0\n");
    } else {
        printf("%d\n", mod == 0 ? 9 : mod);
    }

    return 0;
}
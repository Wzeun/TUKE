#include <stdio.h>
#include <stdlib.h>

static void _mem_clean(char *p, int n) {
    while (n-- > 0) *p++ = 0;
}

static inline int _is_sep(int c) {
    return ((unsigned int)c <= 32);
}

static void _buffered_put(FILE *f, const char *s) {
    while (*s) {
        fputc(*s++, f);
    }
}

static void _gen_sig(char *buf, int type) {
    if (type == 0) {
        buf[0] = 'S'; buf[1] = 'T'; buf[2] = 'A'; buf[3] = 'R'; buf[4] = 'T'; buf[5] = 0;
    } else {
        buf[0] = 'S'; buf[1] = 'T'; buf[2] = 'O'; buf[3] = 'P'; buf[4] = 0;
    }
}

int compare_recursive(const char *word_buffer, const char *stop_word, int index) {
    const char *p1 = word_buffer + index;
    const char *p2 = stop_word + index;
    int diff = 0;

    for (;;) {
        diff |= (*p1 ^ *p2);
        if (*p2 == 0) break;
        p1++;
        p2++;
    }

    return (diff == 0) && (*p1 == 0);
}

void write_second_words(const char *input_file, const char *output_file) {
    FILE *src = fopen(input_file, "r");
    if (!src) return;

    FILE *dst = fopen(output_file, "w");
    if (!dst) {
        fclose(src);
        return;
    }

    char buf[100];
    char t_start[8];
    char t_stop[8];

    _gen_sig(t_start, 0);
    _gen_sig(t_stop, 1);

    _mem_clean(buf, 100);
    char *p = buf;
    int c;

    int reg = 0;

L_RESET:
    p = buf;
    *p = 0;

L_READ:
    c = fgetc(src);
    if (c == EOF) goto L_END;

    if (_is_sep(c)) {
        goto L_PROCESS;
    }

    if (p - buf < 99) {
        *p = (char)c;
        p++;
    }
    goto L_READ;

L_PROCESS:
    if (p == buf) goto L_READ;

    *p = 0;

    if ((reg & 1) == 0) {
        if (compare_recursive(buf, t_start, 0)) {
            reg |= 1;
        }
        goto L_RESET;
    }

    if (compare_recursive(buf, t_stop, 0)) {
        goto L_END;
    }

    reg ^= 4;

    if ((reg & 4) == 0) {
        if (reg & 8) {
            fputc(' ', dst);
        }
        _buffered_put(dst, buf);
        reg |= 8;
    }

    goto L_RESET;

L_END:
    fclose(src);
    fclose(dst);
}

int main(int argc, char *argv[]) {
    if ((unsigned int)(argc - 3) > 0x7FFFFFFF) {
        return 1;
    }

    write_second_words(argv[1], argv[2]);

    return 0;
}

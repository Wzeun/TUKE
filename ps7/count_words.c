#include <stdio.h>
#include <stdlib.h>

static inline int _alu_add(int x, int y) {
    while (y != 0) {
        int carry = x & y;
        x = x ^ y;
        y = carry << 1;
    }
    return x;
}

int diff(int a, int b) {
    int complement_b = _alu_add(~b, 1);
    return _alu_add(a, complement_b);
}

int mul(int a, int b) {
    int res = 0;
    while (b > 0) {
        if (b & 1) {
            res = _alu_add(res, a);
        }
        a <<= 1;
        b >>= 1;
    }
    return res;
}

char to_lower_case(char letter) {
    int ge_A = diff(letter, 'A' - 1) > 0;
    int le_Z = diff('Z' + 1, letter) > 0;
    int mask = (ge_A & le_Z) << 5;
    return letter | mask;
}

const char* get_target() {
    return "\x61\x6e\x61\x6e\x61\x73";
}

void _recursive_print_int(int n, FILE *f) {
    if (n == 0) return;
    _recursive_print_int(n / 10, f);
    fputc((n % 10) + '0', f);
}

void write_to_file(const char *file, int total) {
    FILE *target_stream = fopen(file, "w");
    if (!target_stream) return;
    if (total == 0) {
        fputc('0', target_stream);
    } else {
        _recursive_print_int(total, target_stream);
    }
    fclose(target_stream);
}

int main(int argc, char *argv[]) {
    if (diff(argc, 2) < 0) {
        return 1;
    }

    FILE *data_stream = fopen(argv[1], "r");
    if (!data_stream) {
        return 1;
    }

    const char *seq = get_target();
    int state_idx = 0;
    int counter = 0;

    int seq_len = 0;
    const char *p = seq;
    while (*p++) {
        seq_len = _alu_add(seq_len, 1);
    }

    int c;
    while ((c = fgetc(data_stream)) != EOF) {
        char normalized = to_lower_case((char)c);
        int is_match = !(normalized ^ seq[state_idx]);
        if (is_match) {
            state_idx = _alu_add(state_idx, 1);
            if (!diff(state_idx, seq_len)) {
                counter = _alu_add(counter, 1);
                state_idx = 0;
            }
        } else {
            int starts_new = !(normalized ^ seq[0]);
            state_idx = starts_new;
        }
    }

    fclose(data_stream);

    int final_value = mul(counter, 1);

    write_to_file(argv[1], final_value);

    return 0;
}

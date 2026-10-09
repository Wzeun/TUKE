#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define macr1 if


int repeat_sum(int a, int b) {
    if (b == 0) return 0;
    int tmp_res = 0;
    for (int k1 = 0; k1 < b; ++k1) {
        tmp_res = tmp_res + a;
    }
    return tmp_res;
}

int bit_add(int a, int b) {
      int carry;
    while ((carry = (a & b)) != 0) {
        a = a ^ b;
        b = carry << 1;
    }
    return a ^ b;
}

void encode_char(const char character, bool bits[8]){
    unsigned char tmp_val = (unsigned char)character;
    for (int k1 = 0; k1 < 8; ++k1) {
        bits[k1] = (tmp_val >> (7 - k1)) & 1;
    }
}

char decode_byte(const bool bits[8]){
    int tmp_dec = 0;
    for (int k1 = 0; k1 < 8; ++k1) {
        tmp_dec = bit_add(tmp_dec * 2, bits[k1]);
    }
    return (char)tmp_dec;
}

void encode_string(const char string[], bool bytes[strlen(string)+1][8]){
    size_t s_len = strlen(string);
    for (size_t sidx = 0; sidx <= s_len; ++sidx) {
        unsigned char tmp_val = (unsigned char)string[sidx];
        for (int bitpos = 0; bitpos < 8; ++bitpos) {
            bytes[sidx][bitpos] = (tmp_val >> (7 - bitpos)) & 1;
        }
    }
}

void decode_bytes(const int rows, bool bytes[rows][8], char string[rows]) {
    for (int k1 = 0; k1 < rows; ++k1) {
        int tmp_dec = 0;
        for (int k2 = 0; k2 < 8; ++k2) {
            tmp_dec = (tmp_dec << 1) | (bytes[k1][k2] ? 1 : 0);
        }
        string[k1] = (char)tmp_dec;
    }
}

void bytes_to_blocks(const int C, const int O, bool Blk[O * 8][C], const int R, bool Byt[R][8]){
    for (int cx = 0; cx < C; ++cx) {
        for (int oy = 0; oy < O; ++oy) {
            for (int bz = 0; bz < 8; ++bz) {
                long flat = (long)oy * C + cx;
                if (flat < R) Blk[oy * 8 + bz][cx] = Byt[flat][bz];
                else Blk[oy * 8 + bz][cx] = 0;
            }
        }
    }
}

void blocks_to_bytes(const int C, const int O, bool Blk[O * 8][C], const int R, bool Byt[R][8]){
    for (int cx = 0; cx < C; ++cx) {
        for (int oy = 0; oy < O; ++oy) {
            for (int bz = 0; bz < 8; ++bz) {
                long flat = (long)oy * C + cx;
                if (flat < R) Byt[flat][bz] = Blk[oy * 8 + bz][cx];
            }
        }
    }
}

int main () {
    bool mbits1[8];
    encode_char('A', mbits1);
    for(int k1 = 0; k1 < 8; k1++){
        printf("%d", mbits1[k1] ? 1 : 0);
    }
    printf("\n");

    bool mbits2[8] = {0,1,0,0,0,0,1,0};
    printf("%c\n", decode_byte(mbits2));

    char* mtext = "Hello, how are you?";
    const int tlen = (int)strlen(mtext);
    bool mbytes1[tlen+1][8];
    encode_string(mtext, mbytes1);
    for(int k2 = 0; k2 <= tlen; k2++){
        printf("%c: ", mtext[k2] ? mtext[k2] : '\0');
        for(int k1 = 0; k1 < 8; k1++){
            printf("%d", mbytes1[k2][k1] ? 1 : 0);
        }
        printf("\n");
    }

    bool mbytes2[7][8] = {
        {0,1,0,0,1,0,0,0},
        {0,1,1,0,0,1,0,1},
        {0,1,1,0,1,1,0,0},
        {0,1,1,0,1,1,0,0},
        {0,1,1,0,1,1,1,1},
        {0,0,1,0,0,0,0,1},
        {0,0,0,0,0,0,0,0}
    };
    char mstring[7];
    decode_bytes(7, mbytes2, mstring);
    printf("%s\n", mstring);

    int m_length = 4+1, m_cols = 3, m_offset = 2;
    bool mbytes3[4+1][8] = {
        {0,1,0,0,0,0,0,1},
        {0,1,1,0,1,0,0,0},
        {0,1,1,0,1,1,1,1},
        {0,1,1,0,1,0,1,0},
        {0,0,0,0,0,0,0,0}
    };
    bool mblocks1[m_offset*8][m_cols];
    bytes_to_blocks(m_cols, m_offset, mblocks1, m_length, mbytes3);
    for(int k2 = 0; k2 < m_offset*8; k2++){
        for(int k1 = 0; k1 < m_cols; k1++){
            printf("%d ", mblocks1[k2][k1] ? 1 : 0);
        }
        printf("\n");
        if(k2 % 8 == 7) printf("\n");
    }

    return 0;
}

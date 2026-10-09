#include "bmp.h"
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

unsigned char swap_bits(unsigned char first,int one){
    return ((first & 0b1010)>>one)|((first & 0b0101)<<one);
}

static unsigned char hi4(unsigned char c){
    return c>>4;
}

static unsigned char lo4(unsigned char c){
    return c & 0xF;
}

static unsigned char glue(unsigned char a,unsigned char b){
    return (a<<4)|b;
}

static int rune_valid(const char *k){
    if(!k||!*k) return 0;
    while(*k){
        if(!isalpha((unsigned char)*k)) return 0;
        k++;
    }
    return 1;
}

char* my_strdup(const char* src){
    if(!src) return NULL;
    size_t s=strlen(src)+1;
    char *m=malloc(s);
    if(!m) return NULL;
    return memcpy(m,src,s);
}

static char upperize(char c){
    return (c>='a'&&c<='z')?c^32:c;
}

static void upper_buffer(char *p){
    while(*p){
        *p=upperize(*p);
        p++;
    }
}

static void build_table(char table[26][26]){
    for(int r=0;r<26;r++)
        for(int c=0;c<26;c++)
            table[r][c]='A'+((r+c)%26);
}

void help_func(char* text,char* key,int text_len,int key_len,char* encrypted_text){
    char table[26][26];
    build_table(table);

    int k=0;

    for(int i=0;i<text_len;i++){
        char ch=text[i];

        if(isalpha((unsigned char)ch)){
            int row=toupper(key[k%key_len])-'A';
            int col=toupper(ch)-'A';

            char enc=table[row][col];

            if(islower(ch))
                enc=tolower(enc);

            encrypted_text[i]=enc;

            k++;
        }
        else{
            encrypted_text[i]=ch;
        }
    }
}

void help_func_decrypt(char* text,char* key,int text_len,int key_len,char* decrypted_text){
    char table[26][26];
    build_table(table);

    int k=0;

    for(int i=0;i<text_len;i++){
        char ch=text[i];

        if(isalpha((unsigned char)ch)){
            int row=toupper(key[k%key_len])-'A';
            int col=0;

            char target=toupper(ch);

            for(int j=0;j<26;j++){
                if(table[row][j]==target){
                    col=j;
                    break;
                }
            }

            char dec='A'+col;

            if(islower(ch))
                dec=tolower(dec);

            decrypted_text[i]=dec;

            k++;
        }
        else{
            decrypted_text[i]=ch;
        }
    }
}

char* reverse(const char* text){
    if(!text||!*text) return NULL;

    int n=strlen(text);

    char *buf=malloc(n+1);

    if(!buf) return NULL;

    for(int i=0;i<n;i++)
        buf[i]=upperize(text[n-1-i]);

    buf[n]=0;

    return buf;
}

unsigned char* bit_encrypt(const char* text){
    if(!text||!*text) return NULL;

    size_t L=strlen(text);

    unsigned char *out=malloc(L+1);

    if(!out) return NULL;

    for(size_t i=0;i<L;i++){
        unsigned char h=hi4(text[i]);
        unsigned char l=lo4(text[i]);

        unsigned char m=swap_bits(h,1);

        out[i]=glue(m,m^l);
    }

    out[L]=0;

    return out;
}

char* bit_decrypt(const unsigned char* text){
    if(!text||!*text) return NULL;

    size_t L=strlen((const char*)text);

    unsigned char *out=malloc(L+1);

    if(!out) return NULL;

    for(size_t i=0;i<L;i++){
        unsigned char h=hi4(text[i]);
        unsigned char l=lo4(text[i]);

        unsigned char A=swap_bits(h,1);
        unsigned char B=l^h;

        unsigned char r=glue(A,B);

        if(r<0x20||r>0x7E) r=0x20;

        out[i]=r;
    }

    out[L]=0;

    return (char*)out;
}

char* vigenere_encrypt(const char* key,const char* text){
    if(!rune_valid(key)||!text||!*text)
        return NULL;

    int L=strlen(text);
    int K=strlen(key);

    char *clone=my_strdup(text);

    help_func(clone,(char*)key,L,K,clone);

    upper_buffer(clone);

    return clone;
}

char* vigenere_decrypt(const char* key,const char* text){
    if(!rune_valid(key)||!text||!*text)
        return NULL;

    int L=strlen(text);
    int K=strlen(key);

    char *mem=malloc(L+1);

    if(!mem) return NULL;

    help_func_decrypt((char*)text,(char*)key,L,K,mem);

    mem[L]=0;
    return mem;
}

unsigned char* bmp_encrypt(const char* key,const char* text){
    if(!rune_valid(key)||!text||!*text)
        return NULL;

    char *a=reverse(text);

    char *b=vigenere_encrypt(key,a);
    free(a);

    unsigned char *c=bit_encrypt(b);
    free(b);

    return c;
}

char* bmp_decrypt(const char* key,const unsigned char* text){
    if(!rune_valid(key)||!text)
        return NULL;

    unsigned char *a=(unsigned char*)bit_decrypt(text);

    char *b=vigenere_decrypt(key,(char*)a);
    free(a);

    char *c=reverse(b);

    if(c!=b) free(b);

    return c;
}

#include "playfair.h"
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MATRIX 5

typedef struct{
    char cells[MATRIX*MATRIX];
} MatrixPF;

static int normalize_letter(char c){
    c=toupper(c);
    if(c=='W') c='V';
    return c;
}

static int valid_letter(char c){
    return isalpha((unsigned char)c);
}

static int key_valid(const char *key){
    for(int i=0;key[i];i++){
        if(isalpha((unsigned char)key[i]))
            return 1;
    }
    return 0;
}

static int matrix_index(int r,int c){
    return r*MATRIX+c;
}

static char matrix_get(MatrixPF *m,int r,int c){
    return m->cells[matrix_index(r,c)];
}

static void locate(MatrixPF *m,char target,int *r,int *c){
    for(int i=0;i<25;i++){
        if(m->cells[i]==target){
            *r=i/MATRIX;
            *c=i%MATRIX;
            return;
        }
    }
}

static void key_fill(MatrixPF *m,const char *key){
    int used[26]={0};
    int pos=0;

    for(int i=0;key[i];i++){
        if(!valid_letter(key[i])) continue;
        char ch=normalize_letter(key[i]);
        int id=ch-'A';
        if(!used[id]){
            m->cells[pos++]=ch;
            used[id]=1;
        }
    }

    for(char c='A';c<='Z';c++){
        if(c=='W') continue;
        int id=c-'A';
        if(!used[id]){
            m->cells[pos++]=c;
            used[id]=1;
        }
    }
}

static int clean_text(const char *src,char *dst){
    int n=0;
    for(int i=0;src[i];i++){
        if(src[i]==' ') continue;
        char c=normalize_letter(src[i]);
        if(valid_letter(c))
            dst[n++]=c;
    }

    int i=0;
    while(i<n-1){
        if(dst[i]==dst[i+1]){
            char filler = (dst[i]=='X') ? 'Q' : 'X';
            memmove(dst+i+2,dst+i+1,n-i-1);
            dst[i+1]=filler;
            n++;
        }
        i+=2;
    }

    if(n%2){
        dst[n++]='X';
    }

    return n;
}

static int clean_cipher(const char *src,char *dst){
    int n=0;
    for(int i=0;src[i];i++){
        if(src[i]==' ') continue;
        if(valid_letter(src[i]))
            dst[n++]=toupper(src[i]);
    }
    return n;
}

static void encode_pair(MatrixPF *m,char a,char b,char *out){
    int r1,c1,r2,c2;

    locate(m,a,&r1,&c1);
    locate(m,b,&r2,&c2);

    if(r1==r2){
        c1=(c1+1)%MATRIX;
        c2=(c2+1)%MATRIX;
    }
    else if(c1==c2){
        r1=(r1+1)%MATRIX;
        r2=(r2+1)%MATRIX;
    }
    else{
        int t=c1;
        c1=c2;
        c2=t;
    }

    out[0]=matrix_get(m,r1,c1);
    out[1]=matrix_get(m,r2,c2);
}

static void decode_pair(MatrixPF *m,char a,char b,char *out){
    int r1,c1,r2,c2;

    locate(m,a,&r1,&c1);
    locate(m,b,&r2,&c2);

    if(r1==r2){
        c1=(c1+MATRIX-1)%MATRIX;
        c2=(c2+MATRIX-1)%MATRIX;
    }
    else if(c1==c2){
        r1=(r1+MATRIX-1)%MATRIX;
        r2=(r2+MATRIX-1)%MATRIX;
    }
    else{
        int t=c1;
        c1=c2;
        c2=t;
    }

    out[0]=matrix_get(m,r1,c1);
    out[1]=matrix_get(m,r2,c2);
}

static char* process_encrypt(MatrixPF *m,char *prepared,int len){
    char *result=malloc(len*3+1);
    int p=0;

    for(int i=0;i<len;i+=2){
        encode_pair(m,prepared[i],prepared[i+1],result+p);
        p+=2;

        if(i+2<len)
            result[p++]=' ';
    }

    result[p]='\0';
    return result;
}

static char* process_decrypt(MatrixPF *m,char *prepared,int len){
    char *result=malloc(len+1);
    int p=0;

    for(int i=0;i<len;i+=2){
        decode_pair(m,prepared[i],prepared[i+1],result+p);
        p+=2;
    }

    result[p]='\0';
    return result;
}

char* playfair_encrypt(const char* key,const char* text){
    if(!key||!text||!*text||!key_valid(key)) return NULL;

    MatrixPF matrix;
    key_fill(&matrix,key);

    char buffer[512];
    int length=clean_text(text,buffer);

    return process_encrypt(&matrix,buffer,length);
}

char* playfair_decrypt(const char* key,const char* text){
    if(!key||!text||!*text||!key_valid(key)) return NULL;

    MatrixPF matrix;
    key_fill(&matrix,key);

    char buffer[512];
    int length=clean_cipher(text,buffer);

return process_decrypt(&matrix,buffer,length);
}
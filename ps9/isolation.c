#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Node {
    int val;
    struct Node *left, *right;
} Node;


Node* newNode(int v) {
    Node* n = (Node*)malloc(sizeof(Node));
    n->val = v;
    n->left = n->right = NULL;
    return n;
}

Node* insert(Node* root, int v) {
    if (!root) return newNode(v);

    if (v < root->val)
        root->left = insert(root->left, v);
    else
        root->right = insert(root->right, v);

    return root;
}


void encode(Node* root, char* res, int* pos) {
    if (!root) {
        res[(*pos)++] = '.';
        return;
    }

    res[(*pos)++] = 'X';
    encode(root->left, res, pos);
    encode(root->right, res, pos);
}


void freeTree(Node* root) {
    if (!root) return;
    freeTree(root->left);
    freeTree(root->right);
    free(root);
}

int main() {
    int n, k;
    scanf("%d %d", &n, &k);

    char shapes[50][200]; 
    int unique = 0;

    for (int i = 0; i < n; i++) {
        Node* root = NULL;

        for (int j = 0; j < k; j++) {
            int x;
            scanf("%d", &x);
            root = insert(root, x);
        }

        char shape[200];
        int pos = 0;
        encode(root, shape, &pos);
        shape[pos] = '\0';

        int found = 0;
        for (int j = 0; j < unique; j++) {
            if (strcmp(shape, shapes[j]) == 0) {
                found = 1;
                break;
            }
        }

        if (!found) {
            strcpy(shapes[unique++], shape);
        }

        freeTree(root);
    }

    printf("%d\n", unique);
    return 0;
}
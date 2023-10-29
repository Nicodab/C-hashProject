#ifndef LOOKUP_H
#define LOOKUP_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/sha.h>

// Déclaration des constantes
#define HASH_LENGTH (SHA256_DIGEST_LENGTH * 2) + 1 // REVOIR POUR LE +1, peut etre ne pas le faire --> +1) //Fallait rajouter un +1 pcq apparement ça prenait un octet en plus après hasher avec du sha256
#define MAX_LINE_LENGTH 1024

// Structure de l'arbre binaire
typedef struct BinaryTree {
    char* str;
    char* hash;
    struct BinaryTree* left;
    struct BinaryTree* right;
} BinaryTree;

// Fonctions pour gérer l'arbre binaire
BinaryTree* insert(BinaryTree* root, const char* str, const char* hash);
const char* find(const BinaryTree* root, const char* targetHash);
void freeTree(BinaryTree* root);
BinaryTree* createNode(const char* str, const char* hash);
BinaryTree *sortBinaryTree(BinaryTree *tree);
BinaryTree* buildBalancedTree(char** condensats, char** chaines, int start, int end);
void storeInorder(BinaryTree *tree, char **condensats, char **chaines, int *index);
int countNodes(BinaryTree *tree);
void lookupString(const BinaryTree* tree, const char* hash);
BinaryTree* loadTable(const char* filename);
#endif

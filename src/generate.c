#include <stdio.h>
#include <string.h>
#include <openssl/sha.h>
#include <openssl/md5.h>
#include <stdlib.h>
#include "generate.h"

#define MAX_LINE_LENGTH 1024 //Taille maximale que peut avoir une ligne dans le fichier d'entrée (dico)
#define HASH_LENGTH (SHA256_DIGEST_LENGTH * 2) //Longueur de la chaîne de caractère en hexadécimal (SHA_256_DiGEST_LENGTH est de 32 octets) --> Donc si on le représente en hexa ça sera avec 2 caractère par octets, d'où le *2
// La longueur du hachage MD5 (en caractères hexadécimaux)
#define MD5_DIGEST_LENGTH 32

void computeSHA256(const char *input, char *output) {
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256_CTX sha256; // Déclaration de la structure de contexte --> Utilisé pour calculer le hash 
    SHA256_Init(&sha256); // Initialisation de la structure de contexte
    SHA256_Update(&sha256, input, strlen(input)); // maj du contexte sha256 par le calcul du hash de input
    int sha_finale = SHA256_Final(hash, &sha256); // stockage du résultat dans le tableau 'hash' déclaré plus haut

    for (int i = 0; i < SHA256_DIGEST_LENGTH; i++) {
        // On place dans output la représentation hexa de chaque octet contenu dans hash (après le calcul de hash)
        // output + (i*2) signifie que l'on déplace la position d'ajout des carctères de la chaînes hash.
        // Ici on place chaque octet lu dans output, sauf que chacun de ces octets sont représenter en hexa (donc on ajoute 2 caractère à chaque fois)
        sprintf(output + (i * 2), "%02x", hash[i]);
    }
    output[HASH_LENGTH] = '\0'; //caractère nul pour la fin de la chaîne.
}

void computeMD5(const char *input, char *output) {
    unsigned char digest[MD5_DIGEST_LENGTH];
    MD5_CTX md5; // Déclaration de la structure de contexte --> Utilisée pour calculer le hachage MD5

    MD5_Init(&md5); // Initialisation de la structure de contexte
    MD5_Update(&md5, input, strlen(input)); // Mise à jour du contexte MD5 par le calcul du hachage de l'entrée
    int md5_final = MD5_Final(digest, &md5); // Stockage du résultat dans le tableau 'digest' déclaré plus haut

    for (int i = 0; i < MD5_DIGEST_LENGTH; i++) {
        sprintf(output + (i * 2), "%02x", digest[i]);
    }
    output[MD5_DIGEST_LENGTH] = '\0'; // Caractère nul pour la fin de la chaîne.
    printf("md5 hash: %s", output);
}

bool generateTable(const char *inputFileName, const char *outputFileName, const char* algo) {
    printf("input file: %s\n",inputFileName);
    printf("output file: %s\n",outputFileName);

    FILE *inputFile = fopen(inputFileName, "r");
    FILE *outputFile = fopen(outputFileName, "w");

    if (!inputFile || !outputFile) {
        perror("Error opening files");
        return false;
    }

    char line[MAX_LINE_LENGTH];
    char hash[HASH_LENGTH + 1];
    CorrespondenceEntry entry;

    long int nbHashWritten = 1; //Nb de hash écris dans une T3C (à afficher tous les *10)
    int compteur=0; // Compteur incrémenté à chaque tour de boucle

    while (fgets(line, sizeof(line), inputFile)) {
        line[strcspn(line, "\n")] = '\0'; //Supprime le caractère de saut de ligne à la fin de chaque ligne lue

        entry.string = strdup(line); // Place la chaîne en claire lue dans entry.string 
        if (strcmp(algo, "sha256") == 0)
            computeSHA256(line, hash);
        else if (strcmp(algo, "md5") == 0)
            computeMD5(line, hash);
        entry.hash = strdup(hash); // Place le hash calculé dans entry.hash 

        fprintf(outputFile, "%s:%s\n", entry.string, entry.hash);

        free(entry.string);
        free(entry.hash);
        compteur++;
        if (compteur == (nbHashWritten*10)){
            nbHashWritten *= 10;
            fprintf("INFO: %ld HASH WRITTEN...\n", nbHashWritten);
            printf("INFO: %ld HASH WRITTEN...\n", nbHashWritten);
        }
    }

    fclose(inputFile);
    fclose(outputFile);

    return true;
}

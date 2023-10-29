#include <stdio.h>
#include <string.h>
#include "generate.h"
#include "lookup.h"
#include <time.h>

void exitFunction(char *hash);

int main(int argc, char *argv[]) {

    bool balanceTheTree = false;
    char *mode = argv[1]; // Mode 'G' ou 'L'
    // Generation de table de correspondance
    if (strcmp(mode, "-G") == 0 && (strcmp(argv[3], "-o") == 0)) {
        if (argc < 5) {
            fprintf(stderr, "Usage: %s -G <intput_file> -o <output_file> ou %s -L <input_file>\n", argv[0], argv[0]);
            return 1;
        }
        const char *inputFileName = argv[2];// POur linstant on teste avec un plsu petit fichier --> on remet ça plus tard "/usr/share/wordlists/rockyou.txt";
        
        const char *outputFileName = argv[4];
        const char *hashAlgorithm = "";
        // Si le 5eme element du tableau d'arg du mode est "--algo"
        // --> On prend le 6ème elem du tableau qui donne l'algo de hash voulu
        if ((argc == 7) && (strcmp(argv[5], "--algo") == 0)) hashAlgorithm = argv[6]; // --algo 
        if (!generateTable(inputFileName, outputFileName, hashAlgorithm)) {
            fprintf(stderr, "Erreur lors de la génération de la table.\n");
            return 1;
        }
        else{
            printf("'%s' a été généré\n", outputFileName);
            return 0;
        }
    }
    else if (strcmp(mode, "-L") == 0) {
        if (argc < 3) {
            fprintf(stderr, "Usage: %s -G <intput_file> -o <output_file> ou %s -L <input_file>\n", argv[0], argv[0]);
            return 1;
        }
        printf("Chargement de la table de correspondance...\n");
        // initialisation des variables pour chronométrer la création de l'arbre binaire
        clock_t debut, fin;
        double temps;
        //Heure de début
        debut = clock();

        BinaryTree* tree = loadTable(argv[2]); // A CHANGER pour le chemin spécifié par le user
        if (tree == NULL) {
            fprintf(stderr, "Erreur d'allocation de mémoire pour l'arbre binaire\n");
            printf("Erreur d'allocation de mémoire pour l'arbre binaire\n");
            exit(EXIT_FAILURE);
        }
        // Heure de fin
        fin = clock();
        // Calcul du temps écoulé en secondes
        temps = (double)(fin - debut) / CLOCKS_PER_SEC;

        printf("> Temps de chargement: %f secondes\n", temps);
        BinaryTree* balancedTree;
        printf("Table de correspondance chargée en mémoire\n");
        
        //Si le user veut équilibrer l'arbre
        if ((argc == 4) && (strcmp(argv[3], "--balanced") == 0)){
            printf("Triage de l'arbre...\n");
            balanceTheTree = true;
            // On trie l'arbre en faisant un balancedTree
            balancedTree = sortBinaryTree(tree);
            //Libérez la mémoire de l'arbre non équilibré pour économiser de la mémoire
            freeTree(tree);
            printf("Arbre équilibré crée.\n");
            if (balancedTree == NULL) {
                fprintf(stderr, "Erreur d'allocation de mémoire pour le Balanced tree\n");
                exit(EXIT_FAILURE);
            }
        }

        char hash[HASH_LENGTH];

        printf("Entrez les condensats (séparés par des sauts de ligne) :\n");
        while (fgets(hash, HASH_LENGTH, stdin) != NULL) {
            
            // Si c'est pas un saut de ligne
            if (strcmp(hash, "\n") != 0){
                hash[strcspn(hash, "\n")] = '\0';
                if (!balanceTheTree) lookupString(tree, hash); // Affiche ds la console le mot en claire du condensat
                else lookupString(balancedTree, hash);
            }
            // Si c'est un \n
            else{
                for (int i = 0; i < HASH_LENGTH; i++){
                    hash[i] = "";// On remet à vide le string pour être sur que y'a plus de \n ou \0 dedans
                }
            }
        }
        // Libérer la mémoire
        freeTree(tree);
        freeTree(balancedTree);
    } else {
        fprintf(stderr, "Mode non reconnu. Utilisation : %s -G <input_file> -o <output_file> ou %s -L\n", argv[0], argv[0]);
        return 1;
    }

    return 0;
}

void exitFunction(char *hash){
    printf("4 premieres lettres: %c%c%c%c\n",hash[0],hash[1], hash[2], hash[3]);
    if ((strcmp(hash[0],"e") == 0) && (strcmp(hash[1], "x") == 0) && (strcmp(hash[2], "i") == 0) && (strcmp(hash[3], "t") == 0))
        exit(EXIT_FAILURE);
}
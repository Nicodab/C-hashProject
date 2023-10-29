#include <stdio.h>
#include <string.h>
#include "generate.h"
#include "lookup.h"
#include <time.h>

void exitFunction(char *hash);

int main(int argc, char *argv[]) {
    printf("nb arg: %d\n",argc);
    printf("-------------------------\n");
    bool balanceTheTree = false;
    char *mode = argv[1]; // Mode 'G' ou 'L'
    printf("mode : %s\n", mode);
    // Generation de table de correspondance
    if (strcmp(mode, "-G") == 0 && (strcmp(argv[3], "-o") == 0)) {
        // Sur un environnement Linux, on peut mettre le chemin absolue  
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
            fprintf(stderr, "Error generating the table.\n");
            return 1;
        }
        else{
            printf("File '%s' was successfully generated\n", outputFileName);
            return 0;
        }
    }
    // APPAREMENT Y'a aucune correspondance trouvés ????? quand je met le bon hash pourtant  
    else if (strcmp(mode, "-L") == 0) {
        if (argc < 3) {
            fprintf(stderr, "Usage: %s -G <intput_file> -o <output_file> ou %s -L <input_file>\n", argv[0], argv[0]);
            return 1;
        }
        printf("Before loading the table\n");
        // Mode "lookup", traitez le cas ici
        clock_t debut, fin;
        double temps;

        // Enregistrez l'heure de début
        debut = clock();

        BinaryTree* tree = loadTable(argv[2]); // A CHANGER pour le chemin spécifié par le user
        if (tree == NULL) {
            fprintf(stderr, "Erreur d'allocation de mémoire pour l'arbre binaire\n");
            printf("Erreur d'allocation de mémoire pour l'arbre binaire\n");
            exit(EXIT_FAILURE);
        }
        // Enregistrez l'heure de fin
        fin = clock();
        // Calculez le temps écoulé en secondes
        temps = (double)(fin - debut) / CLOCKS_PER_SEC;

        printf("Temps de chargement de la T3C : %f secondes\n", temps);
        BinaryTree* balancedTree;
        printf("After loading the table\n");
        //Si le user veut équilibrer l'arbre
        //printf("nb arg: %d\n", strlen(argv));
        //if (argc == 4) printf("argv[3]: %s\n", argv[3]);

        if ((argc == 4) && (strcmp(argv[3], "--balanced") == 0)){
            printf("Sorting de l'arbre...\n");
            balanceTheTree = true;
            // On trie l'arbre en faisant un balancedTree
            balancedTree = sortBinaryTree(tree); //A DECOMMENTER :!!!!!!!!!!!!!!!!!!!!
            //Libérez la mémoire de l'arbre non équilibré
            freeTree(tree);
            printf("Balanced tree created.\n");
            if (balancedTree == NULL) {
                fprintf(stderr, "Erreur d'allocation de mémoire pour le Balanced tree\n");
                printf("Erreur d'allocation de mémoire pour le Balanced tree\n");
                exit(EXIT_FAILURE);
            }
        }

        char hash[HASH_LENGTH];

        printf("Entrez les condensats (séparés par des sauts de ligne) :\n");
        while (fgets(hash, HASH_LENGTH, stdin) != NULL) {
            //setbuf(stdin, NULL);
            //fflush(stdin);
            // Si c'est pas un saut de ligne

            if (strcmp(hash, "\n") != 0){
                //printf("Before exit condition\n");
                //exitFunction(hash); // Quitte ke programme si c'est exit MAIS j'aimerais éviter le "segmentation fault (core dumped)"
                //printf("After exit condition\n");
                //printf("HASH_LENGTH: %d\n", HASH_LENGTH);
                //printf("Hash avant strcspn: %s\n", hash);
                //printf("63e du hash recherché: %c\n", hash[63]);
                //printf("Taille du hash avant toutes actions: %ld\n", strlen(hash));
                // Supprimer le caractère de saut de ligne
                //printf("strcspn index: %d\n", strcspn(hash, "\n"));
                hash[strcspn(hash, "\n")] = '\0';
                //printf("Hash après strcspn: %s\n", hash);
                //printf("Taille du hash après strcspn et avant lookupString: %ld\n", strlen(hash));
                //lookupString(tree, hash);
                if (!balanceTheTree) lookupString(tree, hash); // Affiche ds la console le mot en claire du condensat
                else lookupString(balancedTree, hash);
            }
            else{
                for (int i = 0; i < HASH_LENGTH; i++){
                    hash[i] = "";// On remet à vide le string pour être sur que y'a plus de \n ou \0 dedans
                }
            }
        }
        // Libérer la mémoire de l'arbre balancedTree
        freeTree(tree);
        freeTree(balancedTree); // A DECOMMENTER
    } else {
        fprintf(stderr, "Mode non reconnu. Utilisation : %s -G -o <output_file> ou %s -L\n", argv[0], argv[0]);
        return 1;
    }

    return 0;
}

void exitFunction(char *hash){
    printf("4 premieres lettres: %c%c%c%c\n",hash[0],hash[1], hash[2], hash[3]);
    if ((strcmp(hash[0],"e") == 0) && (strcmp(hash[1], "x") == 0) && (strcmp(hash[2], "i") == 0) && (strcmp(hash[3], "t") == 0))
        exit(EXIT_FAILURE);
}
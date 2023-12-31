// Dans le fichier lookup.c
#include "lookup.h"
#include <time.h>

BinaryTree* loadTable(const char* filename) {
    // Ouvrir le fichier en lecture
    FILE* file = fopen(filename, "r");
    printf("Fichier %s ouvert en lecture\n", filename);
    if (file == NULL) {
        fprintf(stderr, "Erreur : impossible d'ouvrir le fichier de table.\n");
        exit(1);
    }

    BinaryTree* tree = NULL; // Initialisation de l'arbre binaire

    char line[MAX_LINE_LENGTH];

    printf("Lecture du fichier %s...\n", filename);
    // Parcourir le fichier ligne par ligne
    while (fgets(line, MAX_LINE_LENGTH, file) != NULL) {
        // Insérer la paire chaîne-condensat dans l'arbre
        char *dernierSeparateur = strrchr(line, ':');

        if (dernierSeparateur != NULL) {
            // Calculer la position du dernier ':' dans la chaîne
            int positionDernierSeparateur = dernierSeparateur - line;

            // Diviser la chaîne en deux parties
            line[positionDernierSeparateur] = '\0'; // Séparer la chaîne au dernier ':'
            char *str = line;
            char *hash = dernierSeparateur + 1;
            if (hash[strlen(hash)-1] == '\n') hash[strlen(hash)-1] = '\0';
            tree = insert(tree, str, hash);
        } else {
            // Si aucun ':' n'a été trouvé, faire quelque chose en conséquence
            printf("Erreur: Aucun separateur ':' n'a été trouvé dans ligne '%s'.\n", line);
        }
    }
    printf("Fermeture du fichier %s\n", filename);
    fclose(file);
    return tree;
}

// Recherche du condensat dans l'arbre binaire
void lookupString(const BinaryTree* tree, const char* hash) {
    // Variables pour mesurer le temps
    clock_t debut, fin;
    double temps;
    // Heure de début
    debut = clock();
    const char* result = find(tree, hash);
    // Heure de fin
    fin = clock();
    // Calcul du temps écoulé en secondes
    temps = (double)(fin - debut) / CLOCKS_PER_SEC;

    printf("Temps de recherche: %f secondes\n", temps);

    if (result != NULL) {
        printf("Chaîne correspondante : %s\n", result);
    } else {
        printf("Aucune correspondance trouvée.\n");
    }
}

// Fonction récursive pour trouver un condensat dans l'arbre binaire
const char* find(const BinaryTree* root, const char* targetHash) {
    if (root == NULL) {
        return NULL; // L'arbre est vide, le condensat n'existe pas
    }
    
    int comparison = strcmp(targetHash, root->hash);
    if (comparison == 0) {
        return root->str; // Le condensat a été trouvé, renvoie la chaîne associée
    } else if (comparison < 0) {
        return find(root->left, targetHash); // Recherche à gauche
    } else {
        return find(root->right, targetHash); // Recherche à droite
    }
}

//Insertion à gauche des hash les + faibles et à droite de ceux les plus forts.
// Comme ça on peut le parcourir ensuite + simplement de gauche à droite pour avoir les hash triés dans l'ordre
BinaryTree* insert(BinaryTree* root, const char* str, const char* hash) {
    // Si l'arbre est vide, on créer un nouveau noeud on le retourne comme nouvelle racine
    if (root == NULL) {
        BinaryTree* newNode = (BinaryTree*)malloc(sizeof(BinaryTree));
        newNode->str = strdup(str);
        newNode->hash = strdup(hash);
        newNode->hash[64] = '\0'; // On remet la taille à 63 octets pour que le strcmp se fasse correctement avec les hash lues en stdin.
        newNode->left = NULL;
        newNode->right = NULL;
        return newNode;
    }

    // Sinon, récursivement on insère dans le sous-arbre gauche ou droit
    if (strcmp(hash, root->hash) < 0) {
        root->left = insert(root->left, str, hash);
    } else if (strcmp(hash, root->hash) > 0) {
        root->right = insert(root->right, str, hash);
    }

    // retournez le noeud (inchangé) s'il existe déjà
    return root;
}

// Compter le nombre de noeuds dans l'arbre.
int countNodes(BinaryTree *tree) {
    if (tree == NULL) {
        return 0;
    }
    return 1 + countNodes(tree->left) + countNodes(tree->right);
}

// Fonction pour stocker les condensats dans un tableau trié.
void storeInorder(BinaryTree *tree, char **condensats, char **chaines, int *index) {
    if (tree == NULL) {
        return;
    }
    
    // Parcours d'abord le sous-arbre gauche.
    storeInorder(tree->left, condensats, chaines, index);
    
    // Stocke le condensat dans le tableau qui sera triés
    condensats[*index] = tree->hash;
    chaines[*index] = tree->str;
    (*index)++;
    
    // Parcours ensuite le sous-arbre droit.
    storeInorder(tree->right, condensats, chaines, index);
}
// Création de noeud pour les insérer dans l'arbre binaire
BinaryTree* createNode(const char* str, const char* hash) {
    BinaryTree* newNode = (BinaryTree*)malloc(sizeof(BinaryTree));
    if (newNode == NULL) {
        fprintf(stderr, "Erreur d'allocation de mémoire\n");
        exit(EXIT_FAILURE);
    }
    newNode->str = strdup(str);
    newNode->hash = strdup(hash);
    newNode->left = NULL;
    newNode->right = NULL;
    return newNode;
}


// Fonction de construction de arbre binaire équilibré (trié à partir d'un tableau trié).
BinaryTree* buildBalancedTree(char** condensats, char** chaines, int start, int end) {
    if (start > end) {
        return NULL;
    }
    // On prend l'index à la moitié entre 0 et la taille de condensats - 1
    int middle = (start + end) / 2;
    
    // Création d'un nouveau noeud avec le condensat au milieu.
    BinaryTree* root = createNode(chaines[middle], condensats[middle]);
    
    // Construit le sous-arbre gauche à partir de la première moitié du tableau.
    root->left = buildBalancedTree(condensats, chaines, start, middle - 1);
    
    // Construit le sous-arbre droit à partir de la deuxième moitié du tableau.
    root->right = buildBalancedTree(condensats, chaines, middle + 1, end);
    
    return root;
}


// Fonction pour trier un arbre binaire.
BinaryTree *sortBinaryTree(BinaryTree *tree) {
    int nodeCount = countNodes(tree);
    printf("Nombre de noeuds: %d\n", nodeCount);
    char **condensats = malloc(nodeCount * sizeof(char *));
    char **chaines = malloc(nodeCount * sizeof(char *));
    int index = 0;
    
    // Stocke les condensats dans le tableau trié.
    storeInorder(tree, condensats, chaines, &index);
    
    // Reconstruit l'arbre à partir du tableau trié.
    BinaryTree *sortedTree = buildBalancedTree(condensats, chaines, 0, nodeCount - 1);
    
    free(condensats);
    
    return sortedTree;
}

// Free tous l'arbre binaire généré (depuis la lecture de T3C) pour la recherche d'un hash de mdp
void freeTree(BinaryTree* root) {
    if (root == NULL) {
        return;
    }
    freeTree(root->left);
    freeTree(root->right);
    free(root->str);
    free(root->hash);
    free(root);
}



#ifndef GENERATE_H
#define GENERATE_H

#include <stdbool.h> //"include squiggles" de vscode a permis de régler cette erreur

typedef struct {
    char *string;
    char *hash;
} CorrespondenceEntry;

bool generateTable(const char *inputFileName, const char *outputFileName, const char* algo);

#endif

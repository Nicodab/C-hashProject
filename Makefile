# Nom du programme de sortie
TARGET = finalproject

# Répertoire source
SRC_DIR = src

# Fichiers source
SRCS = $(SRC_DIR)/main.c $(SRC_DIR)/generate.c $(SRC_DIR)/lookup.c

# Fichiers d'en-tête
HEADERS = $(SRC_DIR)/lookup.h $(SRC_DIR)/generate.h

# Compilateur
CC = gcc

# Options de compilation
CFLAGS = -I$(SRC_DIR)

# Options d'édition de liens.
LDFLAGS = -lssl -lcrypto

# Règle par défaut : générer le programme cible
all: $(TARGET)

# Règle pour générer le programme cible
$(TARGET): $(SRCS) $(HEADERS)
	$(CC) $(CFLAGS) -o $@ $(SRCS) $(LDFLAGS)

# Nettoyage : supprimer le programme cible
clean:
	rm -f $(TARGET)

.PHONY: all clean

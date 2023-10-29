# Utiliser la dernière image Ubuntu dispo
FROM ubuntu:latest

# Mise à jour du système et installation des dépendances
RUN apt-get update && apt-get install -y \
    gcc \
    libssl-dev \
    wget \
    valgrind \
    make

# Téléchargement du fichier rockyou.txt dans le répertoire courant
RUN wget -O rockyou.txt https://github.com/brannondorsey/naive-hashcat/releases/download/data/rockyou.txt

# Copie du dossier src et du fichier Makefile dans le conteneur
COPY src /app/src
COPY Makefile /app/Makefile

# Définition du répertoire de travail
WORKDIR /app

# Compilation du projet à l'aide du Makefile
RUN make
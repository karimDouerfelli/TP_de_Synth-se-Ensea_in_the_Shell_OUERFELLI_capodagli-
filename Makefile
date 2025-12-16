########Configuration de la compilation#######

# Nom du programme final (l'exécutable qui sera créé)
TARGET = enseaShell

# Choix du compilateur (GCC est le standard sous Linux)
CC = gcc

# Options de compilation (Flags) :
# -Wall -Wextra : Active tous les avertissements. Indispensable pour avoir un code "propre" (Clean Code)
# -g            : Ajoute les infos de débogage (utile pour gdb ou valgrind)
# -ICore/Inc    : Indique au compilateur de chercher les fichiers .h dans le dossier Core/Inc
#                 Cela permet de faire #include "enseash.h" proprement.
CFLAGS = -Wall -Wextra -g -ICore/Inc
# Dossiers
SRC_DIR = Core/Src
OBJ_DIR = Obj

# Liste des fichiers sources (trouve tous les .c dans Core/Src)
SRCS = $(wildcard $(SRC_DIR)/*.c)
# Transforme les .c en .o (dans le dossier Obj pour rester propre)
OBJS = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRCS))

# Règle par défaut
all: $(TARGET)

# Link de l'exécutable
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^
	@echo "Compiled Successfully  ! Run ./$(TARGET)"

# Compilation des fichiers objets
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Création du dossier Obj s'il n'existe pas
$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

# Nettoyage
clean:
	rm -rf $(OBJ_DIR) $(TARGET)

.PHONY: all clean
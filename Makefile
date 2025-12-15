# Nom de l'exécutable final
TARGET = enseash

# Compilateur et options (flags)
CC = gcc
# -I permet d'inclure les headers dans Core/Inc sans mettre le chemin complet dans le .c
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
	@echo "Compilation réussie ! Lancez ./$(TARGET)"

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
#								#
#Compilation configuration #
#								#

# Name of the programme(our shell )
TARGET = enseash

# Choice of the compilateur (GCC because we have the Linux)
CC = gcc

# Compilation options (Flags):
# -Wall -Wextra: Enables all warnings. Essential for having "clean" code.
# -g: Adds debugging information (useful for gdb or valgrind)
# -ICore/Inc: Tells the compiler to look for .h files in the Core/Inc folder
# This allows you to do #include "enseash.h" properly.
CFLAGS = -Wall -Wextra -g -ICore/Inc
# Files
SRC_DIR = Core/Src
OBJ_DIR = Obj

# List of source files (finds all .c files in Core/Src)
SRCS = $(wildcard $(SRC_DIR)/*.c)
# Transforme  .c in  .o (in the Obj folder pour rester propre)
OBJS = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRCS))

# default
all: $(TARGET)

# Link of our shell 
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^
	@echo "Compilation réussie ! Lancez ./$(TARGET)"

# Compilation of the object file 
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# creation of the folder if it don't exist 
$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

# cleaning 
clean:
	rm -rf $(OBJ_DIR) $(TARGET)

.PHONY: all clean
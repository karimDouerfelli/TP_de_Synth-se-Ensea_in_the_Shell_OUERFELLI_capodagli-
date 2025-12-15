#ifndef ENSEASH_H
#define ENSEASH_H

#define _GNU_SOURCE 200112L 

#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <time.h> 
#include <fcntl.h> // Required for Q7

// --- COLORS ---
#define RED "\033[0;31m" 
#define GRN "\033[0;32m"
#define L_CYAN "\033[1;36m"
#define NC  "\033[0m" // No Color (Reset)

// Defines
#define BUF_SIZE 1024
#define MAX_ARGS 64

#define WELCOME_MSG "Welcome to " RED "ENSEA Shell" NC ".\nType 'exit' to quit.\n"
#define PROMPT L_CYAN "enseash % " NC

// Prototypes
void display_message(const char *str);
void display_int(long value); // Displays an integer using write
int execute_command(char *command); 

#endif
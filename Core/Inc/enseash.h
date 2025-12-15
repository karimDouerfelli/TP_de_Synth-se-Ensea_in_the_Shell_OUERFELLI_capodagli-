#ifndef ENSEASH_H
#define ENSEASH_H

#define _GNU_SOURCE 200112L 

#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <stdio.h> 
#include <time.h> 


// --- COLORED OUTPUTS ---
#define RED "\033[0;31m" 
#define GRN "\033[0;32m"
#define NC  "\033[0m" // No Color (Reset)

#define BUF_SIZE 1024
#define WELCOME_MSG "Welcome to ENSEA Shell.\nType 'exit' to quit.\n"
#define PROMPT "enseash % "

// Prototypes
void display_message(const char *str);
int execute_command(char *command); 

#endif
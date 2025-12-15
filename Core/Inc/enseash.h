#ifndef ENSEASH_H
#define ENSEASH_H

#define _GNU_SOURCE 200112L 

#include <unistd.h>
#include <string.h>
#include <stdio.h> //  for perror
#include <sys/wait.h>
#include <stdlib.h>
#include <time.h> 
#include <fcntl.h> //  for open()

// Colors 
#define RED "\033[0;31m" 
#define GRN "\033[0;32m"
#define L_CYAN "\033[1;36m"
#define NC  "\033[0m" // No Color ->white

// Defines
#define BUF_SIZE 1024
#define MAX_ARGS 64

#define WELCOME_MSG "Welcome to " RED "ENSEA Shell" NC ".\nType 'exit' to quit.\n"
#define PROMPT L_CYAN "enseash % " NC

// Function defnitions
void display_message(const char *str);
void display_int(long value);
int execute_simple_command(char *command); 
int execute_command(char *command);


#endif
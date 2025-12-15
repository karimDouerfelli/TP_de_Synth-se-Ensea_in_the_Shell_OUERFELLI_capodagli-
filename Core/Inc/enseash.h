#ifndef ENSEASH_H
#define ENSEASH_H

#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <time.h>

#define BUF_SIZE 1024
#define WELCOME_MSG "Bienvenue dans le Shell ENSEA.\nPour quitter, tapez 'exit'.\n"
#define PROMPT "enseash % "

// Declarations
// On utilise une seule fonction générique pour afficher (Prompt ou Message)
void display_message(const char *str); 

#endif
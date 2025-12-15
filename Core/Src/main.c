/*
File : main.c
Author : Capodagli Janus, Ouerfelli Karim 
*/

#include "enseash.h"

int main(void) {
    // Q1: Welcome message 
    display_message(WELCOME_MSG);

    // Q1: Simple Prompt (Juste l'affichage pour l'instant)
    display_message(PROMPT);

    // Pour l'instant on quitte direct car on n'a pas encore la boucle (Q2)
    // Cela permet de tester si l'affichage fonctionne.
    return EXIT_SUCCESS;
}
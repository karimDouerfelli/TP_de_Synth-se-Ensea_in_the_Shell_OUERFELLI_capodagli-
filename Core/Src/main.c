/*
File : main.c
Author : Capodagli Janus, Ouerfelli Karim 
*/

#include "enseash.h"

int main(void) {
    char buffer[BUF_SIZE];
    // Q1: Welcome message 
    display_message(WELCOME_MSG);

    // Q1: Simple Prompt
    display_message(PROMPT);
    // Q2 
    while(1){
       int status = execute_command(buffer);
        
    }

    return EXIT_SUCCESS;
}
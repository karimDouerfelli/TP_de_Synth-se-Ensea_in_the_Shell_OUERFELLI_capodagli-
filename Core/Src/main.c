/*
File : main.c
Author : Capodagli Janus, Ouerfelli Karim 
*/

#include "enseash.h"

int main(void) {
    char buffer[BUF_SIZE];
    ssize_t bytes_read; // the number of octet readed 
    // Q1: Welcome message 
    display_message(WELCOME_MSG);

    // Q2 REPL boucle 
    while(1){
        // Q1: Simple Prompt
        display_message(PROMPT);
        //Q2 command read  ( the R from the boucle )
        bytes_read = read(STDIN_FILENO, buffer, BUF_SIZE -1); // we use read with the standard input (STDIN_FILENO)
        //gestion de <crtl>+D or lecture error
        if(bytes_read <= 0){
            break;
        }
        if(strlen(buffer)==0){
            continue;
        }
        buffer[bytes_read - 1]= '\0'; // replace the end with \0
        // Q3 
        if(strcmp(buffer,"exit")==0){
            break;
        }
        execute_command(buffer); // we execute the command 
        
    }

    display_message("Bye bye ... \n");
    return EXIT_SUCCESS;
}
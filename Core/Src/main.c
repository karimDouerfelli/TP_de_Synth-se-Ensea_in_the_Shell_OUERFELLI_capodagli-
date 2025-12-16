/*
File : main.c
Author : Capodagli Janus, Ouerfelli Karim 
*/

#include "enseash.h"

int main(void) {
    char buffer[BUF_SIZE];
    ssize_t bytes_read; // the number of octet readed 
    char prompt_msg[BUF_SIZE]; // new buffer for dynamic Q4
    int status =0; // retour command
    int first_loop = 1 ;// indication

    // Q1: Welcome message 
    display_message(WELCOME_MSG);
    // Q1: Simple Prompt
    display_message(PROMPT);

    // Q2 REPL boucle 
    while(1){

        //Q2 command read  ( the R from the boucle )
        bytes_read = read(STDIN_FILENO, buffer, BUF_SIZE -1); // we use read with the standard input (STDIN_FILENO)
        //gestion of <crtl>+D or lecture error
        if(bytes_read <= 0){
            break;
        }
        buffer[bytes_read - 1]= '\0'; // replace the end with \0

        if(strlen(buffer)==0){
            display_message(PROMPT);
            continue;
        }
        // Q3 
        if(strcmp(buffer,"exit")==0){
            break;
        }
        //we should save the status
        status=execute_command(buffer); // we execute the command 
                //Q4 
        //after : we use the previous state
        if(WIFEXITED(status)){// if the program finish (exit)
        //WEXITSTATUS use the retour code 
           if(write(prompt_msg,"enseash[exit:%d]%%",WEXITSTATUS(status))== -1){
            exit(EXIT_FAILURE);
           }

        }else if (WIFSIGNALED(status))
        {//if the program was murdred 
        // WTERMSIG save the signal number
        if(write(prompt_msg,"enseash[sign:%d]%%",WTERMSIG(status))== -1){
            exit(EXIT_FAILURE);
        }
            
        }

        

        
    }

    display_message("Bye bye ... \n");
    return EXIT_SUCCESS;
}
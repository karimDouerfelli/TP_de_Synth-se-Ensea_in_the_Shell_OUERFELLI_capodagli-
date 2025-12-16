/*
File : main.c
Author : Capodagli Janus, Ouerfelli Karim 
*/

#include "enseash.h"
#include <time.h>

int main(void) {
    char buffer[BUF_SIZE];
    ssize_t bytes_read; // the number of octet readed 
    char prompt_msg[BUF_SIZE]; // new buffer for dynamic Q4
    int status =0; // retour command
    struct timespec start, end; // struct to stock the time 
    long long elapsed_ms; // time en millisecondes 


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
        //the start of the chronometre  Q5
        clock_gettime(CLOCK_MONOTONIC, &start);
        //we should save the status
        status=execute_command(buffer); // we execute the command 
        //the end of the chronometre  Q5
        clock_gettime(CLOCK_MONOTONIC, &end);
        //calcul the time 
        elapsed_ms=(end.tv_sec - start.tv_sec)*1000 + (end.tv_nsec - start.tv_nsec)/1000000;
        //Q4 
        display_message("enseash[");

        //after : we use the previous state
        if(WIFEXITED(status)){// if the program finish (exit)
        //WEXITSTATUS use the retour code 
           display_message("exit:");
           write_integer(WEXITSTATUS(status));
           
           

        }else if (WIFSIGNALED(status))
        {//if the program was murdred 
        // WTERMSIG save the signal number
        display_message("sign:");
        write_integer(WTERMSIG(status));
    
        }
    //the time 
    display_message("|");
    write_integer((int)elapsed_ms);
    display_message("ms] %");   
    }

    display_message("Bye bye ... \n");
    return EXIT_SUCCESS;
}
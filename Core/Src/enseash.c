/*
File : enseash.c
Author : Capodagli Janus, Ouerfelli Karim
Description : Implementation of shell functions
*/

#include "enseash.h"

// Implementation of display_message using write
void display_message(const char *str) {
    // STDOUT_FILENO is usually 1. 
    // We use strlen because we are forbidden to use printf but allowed string.h
    if (write(STDOUT_FILENO, str, strlen(str)) == -1) // int write(int fileDescriptor, void *buffer, size_t bytesToWrite)
    { // when write = -1 it's mean a error 
        
        exit(EXIT_FAILURE);
    }
int execute_command(char *command){ // Q2 pdf page 56
    int pid , status ; 
    pid= fork();
    if(pid != 0 ){ // father code 
        wait(&status);
    }
    else{ // child code 
        execlp(command, command, (char *)NULL); //arguments :(filename, arg0, ...) we dont need the path 
        write(STDERR_FILENO,"Command not found , try sudo install \n",18);
        exit(EXIT_FAILURE);

    }
    exit(EXIT_SUCCESS);

}

};
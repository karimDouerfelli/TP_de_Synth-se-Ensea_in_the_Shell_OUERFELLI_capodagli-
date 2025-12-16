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
    }}
int execute_command(char *command){ // Q2 pdf page 56
    int pid , status ; 
    pid= fork();
    if(pid == - 1){
        //fork error
    return -1;
    }
    else if(pid != 0 ){ // father code 
        wait(&status);
        return status; // we send the info ot the main 
    }
    else{ // child code 
        execlp(command, command, (char *)NULL); //arguments :(filename, arg0, ...) we dont need the path 
        char *errormsg= "Command not found\n";
        write(STDERR_FILENO,errormsg,strlen(errormsg));
        exit(EXIT_FAILURE);
    }
}
void write_integer(int n){
    char buffer[20];
    int i =0;
    if(n==0){
        write(STDOUT_FILENO,"0",1);
        return;
    }
    //we() extract one by one 
    while(n > 0){
        buffer[i] = (n % 10) + '0'; // char ASCII
        n/=10;
        i++;
    }
    //buffer normal
    while(i > 0){
        i--;
        write(STDOUT_FILENO, &buffer[i], 1);
    }
}
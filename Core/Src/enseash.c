/*
File : enseash.c
Author : Capodagli Janus, Ouerfelli Karim
Description : Implementation of shell functions (Strictly NO printf)
              Includes parsing, execution, and redirection.
*/

#include "enseash.h"

// Simple wrapper for write
void display_message(const char *str) {
    if (write(STDOUT_FILENO, str, strlen(str)) == -1) {
        // In case of critical error on stdout, we exit
        exit(EXIT_FAILURE);
    }
}

// Converts an integer (long) to text and prints it directly using write
void display_int(long value) {
    char buffer[24]; // Large enough for a long (64 bits)
    int i = 0;
    
    // Special case for 0
    if (value == 0) {
        write(STDOUT_FILENO, "0", 1);
        return;
    }

    // Handle negative numbers
    if (value < 0) {
        write(STDOUT_FILENO, "-", 1);
        value = -value;
    }

    // Extract digits (e.g., 123 -> '3', '2', '1')
    while (value > 0) {
        buffer[i++] = (value % 10) + '0'; // int -> ASCII char conversion
        value /= 10;
    }
    
    // Print in correct order (reverse of extraction)
    while (i > 0) {
        write(STDOUT_FILENO, &buffer[--i], 1);
    }
}

int execute_command(char *command) {
    pid_t pid;
    int status;
    char *argv[MAX_ARGS];
    int argc = 0;

    // Redirection variables (Q7)
    char *redirect_file = NULL;
    int redirect_type = 0; // 0:None, 1:>, 2:<
    int fd;

    // --- 1. Parsing with Redirection Detection ---
    char *token = strtok(command, " ");
    
    while (token != NULL && argc < MAX_ARGS - 1) {
        if (strcmp(token, ">") == 0) {
            // Next token is the filename
            token = strtok(NULL, " ");
            if (token != NULL) { 
                redirect_type = 1; 
                redirect_file = token; 
            }
        } else if (strcmp(token, "<") == 0) {
            // Next token is the filename
            token = strtok(NULL, " ");
            if (token != NULL) { 
                redirect_type = 2; 
                redirect_file = token; 
            }
        } else {
            // Regular argument
            argv[argc++] = token;
        }
        token = strtok(NULL, " ");
    }
    argv[argc] = NULL;

    // Safety check for empty command
    if (argv[0] == NULL) return 0;

    // --- 2. Forking ---
    pid = fork();
    if (pid == -1) { 
        perror("fork"); 
        return -1; 
    }

    if (pid == 0) {
        // --- CHILD PROCESS ---
        
        // Handle Redirection
        if (redirect_type == 1) { // Output '>'
            fd = open(redirect_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
            if (fd == -1) { perror("open >"); exit(EXIT_FAILURE); }
            dup2(fd, STDOUT_FILENO);
            close(fd);
        } else if (redirect_type == 2) { // Input '<'
            fd = open(redirect_file, O_RDONLY);
            if (fd == -1) { perror("open <"); exit(EXIT_FAILURE); }
            dup2(fd, STDIN_FILENO);
            close(fd);
        }

        execvp(argv[0], argv);
        
        // Error message in red without printf
        write(STDERR_FILENO, RED "Command not found\n" NC, 
              sizeof(RED "Command not found\n" NC) - 1);
        exit(EXIT_FAILURE);
    } else {
        // --- PARENT PROCESS ---
        wait(&status);
        return status;
    }
}
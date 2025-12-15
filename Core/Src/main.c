/*
File : main.c

Author : 
    * Capodagli Janus
    * Ouerfelli Karim 
*/

// Dependencies 
#include <stdio.h>
#include <stdlib.h>
#include <syscall.h>
#include <assert.h>



int main(void) {
    // Q1: Welcome message 
    display_message(WELCOME_MSG);

    char buffer[BUF_SIZE];

    // Q2: REPL Loop (Read-Eval-Print Loop) 
    while (1) {
        // Display prompt
        display_message(PROMPT);

        // Q2a: Read command
        // Note: Reset buffer to ensure clean read
        memset(buffer, 0, BUF_SIZE);
        ssize_t bytes_read = read(STDIN_FILENO, buffer, BUF_SIZE - 1);

        // Q3: Handle Ctrl+D (EOF) which returns 0 bytes [cite: 41]
        if (bytes_read == 0) {
            display_message("\nBye bye...\n");
            break;
        }
        
        // Handle read errors
        if (bytes_read < 0) {
            const char *err_msg = "Error reading input.\n";
            write(STDERR_FILENO, err_msg, strlen(err_msg));
            exit(EXIT_FAILURE);
        }

        // Remove the newline character '\n' from the input
        buffer[strcspn(buffer, "\n")] = '\0';

        // Q3: Handle "exit" command [cite: 41]
        if (strncmp(buffer, EXIT_CMD, strlen(EXIT_CMD)) == 0 && strlen(buffer) == strlen(EXIT_CMD)) {
             display_message("Bye bye...\n");
             break;
        }

        // Q2b: Execute simple command
        execute_command(buffer);
    }

    return 0;
}












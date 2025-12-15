/*
File : main.c
Author : Capodagli Janus, Ouerfelli Karim 
Description : Main loop of the shell with execution time measurement (Q5)
*/

#include "enseash.h"
#include <time.h> // Required for clock_gettime

int main(void) {
    char buffer[BUF_SIZE];
    char prompt_msg[BUF_SIZE]; // Buffer for dynamic prompt
    ssize_t ret;
    int status = 0; // Stores the return status of the command
    
    // Q5: Timing variables
    struct timespec start, end;
    long elapsed_ms = 0;

    // Flag to check if it's the first loop iteration
    int first_command = 1; 

    display_message(WELCOME_MSG);

    while (1) {
        // --- 1. PROMPT DISPLAY (Logic concentrated here) ---
        // We display the result of the PREVIOUS command here
        if (first_command) {
            display_message(PROMPT);
            first_command = 0;
        } else {
            // Check how the previous command ended and format prompt
            if (WIFEXITED(status)) {
                // Format: [exit:code|time]
                // using snprintf for safety (prevent buffer overflow)
                snprintf(prompt_msg, BUF_SIZE, "enseash [exit:%d|%ldms] %% ", WEXITSTATUS(status), elapsed_ms);
            } else if (WIFSIGNALED(status)) {
                // Format: [sign:sig|time]
                snprintf(prompt_msg, BUF_SIZE, "enseash [sign:%d|%ldms] %% ", WTERMSIG(status), elapsed_ms);
            } else {
                // Fallback
                strncpy(prompt_msg, PROMPT, BUF_SIZE);
            }
            display_message(prompt_msg);
        }

        // --- 2. READ INPUT ---
        ret = read(STDIN_FILENO, buffer, BUF_SIZE - 1);

        // Handle Ctrl+D (EOF) or Error
        if (ret <= 0) {
            break; 
        }

        buffer[ret - 1] = '\0'; // Remove \n

        // Handle empty input (User just pressed Enter)
        if (strlen(buffer) == 0) {
            // If empty, we typically don't want to show the previous time/exit code again.
            // Resetting first_command to 1 allows us to show a clean prompt next loop.
            // Or simpler: just continue, and it will reprint the previous status. 
            // Let's stick to simple "continue" as per your previous snippet.
            continue; 
        }
        
        // Q3: Exit command
        if (strcmp(buffer, "exit") == 0) {
            display_message( RED " Bye bye...\n" NC);
            break;
        }

        // --- 3. EXECUTION & TIMING (Q5) ---
        
        // Start Timer
        clock_gettime(CLOCK_MONOTONIC, &start);

        // Execute
        status = execute_command(buffer);

        // Stop Timer
        clock_gettime(CLOCK_MONOTONIC, &end);

        // Calculate duration in milliseconds
        // (sec difference * 1000) + (nanosec difference / 1,000,000)
        elapsed_ms = (end.tv_sec - start.tv_sec) * 1000 + 
                     (end.tv_nsec - start.tv_nsec) / 1000000;
                     
        // Loop loops back to step 1 to display the result
    }

    return EXIT_SUCCESS;
}
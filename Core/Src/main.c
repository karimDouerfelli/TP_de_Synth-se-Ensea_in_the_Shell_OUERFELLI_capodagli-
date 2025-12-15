/*
File : main.c
Author : Capodagli Janus, Ouerfelli Karim 
Description : Main loop of the shell (Strictly NO PRINTF)
*/

#include "enseash.h"

int main(void) {
    char buffer[BUF_SIZE];
    ssize_t ret;
    int status = 0;
    
    struct timespec start, end;
    long elapsed_ms = 0;
    int first_command = 1; 

    display_message(WELCOME_MSG);

    while (1) {
        // --- 1. PROMPT DISPLAY (Piece by piece construction) ---
        if (first_command) {
            display_message(PROMPT); // Simple prompt for start : Light Cyan)
            first_command = 0;
        } else {
            // Start of standard prompt
            display_message(L_CYAN);
            display_message("enseash [");

            if (WIFEXITED(status)) {
                int exit_code = WEXITSTATUS(status);
                if (exit_code == 0) {
                    // SUCCESS: "exit:0"  | "time" (GREEN)
                    display_message(GRN);
                    display_message("exit:0|");
                    display_int(elapsed_ms);
                    display_message("ms");
                } else {
                    // ERROR: "exit:code" | "time" (RED) 
                    display_message(RED "exit:");
                    display_int((long)exit_code);
                    display_message("|");
                    display_int(elapsed_ms);
                    display_message("ms");
                    display_message(NC);
                }
            } else if (WIFSIGNALED(status)) {
                // SIGNAL: "sign:code" (RED)
                display_message(RED "sign:");
                display_int((long)WTERMSIG(status));
                display_message( "|");
                display_int(elapsed_ms);
                display_message("ms");
            }
            
            // End of prompt
            display_message(L_CYAN"] % "NC);
        }

        // --- 2. READ INPUT ---
        ret = read(STDIN_FILENO, buffer, BUF_SIZE - 1);

        if (ret <= 0) break; // EOF (Ctrl+D)

        buffer[ret - 1] = '\0'; // Remove newline \n

        // Handle empty input
        if (strlen(buffer) == 0) {
            first_command = 1; // Return to simple prompt
            continue; 
        }
        
        // Handle "exit" command
        if (strcmp(buffer, "exit") == 0) {
            display_message(RED "Bye bye...\n" NC);
            break;
        }

        // --- 3. EXECUTION ---
        clock_gettime(CLOCK_MONOTONIC, &start);
        status = execute_command(buffer);
        clock_gettime(CLOCK_MONOTONIC, &end);

        elapsed_ms = (end.tv_sec - start.tv_sec) * 1000 + 
                     (end.tv_nsec - start.tv_nsec) / 1000000;
    }
    return EXIT_SUCCESS;
}
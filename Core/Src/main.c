/*
File : main.c
Author : Capodagli Janus, Ouerfelli Karim 
We tried to make it as short as possible
*/

#include "enseash.h"

int main(void) {
    char input_buf[BUF_SIZE];
    ssize_t read_count;
    int status = 0;
    
    // Timing structures
    struct timespec t_start, t_end;
    long duration_ms = 0;

    // Show welcome message once
    write_out(MSG_WELCOME);
    write_out(PROMPT_TEXT); // First prompt is always simple

    while (1) { //infinite loop
        // 1. Wait for user input
        read_count = read(FD_STDIN, input_buf, BUF_SIZE - 1);

        // Handle (Ctrl+D) or Read Error
        if (read_count <= 0){break;}

        input_buf[read_count - 1] = '\0'; // Replace '\n' with null terminator

        // 2. Handle specific cases
        if (strlen(input_buf) == 0) { //length=0 means user pressed 'enter'
            //  Show simple prompt again
            write_out(PROMPT_TEXT); 
            continue; 
        }
        
        if (strcmp(input_buf, "exit") == 0) { //user entered "exit" so we quit the shell
            write_out(MSG_BYE); //Bye bye to the user
            break;
        }

        // 3. Execution & Timing
        clock_gettime(CLOCK_MONOTONIC, &t_start);
        
        status = execute_dispatcher(input_buf);
        
        clock_gettime(CLOCK_MONOTONIC, &t_end);

        // Compute elapsed time in milliseconds
        duration_ms = (t_end.tv_sec - t_start.tv_sec) * MS_PER_SEC + 
                      (t_end.tv_nsec - t_start.tv_nsec) / NS_PER_MS;

        // 4. Update UI for next turn
        show_prompt(status, duration_ms);
    }

    return EXIT_SUCCESS;
}
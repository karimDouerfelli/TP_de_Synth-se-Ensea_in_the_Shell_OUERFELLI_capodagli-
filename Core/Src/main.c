/*
File : main.c
Author : Capodagli Janus, Ouerfelli Karim 
We tried to make it as short as possible
*/

#include "enseaShell.h"

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

    // Stack struct for navigating commands
    cmd_stack *history_head = NULL; 
    cmd_stack *history_tail = NULL;

    while (1) { //infinite loop
        // 1. Wait for user input
        read_count = read(FD_STDIN, input_buf, BUF_SIZE - 1);

        // Handle (Ctrl+D) or Read Error
        if (read_count <= 0){
            write_out(CLR_RED "\nForced to quit shell\n"CLR_RESET);
            break;
        }

        input_buf[read_count - 1] = '\0'; // Replace '\n' with null terminator

        // 2. Handle specific cases
        if (strlen(input_buf) == 0) { //length=0 means user pressed 'enter'
            //  Show simple prompt again
            write_out(PROMPT_TEXT); 
            continue; 
        }
        
        if (strcmp(input_buf, "exit") == CMP_EQUAL) { //user entered "exit" so we quit the shell
            write_out(MSG_BYE); //Bye bye to the user
            break;
        }

        //  HISTORY MANAGEMENT
        // Add current command to the linked list
        cmd_stack *new_node = create_node(input_buf);

        if (history_head == NULL) {
            history_head = new_node;
            history_tail = new_node;
        } else {
            history_tail->next_cmd = new_node;
            new_node->previous_cmd = history_tail;
            history_tail = new_node;
        }

        // Check if user wants to see history
        if (strcmp(input_buf, "history") == CMP_EQUAL) {
            cmd_stack *curr = history_head;
            int id = 1;
            while (curr != NULL) {
                write_out(CLR_PRP);
                display_int(id++);
                write_out(": ");
                write_out(curr->curr_cmd);
                write_out("\n");
                curr = curr->next_cmd;
            }
            write_out(CLR_RESET);
            show_prompt(STATUS_SUCCESS, 0); 
            continue; 
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
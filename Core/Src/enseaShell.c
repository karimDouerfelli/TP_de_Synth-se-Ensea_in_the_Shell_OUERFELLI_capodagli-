/*
File : enseash.c
Author : Capodagli Janus, Ouerfelli Karim
Description : Shell logic implementation (Parsing, Exec, Signals).
*/

#include "enseaShell.h"

//  I/O HELPERS 

void write_out(const char *str) { //to make write shorter&easier : write_out(const char *str) 
                                            //instead of write(FD_STDOUT, str, strlen(str) ) and with failure management
    if (write(FD_STDOUT, str, strlen(str)) == STATUS_ERROR){
        exit(EXIT_FAILURE);
    }
}

void write_err(const char *str) {
    if (write(FD_STDERR, str, strlen(str)) == STATUS_ERROR){
        exit(EXIT_FAILURE);
    }
}

// Converts long to string manually (replace itoa function)
void display_int(long value) {
    char buffer[INT_BUF_SIZE]; 
    int index = 0;
    
    if (value == 0) {
        write_out("0");
        return;
    }
    
    // Handle negatives (just in case)
    if (value < 0) {
        write_out("-");
        value = -value;
    }

    // Extract digits
    while (value > 0) {
        buffer[index++] = (value % 10) + '0';
        value /= 10;
    }
    
    // Print (in reversed to make the display order correct)
    while (index > 0) {
        char c = buffer[--index];
        write(FD_STDOUT, &c, 1);
    }
}

//  UI LOGIC 


// Displays the complex prompt:  "enseash [status|time elapsed] %"
void show_prompt(int status, long elapsed_ms) {
    write_out(CLR_CYAN "enseaShell [" CLR_RESET);

    if (WIFEXITED(status)) {
        int returnCode = WEXITSTATUS(status);  // EXIT_SUCCESS (0) or EXIT_FAILURE (1) 
        if (returnCode == EXIT_SUCCESS) {
            // Success: Green time
            write_out("exit:0|");
            write_out(CLR_GRN);
            display_int(elapsed_ms);
            write_out("ms" CLR_RESET);
        } else {
            // Error: Red status
            write_out(CLR_RED "exit:");
            display_int((long)returnCode);
            write_out(CLR_RESET "|");
            display_int(elapsed_ms);
            write_out("ms");
        }
    } else if (WIFSIGNALED(status)) {  
        // Signal: Red status
        write_out(CLR_RED "sign:");
        display_int((long)WTERMSIG(status));
        write_out(CLR_RESET "|");
        display_int(elapsed_ms);
        write_out("ms");
    }

    write_out(CLR_CYAN "] % " CLR_RESET); //Cyan for the rest, and white when no process involved
}

// EXECUTION LOGIC

/*
  Parses and executes a single command line.
  Handles '<' and '> 'but not '|' 
*/
int run_simple_cmd(char *cmd_line) {
    pid_t pid;
    int status;
    char *args[MAX_ARGS];
    int arg_idx = 0;

    // Redirection flags
    char *file_target = NULL;
    RedirectionMode redir_mode = REDIR_NONE;
    int fd_file;

    // Parsing tokens
    char *cursor = strtok(cmd_line, " "); //strtok extracts tokens from strings using the given separator (space) which is converted to \0
                    // cursor is in form of pointer on char (string)that points to the first token, and pass to next one when we call strtok(NULL, " ")
    while (cursor != NULL && arg_idx < MAX_ARGS - 1) {
        if (strcmp(cursor, ">") == CMP_EQUAL) { // ">" detected
            cursor = strtok(NULL, " "); //next token
            if (cursor) { redir_mode = REDIR_OUTPUT; file_target = cursor; }
        } 
        else if (strcmp(cursor, "<") == CMP_EQUAL) {// "<" detected
            cursor = strtok(NULL, " ");//next token
            if (cursor) { redir_mode = REDIR_INPUT; file_target = cursor; }
        } 
        else {
            args[arg_idx++] = cursor;
        }
        cursor = strtok(NULL, " ");
    }
    args[arg_idx] = NULL; // Null terminate list

    // Empty command check
    if (args[0] == NULL){
        return STATUS_SUCCESS;
    }

    pid = fork();
    if (pid == STATUS_ERROR) { 
        perror("fork"); 
        return STATUS_ERROR; 
    }

    if (pid == CHILD_PID) {
        // --- CHILD ---
        if (redir_mode == REDIR_OUTPUT) { // '>'
            fd_file = open(file_target, O_WRONLY | O_CREAT | O_TRUNC, FILE_PERMS);
            if (fd_file == FD_ERROR) { perror("open >"); exit(EXIT_FAILURE); }
            dup2(fd_file, FD_STDOUT);
            close(fd_file);
        } 
        else if (redir_mode == REDIR_INPUT) { // '<'
            fd_file = open(file_target, O_RDONLY);
            if (fd_file == FD_ERROR) { perror("open <"); exit(EXIT_FAILURE); }
            dup2(fd_file, FD_STDIN);
            close(fd_file);
        }

        execvp(args[0], args);
        
        // If we get here, exec failed
        write_err(MSG_CMD_ERR);
        exit(EXIT_FAILURE);
    } else {
        // --- PARENT ---
        wait(&status);
        return status;
    }
}

/*
  Dispatcher: Checks for Pipe '|' then delegates.
*/
int execute_dispatcher(char *raw_command) {
    char *pipe_loc = strchr(raw_command, '|');

    if (pipe_loc == NULL) {
        // Standard execution
        return run_simple_cmd(raw_command);
    } 
    
    // --- PIPE HANDLING ---
    *pipe_loc = '\0'; // Split string
    char *lhs_cmd = raw_command;
    char *rhs_cmd = pipe_loc + 1;

    int pipefd[2]; // 0: Read, 1: Write
    pid_t pid_l, pid_r;
    int final_status;

    if (pipe(pipefd) == STATUS_ERROR) {
        perror("pipe");
        return STATUS_ERROR;
    }

    // Left Child (Writer)
    pid_l = fork();
    if (pid_l == CHILD_PID) {
        close(pipefd[0]); // No read
        dup2(pipefd[1], FD_STDOUT);
        close(pipefd[1]);
        exit(WEXITSTATUS(run_simple_cmd(lhs_cmd)));
    }

    // Right Child (Reader)
    pid_r = fork();
    if (pid_r == CHILD_PID) {
        close(pipefd[1]); // No write
        dup2(pipefd[0], FD_STDIN);
        close(pipefd[0]);
        exit(WEXITSTATUS(run_simple_cmd(rhs_cmd)));
    }

    // Parent cleanup
    close(pipefd[0]);
    close(pipefd[1]);

    // Wait for both, return status of the right-hand command
    waitpid(pid_l, NULL, 0);
    waitpid(pid_r, &final_status, 0);

    return final_status;
}


// I wanted to add command navigation with  UP and Down buttons
// But it was a bit complexe I couldn't make it in time 
//so I just add a storage of command so I can execute history
//  HISTORY MANAGEMENT 

cmd_stack *create_node(const char *command) {
    cmd_stack *node = (cmd_stack *)malloc(sizeof(cmd_stack));
    if (node == NULL) {
        write_err("Malloc failed\n");
        exit(EXIT_FAILURE);
    }

    node->curr_cmd = strdup(command); // calculate length, malloc, then copy the string
    if (node->curr_cmd == NULL) {
        free(node);
        write_err("Strdup failed\n");
        exit(EXIT_FAILURE);
    }

    node->previous_cmd = NULL;
    node->next_cmd = NULL;
    return node;
}

void free_stack(cmd_stack *head) {
    cmd_stack *tmp;
    while (head != NULL) {
        tmp = head;
        head = head->next_cmd;
        free(tmp->curr_cmd); 
        free(tmp);           
    }
}




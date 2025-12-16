/*
File : enseash.c
Author : Capodagli Janus, Ouerfelli Karim
Description : Shell logic implementation (Parsing, Exec, Signals).
*/

#include "enseash.h"

// --- I/O HELPERS ---

void write_out(const char *str) {
    if (write(FD_STDOUT, str, strlen(str)) == -1){
        exit(EXIT_FAILURE);
    }
}

void write_err(const char *str) {
    if (write(FD_STDERR, str, strlen(str)) == -1){
        exit(EXIT_FAILURE);
    }
}

// Converts long to string manually to avoid printf
void display_int(long value) {
    char buffer[24]; 
    int idx = 0;
    
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
        buffer[idx++] = (value % 10) + '0';
        value /= 10;
    }
    
    // Print reversed
    while (idx > 0) {
        char c = buffer[--idx];
        write(FD_STDOUT, &c, 1);
    }
}

// --- UI LOGIC ---

/*
 Displays the complex prompt: enseash [exit:0|4ms] %
 Centralizes all display logic to keep main.c clean.
*/
void show_prompt(int status, long elapsed_ms) {
    write_out(CLR_CYAN "enseash [" CLR_RESET);

    if (WIFEXITED(status)) {
        int code = WEXITSTATUS(status);
        if (code == 0) {
            // Success: Green time
            write_out("exit:0|");
            write_out(CLR_GRN);
            display_int(elapsed_ms);
            write_out("ms" CLR_RESET);
        } else {
            // Error: Red status
            write_out(CLR_RED "exit:");
            display_int((long)code);
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

    write_out(CLR_CYAN "] % " CLR_RESET);
}

// --- EXECUTION LOGIC ---

/*
  Parses and executes a single command line.
  Handles < and > redirections.
*/
int run_simple_cmd(char *cmd_line) {
    pid_t pid;
    int status;
    char *args[MAX_ARGS];
    int arg_idx = 0;

    // Redirection flags
    char *file_target = NULL;
    int redir_mode = 0; // 0: None, 1: Out (>), 2: In (<)
    int fd_file;

    // Parsing tokens
    char *cursor = strtok(cmd_line, " ");
    while (cursor != NULL && arg_idx < MAX_ARGS - 1) {
        if (strcmp(cursor, ">") == 0) {
            cursor = strtok(NULL, " ");
            if (cursor) { redir_mode = 1; file_target = cursor; }
        } 
        else if (strcmp(cursor, "<") == 0) {
            cursor = strtok(NULL, " ");
            if (cursor) { redir_mode = 2; file_target = cursor; }
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
    if (pid == -1) { 
        perror("fork"); 
        return STATUS_ERROR; 
    }

    if (pid == 0) {
        // --- CHILD ---
        if (redir_mode == 1) { // '>'
            fd_file = open(file_target, O_WRONLY | O_CREAT | O_TRUNC, FILE_PERMS);
            if (fd_file == -1) { perror("open >"); exit(EXIT_FAILURE); }
            dup2(fd_file, FD_STDOUT);
            close(fd_file);
        } 
        else if (redir_mode == 2) { // '<'
            fd_file = open(file_target, O_RDONLY);
            if (fd_file == -1) { perror("open <"); exit(EXIT_FAILURE); }
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

    int pipe_fds[2]; // 0: Read, 1: Write
    pid_t pid_l, pid_r;
    int final_status;

    if (pipe(pipe_fds) == -1) {
        perror("pipe");
        return STATUS_ERROR;
    }

    // Left Child (Writer)
    pid_l = fork();
    if (pid_l == 0) {
        close(pipe_fds[0]); // No read
        dup2(pipe_fds[1], FD_STDOUT);
        close(pipe_fds[1]);
        exit(WEXITSTATUS(run_simple_cmd(lhs_cmd)));
    }

    // Right Child (Reader)
    pid_r = fork();
    if (pid_r == 0) {
        close(pipe_fds[1]); // No write
        dup2(pipe_fds[0], FD_STDIN);
        close(pipe_fds[0]);
        exit(WEXITSTATUS(run_simple_cmd(rhs_cmd)));
    }

    // Parent cleanup
    close(pipe_fds[0]);
    close(pipe_fds[1]);

    // Wait for both, return status of the right-hand command
    waitpid(pid_l, NULL, 0);
    waitpid(pid_r, &final_status, 0);

    return final_status;
}
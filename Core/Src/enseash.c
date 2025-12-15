/*
File : enseash.c
Author : Capodagli Janus, Ouerfelli Karim
Description : Implementation of shell functions.
              Handles parsing, simple execution, redirections (<, >), and pipes (|).
*/

#include "enseash.h"

// Helper Functions 

void display_message(const char *str) {
    if (write(STDOUT_FILENO, str, strlen(str)) == -1) {
        exit(EXIT_FAILURE);
    }
}

void display_int(long value) {
    char buffer[24]; 
    int i = 0;
    
    if (value == 0) {
        write(STDOUT_FILENO, "0", 1);
        return;
    }
    if (value < 0) {
        write(STDOUT_FILENO, "-", 1);
        value = -value;
    }
    while (value > 0) {
        buffer[i++] = (value % 10) + '0';
        value /= 10;
    }
    while (i > 0) {
        write(STDOUT_FILENO, &buffer[--i], 1);
    }
}

// --- Execution Logic ---

/*
  Executes a single command (whithout '|').
  This is essentially from the code I did for Question 7.
 */
int execute_simple_command(char *command) {
    pid_t pid;
    int status;
    char *argv[MAX_ARGS];
    int argc = 0;

    // Redirection variables
    char *redirect_file = NULL;
    int redirect_type = 0; // 0:None, 1:>, 2:<
    int fd;

    // Use strtok to parse args
    char *token = strtok(command, " ");
    
    while (token != NULL && argc < MAX_ARGS - 1) {
        if (strcmp(token, ">") == 0) {
            token = strtok(NULL, " ");
            if (token != NULL) { redirect_type = 1; redirect_file = token; }
        } else if (strcmp(token, "<") == 0) {
            token = strtok(NULL, " ");
            if (token != NULL) { redirect_type = 2; redirect_file = token; }
        } else {
            argv[argc++] = token;
        }
        token = strtok(NULL, " ");
    }
    argv[argc] = NULL;

    if (argv[0] == NULL) return 0;

    pid = fork();
    if (pid == -1) { perror("fork"); return -1; }

    if (pid == 0) {
        //  CHILD process
        // Handle Redirections
        if (redirect_type == 1) { // we've got an '>'
            fd = open(redirect_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
            if (fd == -1) { perror("open >"); exit(EXIT_FAILURE); }
            dup2(fd, STDOUT_FILENO);
            close(fd);
        } else if (redirect_type == 2) { // we've got an '<'
            fd = open(redirect_file, O_RDONLY);
            if (fd == -1) { perror("open <"); exit(EXIT_FAILURE); }
            dup2(fd, STDIN_FILENO);
            close(fd);
        }

        execvp(argv[0], argv);
        
        // Error handling
        write(STDERR_FILENO, RED "Command not found\n" NC, 
              sizeof(RED "Command not found\n" NC) - 1);
        exit(EXIT_FAILURE);
    } else {
        //  PARENT process
        wait(&status);
        return status;
    }
}

/*
 Q8 strtegy : 
 I check for '|' first. 
 If found, splits execution. If not, calls simple execution.
 */
int execute_command(char *command) {
    // Check if there is a pipe symbol in the command
    char *pipe_pos = strchr(command, '|');

    if (pipe_pos != NULL) {
        // --- PIPE DETECTED ---
        
        // 1. Split the string into two parts
        *pipe_pos = '\0'; // Replace '|' with null terminator
        char *left_cmd = command;
        char *right_cmd = pipe_pos + 1; // Start of next command

        // Pipe file descriptors: fd[0] = read, fd[1] = write
        int pipefd[2];
        pid_t pid1, pid2;
        int status;

        if (pipe(pipefd) == -1) {
            perror("pipe");
            return -1;
        }

        // 2. First Child (Left Command) -> Writes to Pipe
        pid1 = fork();
        if (pid1 == 0) {
            // Close Read end (unused by this child)
            close(pipefd[0]);
            // Redirect Stdout to Write end
            dup2(pipefd[1], STDOUT_FILENO);
            close(pipefd[1]); // Close original fd after dup
            
            // Execute the left command
            // We exit with the return value of the command
            exit(WEXITSTATUS(execute_simple_command(left_cmd)));
        }

        // 3. Second Child (Right Command) -> Reads from Pipe
        pid2 = fork();
        if (pid2 == 0) {
            // Close Write end (unused by this child)
            close(pipefd[1]);
            // Redirect Stdin to Read end
            dup2(pipefd[0], STDIN_FILENO);
            close(pipefd[0]); // Close original fd after dup

            // Execute the right command
            exit(WEXITSTATUS(execute_simple_command(right_cmd)));
        }

        // 4. Parent Logic
        // Parent must close pipe ends, otherwise children hang waiting for EOF
        close(pipefd[0]);
        close(pipefd[1]);

        // Wait for children
        // We generally return the status of the last command in the pipe (right side)
        waitpid(pid1, NULL, 0);
        waitpid(pid2, &status, 0); 

        return status;

    } else {
        // --- NO PIPE ---
        return execute_simple_command(command);
    }
}
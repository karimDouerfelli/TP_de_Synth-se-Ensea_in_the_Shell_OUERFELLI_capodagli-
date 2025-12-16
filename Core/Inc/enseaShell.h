#ifndef ENSEASHELL_H
#define ENSEASHELL_H


//same as in TDms, if i dont add this line gcc wont compile:
#define _GNU_SOURCE 200112L 

#include <unistd.h>
#include <string.h>
#include <stdio.h> 
#include <sys/wait.h>
#include <stdlib.h>
#include <time.h> 
#include <fcntl.h> 

//  COLORS 
#define CLR_RED     "\033[0;31m" //red color
#define CLR_GRN     "\033[0;32m" //green color
#define CLR_YLW     "\033[1;33m"  //yellow color
#define CLR_CYAN    "\033[0;36m" //Cyan
#define CLR_PRP   "\033[0;35m"  //purple
#define CLR_RESET   "\033[0m"  //reset color (white)

// CONSTANTS 
#define BUF_SIZE        1024
#define MAX_ARGS        64
#define INT_BUF_SIZE    24 //for the function display_int-- 
                            //sufficient for a signed long type (around 20chars)

// Time conversion
#define NS_PER_MS       1000000
#define MS_PER_SEC      1000

// File Descriptors 
#define FD_STDIN        0
#define FD_STDOUT       1
#define FD_STDERR       2

// Return codes for internal functions
#define STATUS_SUCCESS  0
#define STATUS_ERROR    -1
#define FD_ERROR        -1
#define CHILD_PID 0
#define CMP_EQUAL 0

// File permissions I used in case we create file (O_CREAT -> enseash.c line138): rw-r--r--
#define FILE_PERMS      0644
/*
0644 => 0 (octale number) + 6 (admin) 4(groupe)  4(other)
    In octale, we can represent permissions by the following rule :
        Read = 4
        Write = 2
        Execute = 1
        None = 0  then we add for each. so '6' is 4(r)+2(w)+0(-) means "rw-"
*/

// Messages
#define MSG_WELCOME     CLR_YLW "Welcome to ENSEA Shell \nAuthors : CAPODAGLI Janus & OUERFELLI Karim.\nType 'exit' to quit.\nType 'history' to see commands history\n-----------------\n"CLR_RESET
#define MSG_BYE         CLR_RED "Bye bye...\n" CLR_RESET
#define MSG_CMD_ERR     CLR_RED "Command not found\n" CLR_RESET
#define PROMPT_TEXT     "enseaShell % "

// Redirection Modes (Replaces magic numbers 0, 1, 2)
typedef enum {
    REDIR_NONE ,
    REDIR_OUTPUT,  
    REDIR_INPUT    
} RedirectionMode;

// Custom type with LIFO structure for cammands storage (in order for the shell to navigate between commands)
#define MAX_CMD_IDX  100   //we dont want to store more than 100 commands

// Structure Linked List pour l'historique
typedef struct cmd_stack {
    struct cmd_stack *previous_cmd;
    struct cmd_stack *next_cmd;
    char *curr_cmd;
} cmd_stack;



// functions declarations ( prototypes)

// I/O functions
void write_out(const char *str);     // wrapper for write(STDOUT)
void write_err(const char *str);     // wrapper for write(STDERR)
void display_int(long value);        // home made int-to-string print

// Core logic functions
void show_prompt(int status, long elapsed_ms);
int execute_dispatcher(char *raw_command);

// History Management
cmd_stack *create_node(const char *command);
void free_stack(cmd_stack *head);

#endif
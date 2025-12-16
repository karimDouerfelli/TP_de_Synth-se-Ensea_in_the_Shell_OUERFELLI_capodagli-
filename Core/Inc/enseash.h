#ifndef ENSEASH_H
#define ENSEASH_H


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
#define CLR_CYAN    "\033[1;36m" //Cyan
#define CLR_RESET   "\033[0m"  //reset color (white)

// CONSTANTS 
#define BUF_SIZE        1024
#define MAX_ARGS        64

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

// File permissions I used : rw-r--r--
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
#define MSG_WELCOME     "Welcome to " CLR_RED "ENSEA Shell" CLR_RESET ".\nType 'exit' to quit.\n"
#define MSG_BYE         CLR_RED "Bye bye...\n" CLR_RESET
#define MSG_CMD_ERR     CLR_RED "Command not found\n" CLR_RESET
#define PROMPT_TEXT     "enseash % "

// functions definitions ( prototypes)

// I/O functions
void write_out(const char *str);     // wrapper for write(STDOUT)
void write_err(const char *str);     // wrapper for write(STDERR)
void display_int(long value);        // home made int-to-string print

// Core logic functions
void show_prompt(int status, long elapsed_ms);
int execute_dispatcher(char *raw_command);

#endif
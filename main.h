#ifndef MAIN_H
#define MAIN_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>
#include <sys/wait.h>

#define SUCCESS 0
#define FAILURE 1
#define EXTERNAL 1
#define INTERNAL 2
#define ECHO 3
#define INVALID 4
#define MAX_ARGS 64
#define MAX_INPUT 1024
#define NO_JOB -1

// Job state
typedef enum
{
    RUNNING,
    STOPPED
} job_state_t;

// Job linked list
typedef struct job
{
    pid_t pid;
    job_state_t state;
    struct job *next;
} job_t;

extern job_t *head;

// global shell state
typedef struct
{
    char shell_path[256];
    int last_exit_status;
    pid_t shell_pid;
    pid_t fg_pid;
    char prompt[30];
} shell_state_t;

extern shell_state_t SHELL;

// parsing functions
int parse_input(char *input, char **args);
int check_command_type(char *args);

// command execution function
int execute_external_commands(char *cmd[]);
int execute_internal_commands(char *cmd[]);

// signal handling
void setup_signals(void);

// Shell initialization
void init_shell(void);
void display_prompt(void);

// Job functions
void insert_job(pid_t pid, job_state_t state);
void delete_job(pid_t pid);
void display_jobs();
void refresh_jobs();
void mark_job_stopped(pid_t pid);
void mark_job_running(pid_t pid);
job_t *find_stopped_job();
void init_job();

// echo command
void echo(char *args[]);

// Pipe handling
int has_pipe(char *input);
int split_pipe(char *input, char *args[]);
int get_cmd_type(char *cmd);
void execute_command_in_child(char *args[]);
int execute_pipe(char *cmds[], int cmd_count);

#endif
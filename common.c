/***************************************************************************************************************************************************
* Author        : Labhesh Patil
* Date          : Mon Feb 09 2026
* File          : common.c
* Title         : Utility functions for command parsing and type detection
* Description   : Provides helper functions for parsing user input into argument arrays, detecting command
*                 types (external, built-in, echo), and maintaining the built-in command list. Also includes
*                 the echo implementation with support for special variable expansion ($$, $?, $SHELL).
****************************************************************************************************************************************************/

#include "main.h"

// list of builtin commands
char *builtins[] = {"cd", "pwd", "fg", "bg", "exit", "jobs", "clear", NULL};

// split input into individual arguments
int parse_input(char *input, char *args[])
{
    int i = 0;
    char *token = strtok(input, " ");
    while (token != NULL && i < MAX_ARGS - 1)
    {
        args[i++] = token;
        token = strtok(NULL, " ");
    }
    args[i] = NULL;
    return i;
}

// determine command type
int check_command_type(char *args)
{
    if (strcmp(args, "echo") == 0)
        return ECHO;

    for (int i = 0; builtins[i] != NULL; i++)
    {
        if (strcmp(args, builtins[i]) == 0)
            return INTERNAL;
    }

    return EXTERNAL;
}

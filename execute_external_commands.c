/***************************************************************************************************************************************************
* Author        : Labhesh Patil
* Date          : Thu Feb 05 2026
* File          : execute_external_commands.c
* Title         : External command execution handler
* Description   : Handles execution of external programs by forking a child process and using execvp.
*                 Parent process waits for foreground commands, tracks exit status, handles stopped processes
*                 via SIGTSTP, and adds them to the background job list. Properly reports termination reasons
*                 and exit values for all executed external commands.
****************************************************************************************************************************************************/

#include "main.h"

// Execute external command 
int execute_external_commands(char *cmd[])
{

    pid_t pid = fork();
    int exit_val = 0;

    // Child process
    if (pid == 0)
    {
        //reset signal handlers to default
        signal(SIGINT, SIG_DFL);
        signal(SIGTSTP, SIG_DFL);

        if (execvp(cmd[0], cmd) == -1)
        {
            if (errno == ENOENT)
            {
                fprintf(stderr, "Command '%s' not found\n", cmd[0]);
                exit(127);
            }
            else if (errno == EACCES)
            {
                fprintf(stderr, "Permission denied: %s\n", cmd[0]);
                exit(126);
            }
            else
            {
                fprintf(stderr, "Execution failed: %s\n", strerror(errno));
                exit(1);
            }
        }
    }
    else if (pid > 0)//parent process
    {
        SHELL.fg_pid = pid;

        int status;
        if (waitpid(pid, &status, WUNTRACED) == -1)
        {
            perror("waitpid");
            SHELL.fg_pid = -1;
            return FAILURE;
        }
        //check if process was stopped
        if (WIFSTOPPED(status))
        {
            printf("The child %d stopped by receiving the signal %d\n", pid, (exit_val = WSTOPSIG(status)));
            insert_job(pid, STOPPED);
            printf("[%d] Stopped\n", pid);
            SHELL.fg_pid = -1;
            return exit_val += 128;
        }

        SHELL.fg_pid = -1;

        // Check exit status
        if (WIFEXITED(status))
        {
            printf("The child %d terminated normally with value %d\n", pid, (exit_val = WEXITSTATUS(status)));
            return exit_val;
        }
        else if (WIFSIGNALED(status))
        {
            printf("The child %d terminated abnormally by receiving the signal %d\n", pid, (exit_val = WTERMSIG(status)));
            return exit_val += 128;
        }
    }
    else
    {
        perror("fork");
        return FAILURE;
    }
    return SUCCESS;
}
/***************************************************************************************************************************************************
* Author        : Labhesh Patil
* Date          : Thu Feb 05 2026
* File          : execute_internal_commands.c
* Title         : Built-in command execution handler
* Description   : Implements all shell built-in commands including cd for directory changes, pwd for current
*                 directory display, jobs for listing background processes, fg for bringing jobs to foreground,
*                 and bg for resuming stopped jobs in background. All built-ins execute within the shell process
*                 without forking, maintaining the shell's internal state.
****************************************************************************************************************************************************/

#include "main.h"

// Builtin commands(internal cmd)
int execute_internal_commands(char *cmd[])
{

    // cd
    if (strcmp(cmd[0], "cd") == 0)
    {
        if (cmd[1] == NULL)
        {
            fprintf(stderr, "cd: missing argument\n");
            return FAILURE;
        }
        else if (chdir(cmd[1]) != 0)
        {
            perror("cd");
            return FAILURE;
        }
        return SUCCESS;
    }

    // pwd
    if (strcmp(cmd[0], "pwd") == 0)
    {
        char *cwd = getcwd(NULL, 0);
        if (cwd)
        {
            printf("%s\n", cwd);
            free(cwd);
            return SUCCESS;
        }
        return FAILURE;
    }

    // jobs
    if (strcmp(cmd[0], "jobs") == 0)
    {
        display_jobs();
        return SUCCESS;
    }

    // fg
    if (strcmp(cmd[0], "fg") == 0)
    {
        job_t *job = find_stopped_job();
        if (!job)
        {
            printf("fg: no stopped job\n");
            return SUCCESS;
        }

        pid_t pid = job->pid;

        kill(pid, SIGCONT);
        
        mark_job_running(pid);

        int status;
        SHELL.fg_pid = pid;
        waitpid(pid, &status, WUNTRACED);
        SHELL.fg_pid = -1;

        if (WIFSTOPPED(status))
            mark_job_stopped(pid);
        else
            delete_job(pid);

        return SUCCESS;
    }

    // bg
    if (strcmp(cmd[0], "bg") == 0)
    {
        job_t *job = find_stopped_job();
        if (!job)
        {
            printf("bg: no stopped job\n");
            return SUCCESS;
        }

        pid_t pid = job->pid;
        kill(pid, SIGCONT);
        mark_job_running(pid);

        printf("[%d] Running\n", pid);

        return SUCCESS;
    }
}
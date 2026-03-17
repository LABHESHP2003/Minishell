#include "main.h"

// Execute external command 
int execute_external_commands(char *cmd[])
{

    pid_t pid = fork();
    int exit_val = 0;

    if (pid == 0)
    {
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
    else if (pid > 0)
    {
        SHELL.fg_pid = pid;

        int status;
        if (waitpid(pid, &status, WUNTRACED) == -1)
        {
            perror("waitpid");
            SHELL.fg_pid = -1;
            return FAILURE;
        }

        if (WIFSTOPPED(status))
        {
            printf("The child %d stopped by receiving the signal %d\n", pid, (exit_val = WSTOPSIG(status)));
            insert_job(pid, STOPPED);
            printf("[%d] Stopped\n", pid);
            SHELL.fg_pid = -1;
            return exit_val += 128;
        }

        SHELL.fg_pid = -1;

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
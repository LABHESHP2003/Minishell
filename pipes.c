#include "main.h"

// check for the pipe character
int has_pipe(char *input)
{
    return strchr(input, '|') != NULL;
}

// split input by pipe
int split_pipe(char *input, char *args[])
{

    int pipe_count = 0;
    char *cmd = strtok(input, "|");
    while (cmd && pipe_count < MAX_ARGS - 1)
    {
        while (*cmd == ' ')
        {
            cmd++;
        }
        args[pipe_count++] = cmd;
        cmd = strtok(NULL, "|");
    }
    args[pipe_count] = NULL;

    return pipe_count;
}

// detect command type inside pipe
int get_cmd_type(char *cmd)
{
    if (strcmp(cmd, "echo") == 0)
        return ECHO;
    return check_command_type(cmd);
}

// execute single piped command
void execute_command_in_child(char *args[])
{
    int type = get_cmd_type(args[0]);

    if (type == INTERNAL)
    {
        execute_internal_commands(args);
        exit(0);
    }
    else if (type == ECHO)
    {
        echo(args);
        exit(0);
    }
    else
    {
        execvp(args[0], args);

        if (errno == ENOENT)
            exit(127);
        else if (errno == EACCES)
            exit(126);
        else
            perror("execvp");
            exit(1);
    }
}

// Execute pipe
int execute_pipe(char *cmds[], int cmd_count)
{
    int prev_fd[2] = {-1, -1};
    int curr_fd[2];
    pid_t pid;
    int status;
    int last_exit_status = 0;
    pid_t last_pid = -1;

    for (int cmd_no = 0; cmd_no < cmd_count; cmd_no++)
    {
        char *args[MAX_ARGS];
        parse_input(cmds[cmd_no], args);

        if (cmd_no < cmd_count - 1)
        {
            if (pipe(curr_fd) < 0)
            {
                perror("pipe");
                return FAILURE;
            }
        }

        pid = fork();
        if (pid < 0)
        {
            perror("fork");
            return FAILURE;
        }

        if (pid == 0)
        {
            signal(SIGINT, SIG_DFL);
            signal(SIGTSTP, SIG_DFL);

            if (cmd_no > 0)
                dup2(prev_fd[0], STDIN_FILENO);

            if (cmd_no < cmd_count - 1)
                dup2(curr_fd[1], STDOUT_FILENO);

            if (cmd_no > 0)
            {
                close(prev_fd[0]);
                close(prev_fd[1]);
            }
            if (cmd_no < cmd_count - 1)
            {
                close(curr_fd[0]);
                close(curr_fd[1]);
            }

            execute_command_in_child(args);
        }

        if (cmd_no > 0)
        {
            close(prev_fd[0]);
            close(prev_fd[1]);
        }

        if (cmd_no < cmd_count - 1)
        {
            prev_fd[0] = curr_fd[0];
            prev_fd[1] = curr_fd[1];
        }

        if (cmd_no == cmd_count - 1){
             last_pid = pid;
        }
    }

    waitpid(last_pid, &status, 0);

    if (WIFEXITED(status))
        last_exit_status = WEXITSTATUS(status);
    else if (WIFSIGNALED(status))
        last_exit_status = 128 + WTERMSIG(status);

    while (wait(NULL) > 0);

    return last_exit_status;
}
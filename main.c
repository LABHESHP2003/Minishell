#include "main.h"

// global shell
shell_state_t SHELL;

// Implementation of echo command
void echo(char *args[])
{
    for (int i = 1; args[i] != NULL; i++)
    {
        if (strcmp(args[i], "$$") == 0)
        {
            printf("%d", SHELL.shell_pid);
        }
        else if (strcmp(args[i], "$?") == 0)
        {
            printf("%d", SHELL.last_exit_status);
        }
        else if (strcmp(args[i], "$SHELL") == 0)
        {
            printf("%s", SHELL.shell_path);
        }
        else
        {
            printf("%s", args[i]);
        }

        if (args[i + 1] != NULL)
            printf(" ");
    }
    printf("\n");
    fflush(stdout);
}

int main()
{
    char *args[MAX_ARGS];
    char input[MAX_INPUT];

    init_shell();
    init_job();
    setup_signals();

    while (1)
    {
        display_prompt();

        // command line input
        if (!fgets(input, sizeof(input), stdin))
        {
            printf("\n");
            break;
        }

        // replace last enter key
        size_t len = strlen(input);
        if (len > 0 && input[len - 1] == '\n')
        {
            input[len - 1] = '\0';
        }

        // skip empty input
        if (input[0] == '\0')
            continue;

        // check if piped commands exists
        if (has_pipe(input)){
            int cmd_count = split_pipe(input, args);
            SHELL.last_exit_status = execute_pipe(args, cmd_count);
            continue;
        }

        // tokenize input
        int n = parse_input(input, args);
        if (n == 0)
            continue;

        // Exit builtin command
        if (strcmp(args[0], "exit") == 0)
        {
            SHELL.last_exit_status = 0;
            return SUCCESS;
        }
        // clear screen builtin command
        else if (strcmp(args[0], "clear") == 0)
        {
            printf("\033[2J");
            printf("\033[3J");
            printf("\033[H");
            fflush(stdout);
            SHELL.last_exit_status = 0;
        }
        // Prompt update
        else if (!strncmp(args[0], "PS1=", 4))
        {
            strcpy(SHELL.prompt, args[0] + 4);
            continue;
        }
        // External command
        else if (check_command_type(args[0]) == EXTERNAL)
        {
            SHELL.last_exit_status = execute_external_commands(args);
        }
        // Internal command
        else if (check_command_type(args[0]) == INTERNAL)
        {
            SHELL.last_exit_status = execute_internal_commands(args);
        }
        // Echo command
        else if (check_command_type(args[0]) == ECHO)
        {
            echo(args);
            SHELL.last_exit_status = 0;
        }
        // unknown command
        else
        {
            fprintf(stderr, "Command '%s' not found.\n", args[0]);
        }
    }

    return 0;
}

// initialize shell varibles
void init_shell()
{
    SHELL.shell_pid = getpid();
    SHELL.last_exit_status = 0;
    SHELL.fg_pid = -1;
    strcpy(SHELL.prompt, "minishell$ ");

    char *ptr = getcwd(SHELL.shell_path, sizeof(SHELL.shell_path));
    strcat(ptr, "/minishell");
}

// Display shell prompt
void display_prompt()
{
    printf("%s", SHELL.prompt);
    fflush(stdout);
}
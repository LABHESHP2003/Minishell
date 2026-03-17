#include "main.h"

void sigint_handler(int sig)
{
    // if foreground process then react to ctrl + c
    if (SHELL.fg_pid > 0)
    {
        kill(SHELL.fg_pid, SIGINT);
    }
    else// else display prompt
    {
        printf("\n");
        display_prompt();
    }
}

void sigtstp_handler(int sig)
{
    // if foreground process then react to ctrl + z
    if (SHELL.fg_pid > 0)
    {
        kill(SHELL.fg_pid, SIGTSTP);
    }
    else
    {
        printf("\n");
        display_prompt();
    }
}

void setup_signals(void)
{
    struct sigaction sa;

    memset(&sa, 0, sizeof(sa));
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;
    sa.sa_handler = sigint_handler;
    sigaction(SIGINT, &sa, NULL);

    sa.sa_handler = sigtstp_handler;
    sigaction(SIGTSTP, &sa, NULL);
}
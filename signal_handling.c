/***************************************************************************************************************************************************
* Author        : Labhesh Patil
* Date          : Fri Feb 06 2026
* File          : signal_handling.c
* Title         : Signal handling for process control
* Description   : Sets up signal handlers for SIGINT (Ctrl+C), SIGTSTP (Ctrl+Z), and SIGCHLD (child termination).
*                 Forwards keyboard signals to foreground processes when active, otherwise re-displays the prompt.
*                 The SIGCHLD handler automatically cleans up terminated background processes and displays their
*                 exit status, ensuring proper job management without zombie processes.
****************************************************************************************************************************************************/

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

// set up signal handlers
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
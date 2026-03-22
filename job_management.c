/***************************************************************************************************************************************************
* Author        : Labhesh Patil
* Date          : Fri Feb 06 2026
* File          : job_management.c
* Title         : Background job management system
* Description   : Manages background and stopped processes using a singly linked list. Provides functions to
*                 insert new jobs, delete terminated jobs, find stopped jobs, update job states, and refresh
*                 the job list by checking process statuses. Displays job information including PID and state
*                 (Running/Stopped) for the jobs command.
****************************************************************************************************************************************************/

#include "main.h"

job_t *head = NULL;

// Initialize job list
void init_job()
{
    head = NULL;
}

// insert job into linked list
void insert_job(pid_t pid, job_state_t state)
{
    job_t *new_job = malloc(sizeof(job_t));
    new_job->pid = pid;
    new_job->state = state;
    new_job->next = head;
    head = new_job;
}

// remove job from list
void delete_job(pid_t pid)
{
    job_t *curr = head, *prev = NULL;
    while (curr)
    {
        if (curr->pid == pid)
        {
            if (prev)
                prev->next = curr->next;
            else
                head = curr->next;
            free(curr);
            return;
        }
        prev = curr;
        curr = curr->next;
    }
}

// find first stopped job
job_t *find_stopped_job()
{
    job_t *curr = head;
    while (curr)
    {
        if (curr->state == STOPPED)
            return curr;
        curr = curr->next;
    }
    return NULL;
}

// mark job as running
void mark_job_running(pid_t pid)
{
    job_t *curr = head;
    while (curr)
    {
        if (curr->pid == pid)
        {
            curr->state = RUNNING;
            return;
        }
        curr = curr->next;
    }
}

// mark job as stopped
void mark_job_stopped(pid_t pid)
{
    job_t *curr = head;
    while (curr)
    {
        if (curr->pid == pid)
        {
            curr->state = STOPPED;
            return;
        }
        curr = curr->next;
    }
}

// update job states
void refresh_jobs()
{
    int status;
    job_t *curr = head;
    job_t *prev = NULL;

    while (curr)
    {
        pid_t ret = waitpid(curr->pid, &status, WNOHANG | WUNTRACED);
        if (ret > 0)
        {
            if (WIFSTOPPED(status))
                curr->state = STOPPED;
            else
            {
                job_t *tmp = curr;
                if (prev)
                    prev->next = curr->next;
                else
                    head = curr->next;
                curr = curr->next;
                free(tmp);
                continue;
            }
        }
        prev = curr;
        curr = curr->next;
    }
}

// Print jobs list
void display_jobs()
{
    refresh_jobs();
    job_t *curr = head;
    if(!curr){
        printf("jobs: no jobs available\n");
        return;
    }
    while (curr)
    {
        printf("[%d] %s\n", curr->pid,
               curr->state == STOPPED ? "Stopped" : "Running");
        curr = curr->next;
    }
}
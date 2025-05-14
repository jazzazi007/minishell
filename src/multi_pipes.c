#include "../include/pipex.h"
#include <stdio.h>
#include <readline/readline.h>
#include <stdbool.h>

bool is_valid_pipe_syntax(char *ag)
{
    int i = 0;
    bool found_command = false;

    while (ag[i])
    {
        while (ag[i] && ag[i] == ' ')
            i++;
        if ((ag[i] == '|' || ag[i] == ';') && !found_command)
            return false;
        
        if ((ag[i] == '|' || ag[i] == ';'))
        {
            i++;
            while (ag[i] && ag[i] == ' ')
                i++;
            if (ag[i] == '|' || ag[i] == ';' || ag[i] == '\0')
                return false;
            found_command = false;
        }
        else if (ag[i] && ag[i] != ' ')
        {
            found_command = true;
            i++;
        }
        else if (ag[i])
            i++;
    }
    return true;
}

void check_pipes_forks(char *ag, char **env)
{
    if (!is_valid_pipe_syntax(ag))
    {
        printf("Error: Invalid pipe or semicolon syntax\n");
        return;
    }

    int i = 0;
    int pipe_count = 0;
    pid_t *child_pids;
    int **pipe_fds;

    while (ag[i])
    {
        if (ag[i] == '|')
            pipe_count++;
        i++;
    }
    pipe_fds = malloc(sizeof(int *) * pipe_count);
    if (!pipe_fds)
    {
        perror("Error allocating memory for pipes");
        return;
    }

    i = 0;
    while (i < pipe_count)
    {
        pipe_fds[i] = malloc(sizeof(int) * 2);
        if (!pipe_fds[i])
        {
            perror("Error allocating memory for pipe");
            while (--i >= 0)
                free(pipe_fds[i]);
            free(pipe_fds);
            return;
        }
        if (pipe(pipe_fds[i]) == -1)
        {
            perror("Pipe creation failed");
            while (i >= 0)
                free(pipe_fds[i]);
            free(pipe_fds);
            return;
        }
        i++;
    }

    child_pids = malloc(sizeof(pid_t) * (pipe_count + 1));
    if (!child_pids)
    {
        perror("Error allocating memory for child PIDs");
        i = 0;
        while (i < pipe_count)
        {
            free(pipe_fds[i]);
            i++;
        }
        free(pipe_fds);
        return;
    }

    i = 0;
    while (i <= pipe_count)
    {
        child_pids[i] = fork();
        if (child_pids[i] == -1)
        {
            perror("Fork failed");
            int j = 0;
            while (j < pipe_count)
            {
                free(pipe_fds[j]);
                j++;
            }
            free(pipe_fds);
            free(child_pids);
            return;
        }

        if (child_pids[i] == 0)
        {
            if (i > 0)  
                dup2(pipe_fds[i - 1][0], STDIN_FILENO);
            if (i < pipe_count)  // Not last command
                dup2(pipe_fds[i][1], STDOUT_FILENO);

            int j = 0;
            while (j < pipe_count)
            {
                close(pipe_fds[j][0]);
                close(pipe_fds[j][1]);
                j++;
            }

            printf("Child process %d ready for command execution\n", i);
            exit(0);
        }
        i++;
    }


    i = 0;
    while (i < pipe_count)
    {
        close(pipe_fds[i][0]);
        close(pipe_fds[i][1]);
        i++;
    }

    i = 0;
    while (i <= pipe_count)
    {
        waitpid(child_pids[i], NULL, 0);
        i++;
    }

    i = 0;
    while (i < pipe_count)
    {
        free(pipe_fds[i]);
        i++;
    }
    free(pipe_fds);
    free(child_pids);
}

int main()
{
    char *readline_input;

    while (1)
    {
        readline_input = readline("Enter command: ");
        if (!readline_input)
        {
            perror("Error reading input");
            break;
        }
        check_pipes_forks(readline_input, NULL);
        free(readline_input);
    }
    return (0);
}
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

void fork_operate(int fd_in, char *cmd, char **env, int *pipe_fd)
{
    (void)env; // Placeholder for command execution
    close(pipe_fd[0]);
    if (fd_in != STDIN_FILENO)
    {
        dup2(fd_in, STDIN_FILENO);
        close(fd_in);
    }
    dup2(pipe_fd[1], STDOUT_FILENO);
    close(pipe_fd[1]);
    exit(cmd_exec(cmd, env));
   
}

static int count_pipes(char *ag)
{
    int i = 0;
    int pipe_count = 0;

    while (ag[i])
    {
        if (ag[i] == '|')
            pipe_count++;
        i++;
    }
    return (pipe_count);
}

static int **init_pipes(int pipe_count)
{
    int **pipe_fds;
    int i;

    pipe_fds = malloc(sizeof(int *) * pipe_count);
    if (!pipe_fds)
        return (NULL);
    i = 0;
    while (i < pipe_count)
    {
        pipe_fds[i] = malloc(sizeof(int) * 2);
        if (!pipe_fds[i])
        {
            while (--i >= 0)
                free(pipe_fds[i]);
            free(pipe_fds);
            return (NULL);
        }
        if (pipe(pipe_fds[i]) == -1)
        {
            while (i >= 0)
                free(pipe_fds[i]);
            free(pipe_fds);
            return (NULL);
        }
        i++;
    }
    return (pipe_fds);
}

static pid_t *init_child_pids(int pipe_count, int **pipe_fds)
{
    pid_t *child_pids;

    child_pids = malloc(sizeof(pid_t) * (pipe_count + 1));
    if (!child_pids)
    {
        int i = 0;
        while (i < pipe_count)
        {
            free(pipe_fds[i]);
            i++;
        }
        free(pipe_fds);
        return (NULL);
    }
    return (child_pids);
}

static void cleanup_resources(int **pipe_fds, pid_t *child_pids, int pipe_count)
{
    int i;

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

void check_pipes_forks(char *ag, char **env)
{
    if (!is_valid_pipe_syntax(ag))
    {
        printf("Error: Invalid pipe or semicolon syntax\n");
        return;
    }

    int pipe_count = count_pipes(ag);
    int **pipe_fds = init_pipes(pipe_count);
    if (!pipe_fds)
        return;

    pid_t *child_pids = init_child_pids(pipe_count, pipe_fds);
    if (!child_pids)
        return;

    int i = 0;
    while (i <= pipe_count)
    {
        child_pids[i] = fork();
        if (child_pids[i] == -1)
        {
            perror("Fork failed");
            cleanup_resources(pipe_fds, child_pids, pipe_count);
            return;
        }

        if (child_pids[i] == 0)
        {
            int fd_in = STDIN_FILENO;
            if (i > 0)
                fd_in = pipe_fds[i - 1][0];

            if (i < pipe_count)
                fork_operate(fd_in, ag, env, pipe_fds[i]);
            else
            {
                if (fd_in != STDIN_FILENO)
                {
                    dup2(fd_in, STDIN_FILENO);
                    close(fd_in);
                }
                exit(cmd_exec(ag, env));
            }
            exit(1);
        }
        i++;
    }

    cleanup_resources(pipe_fds, child_pids, pipe_count);
}

void print_shell_banner(void)
{
    printf("\n");
    printf("\033[1;34m    ███████╗██╗  ██╗███████╗██╗     ██╗     ██╗  ██╗\n");
    printf("\033[1;34m    ██╔════╝██║  ██║██╔════╝██║     ██║     ╚██╗██╔╝\n");
    printf("\033[1;34m    ███████╗███████║█████╗  ██║     ██║      ╚███╔╝ \n");
    printf("\033[1;34m    ╚════██║██╔══██║██╔══╝  ██║     ██║      ██╔██╗ \n");
    printf("\033[1;34m    ███████║██║  ██║███████╗███████╗███████╗██╔╝ ██╗\n");
    printf("\033[1;34m    ╚══════╝╚═╝  ╚═╝╚══════╝╚══════╝╚══════╝╚═╝  ╚═╝\n");
    printf("\033[0m\n");
}

int main(int ac, char **av, char **envp)
{
    (void)ac;
    (void)av;
    char *readline_input;
    check_pipes_forks("clear", envp);
    print_shell_banner();
    while (1)
    {
        readline_input = readline("\001\033[1;36m\002SHELLX\001\033[1;32m\002~%\001\033[0m\002 ");
        if (!readline_input)
        {
            fprintf(stderr,"exit\n");
            break;
        }
        check_pipes_forks(readline_input, envp);
        free(readline_input);
    }
    return (0);
}
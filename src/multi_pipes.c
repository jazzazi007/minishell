#include "../include/minishell.h"

char *get_command(char *input, int position)
{
    char *start = input;
    int pipe_count = 0;
    
    while (*start && pipe_count < position)
    {
        if (*start == '|')
            pipe_count++;
        start++;
    }
    
    while (*start && (*start == ' ' || *start == '\t'))
        start++;
    
    if (!*start)
        return NULL;
    
    char *end = start;
    while (*end && *end != '|')
        end++;
    
    int len = end - start;
    char *cmd = malloc(len + 1);
    if (!cmd)
        return NULL;
    
    strncpy(cmd, start, len);
    cmd[len] = '\0';
    
    while (len > 0 && (cmd[len - 1] == ' ' || cmd[len - 1] == '\t'))
        cmd[--len] = '\0';
    
    return cmd;
}

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
    close(pipe_fd[0]);

    if (fd_in != STDIN_FILENO)
    {
        dup2(fd_in, STDIN_FILENO);
        close(fd_in);
    }

    dup2(pipe_fd[1], STDOUT_FILENO);
    close(pipe_fd[1]);
    
    cmd_exec(cmd, env);
    exit(1);
}

int count_pipes(char *ag)
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

 int **init_pipes(int pipe_count)
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

 pid_t *init_child_pids(int pipe_count, int **pipe_fds)
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

void cleanup_resources(int **pipe_fds, pid_t *child_pids, int pipe_count)
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
            char *command;

            int j = 0;
            while (j < pipe_count)
            {
                if (j != i - 1) 
                    close(pipe_fds[j][0]);
                if (j != i)      
                    close(pipe_fds[j][1]);
                j++;
            }

            if (i > 0)
                fd_in = pipe_fds[i - 1][0];

            command = get_command(ag, i);
            if (!command)
                exit(1);

            if (i < pipe_count)
                fork_operate(fd_in, command, env, pipe_fds[i]);
            else
            {
                if (fd_in != STDIN_FILENO)
                {
                    dup2(fd_in, STDIN_FILENO);
                    close(fd_in);
                }
                cmd_exec(command, env);
                exit(1);
            }
            free(command);
            exit(1);
        }
        i++;
    }

    cleanup_resources(pipe_fds, child_pids, pipe_count);
}
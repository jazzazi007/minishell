#include "../../include/minishell.h"

int env(t_minishell *shell)
{
    char **envp = shell->envp; // Assuming envp contains the environment variables
    int i = 0;

    if (!envp || !envp[0])
    {
        printf("No environment variables set.\n");
        return 1;
    }

    while (envp[i])
    {
        printf("%s\n", envp[i]);
        i++;
    }
    return 0;
}
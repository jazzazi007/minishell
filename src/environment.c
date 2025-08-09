#include "../include/minishell.h"

int ft_envp_size(char **envp)
{
	int size;

	if (!envp || !*envp)
		return (0);
	size = 0;
	while (envp[size])
		size++;
	return (size);
}

char **ft_create_envp(char **envp)
{
	int i;
	int size;
	char **envp_c;

	if (!envp || !*envp)
		return (NULL);
	i = 0;
	size = ft_envp_size(envp);
	envp_c = malloc(sizeof(char *) * (size + 1));
	if (!envp_c)
		return (NULL);
	while (envp[i])
	{
		envp_c[i] = ft_strdup(envp[i]);
		if (!envp_c[i])
		{
			while (--i >= 0)
				free(envp_c[i]);
			free(envp_c);
			return (NULL);
		}
		i++;
	}
	envp_c[i] = NULL;
	return (envp_c);
}

void ft_free_envp(char **envp)
{
	int i;

	if (!envp)
		return;
	i = 0;
	while (envp[i])
	{
		free(envp[i]);
		i++;
	}
	free(envp);
}

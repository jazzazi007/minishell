#include "minishell.h"

int	main(void)
{
	char	*input;

	while (1)
	{
		input = readline("minishell> ");
		if (!input)
		{
			printf("exit\n");
			break ;
		}
		if (*input)
			add_history(input);
		if (strcmp(input, "exit") == 0)
		{
			free(input);
			break ;
		}
		free(input);
	}
	return (0);
}
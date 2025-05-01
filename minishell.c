#include "minishell.h"

int	main(void)
{
	char	*input;

	while (1)
	{
		input = readline("\001\033[31m\002minishell 🔥 > \001\033[0m\002");
		if (!input)
		{
			printf("exit\n");
			break ;
		}
		if (*input)
			add_history(input);
		if (ft_strcmp(input, "exit") == 0)
		{
			free(input);
			break ;
		}
		free(input);
	}
	return (0);
}
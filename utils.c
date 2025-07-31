#include "minishell.h"

void ft_build_prompt(t_shell *shell)
{
	char	*cwd;
	char	*tmp;

	if (shell->prompt)
		free(shell->prompt);
	shell->prompt = NULL;
	cwd = getcwd(NULL, 0); // Get current working directory - dinamic allocation
	if (!cwd)
		perror("Getcwd failed");
	shell->prompt = ft_strjoin("\001\033[0;36m\033[1m\002", cwd); //formato negrita y cyan
	tmp = shell->prompt;
	shell->prompt = ft_strjoin(tmp, " > \001\033[0m\002"); //restauramos formato normal
	free(tmp); 
	if (cwd)
		free(cwd); //debemos liberar la memoria de cwd
	//Nota: también deberemos liberar shell->prompt al final del programa para evitar fugas de memoria.

	//IDEA: esto recalcula siempre el cwd, más adelante mirar que solo se haga esto si hay un cd exitoso.
}

void	ft_add_history(t_hist **lst, char *new)
{
	t_hist *node = malloc(sizeof(t_hist));
	if (!node)
		return ;
	node->line = strdup(new);
	node->next = NULL;

	if (*lst == NULL)
	{
		node->prev = NULL;
		*lst = node;
	}
	else
	{
		t_hist *last = *lst;
		while (last->next)
			last = last->next;
		last->next = node;
		node->prev = last;
	}
}

void print_history(t_hist **lst)
{
	t_hist *current;
	int i = 1;

	if (!lst || !*lst)
		return ;

	current = *lst;
	while (current)
	{
		printf("%d  %s\n", i, current->line);
		current = current->next;
		i++;
	}
}

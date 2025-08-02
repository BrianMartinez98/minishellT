/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jarregui <jarregui@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/02 00:12:49 by jarregui          #+#    #+#             */
/*   Updated: 2025/08/03 00:17:33 by jarregui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void ft_build_prompt(t_shell *shell)
{
	char	*tmp;

	if (shell->prompt)
		free(shell->prompt);
	if (shell->cwd)
		free(shell->cwd);
	shell->prompt = NULL;
	shell->cwd = getcwd(NULL, 0); // Get current working directory - dinamic allocation
	if (!shell->cwd)
		perror("Getcwd failed");
	shell->prompt = ft_strjoin("\001\033[0;36m\033[1m\002", shell->cwd); //formato negrita y cyan
	tmp = shell->prompt;
	shell->prompt = ft_strjoin(tmp, " > \001\033[0m\002"); //restauramos formato normal
	free(tmp); 

	//IDEA: esto recalcula siempre el cwd, más adelante mirar que solo se haga esto si hay un cd exitoso.
}


//NOTA: esto ya no vale y habría que borralo:
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

//Nota: esto tampoco vale y habría que borrarlo:
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

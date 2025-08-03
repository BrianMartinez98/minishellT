/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jarregui <jarregui@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/02 00:12:49 by jarregui          #+#    #+#             */
/*   Updated: 2025/08/03 17:10:57 by jarregui         ###   ########.fr       */
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

void	ft_add_history(char *line, t_shell *shell)
{
	t_hist	*new_hist;
	t_hist	*last_hist;

	add_history(line);
	new_hist = malloc(sizeof(t_hist));
	if (!new_hist)
	{
		perror("Memory allocation failed for history node");
		return ;
	}
	new_hist->line = ft_strdup(line);
	new_hist->next = NULL;
	if (!shell->hist)
		shell->hist = new_hist;
	else
	{
		last_hist = shell->hist;
		while (last_hist->next)
			last_hist = last_hist->next;
		last_hist->next = new_hist;
		new_hist->prev = last_hist;
	}
}

void	ft_print_history(t_shell *shell)
{
	size_t	i;
	t_hist	*current_hist;

	i = 1;
	current_hist = shell->hist;
	while (current_hist)
	{
		printf("%zu  %s\n", i++, current_hist->line);
		current_hist = current_hist->next;
	}
}

void	ft_free_history(t_shell *shell)
{
	t_hist	*next;

	while (shell->hist)
	{
		next = shell->hist->next;
		free(shell->hist->line);   // ← liberas la línea del comando
		free(shell->hist);         // ← liberas el nodo
		shell->hist = next;
	}
}
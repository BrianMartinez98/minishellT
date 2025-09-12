/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_exit.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jarregui <jarregui@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/09 15:00:40 by jarregui          #+#    #+#             */
/*   Updated: 2025/09/12 12:55:12 by jarregui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	ft_free_array(char ***array)
{
	size_t	i;

	if (!array || !*array)
		return ;
	i = 0;
	while ((*array)[i])
	{
		free((*array)[i]);
		i++;
	}
	free(*array);
	*array = NULL;
}

void	ft_free_matrix(char ****matrix)
{
	size_t	i;

	if (!matrix || !*matrix)
		return ;
	i = 0;
	while ((*matrix)[i])
	{
		ft_free_array(&(*matrix)[i]);
		i++;
	}
	free(*matrix);
	*matrix = NULL;
}

int	ft_exit_shell(t_shell *shell)
{
	if (shell->prompt)
		free(shell->prompt);
	if (shell->cwd)
		free(shell->cwd);
	if (shell->hist)
		ft_free_history(shell);
	if (shell->env)
		ft_free_env(shell);
	if (shell->line)
		ft_free_line(shell);
	if (shell->tokens)
		ft_free_array(&shell->tokens);
	if (shell->clean_args)
		ft_free_array(&shell->clean_args);
	if (shell->child_args)
		ft_free_array(&shell->child_args);
	if (shell->cmds)
		ft_free_matrix(&shell->cmds);
	ft_restore_term_settings(shell);
	if (DEBUG && shell->eof)
		write(1, "\n	Pulsada tecla Ctrl+D (EOF)\n", 30);
	printf("\033[1;32mExiting minishell...\033[0m\n");
	exit(0);
}

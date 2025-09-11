/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_exit.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jarregui <jarregui@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/09 15:00:40 by jarregui          #+#    #+#             */
/*   Updated: 2025/09/11 14:44:13 by jarregui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	ft_free_array(char **array)
{
	size_t	i;

	if (!array)
		return ;
	i = 0;
	while (array[i])
	{
		free(array[i]);
		i++;
	}
	free(array);
}

void	ft_free_matrix(char ***matrix)
{
	size_t	i;

	if (!matrix)
		return ;
	i = 0;
	while (matrix[i])
	{
		ft_free_array(matrix[i]);
		i++;
	}
	free(matrix);
}


int	ft_exit_shell(t_shell *shell)
{
	if (shell->prompt)
		free(shell->prompt);
	if (shell->cwd)
		free(shell->cwd);
	ft_free_history(shell);
	ft_free_env(shell);
	ft_free_line(shell);
	ft_free_array(shell->tokens);
	ft_free_array(shell->clean_args);
	ft_free_matrix(shell->cmds);
	ft_restore_term_settings(shell);
	if (DEBUG && shell->eof)
		write(1, "\n	Pulsada tecla Ctrl+D (EOF)\n", 30);
	printf("\033[1;32mExiting minishell...\033[0m\n");
	exit(0);
}

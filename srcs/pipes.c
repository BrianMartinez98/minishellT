/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jarregui <jarregui@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/10 19:14:30 by jarregui          #+#    #+#             */
/*   Updated: 2025/09/12 z13:44:03 by jarregui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	ft_execute_pipes(t_shell *shell)
{
	int		fd[2];
	char	**tokens;
	int		i;

	i = 0;
	while (shell->cmds[i])
	{
		if (shell->cmds[i + 1])
		{
			if (pipe(fd) == -1)
				handle_error(PIPES, shell);
			shell->stdout_save = fd[1];
		}
		filter_args(shell->cmds[i], &tokens, shell); //FIx
		ft_execute(tokens, shell, shell->cmds[i]);
		free(tokens);
		if (shell->cmds[i + 1])
		{
			close(fd[1]);
			shell->stdin_save = fd[0];
		}
		i++;
	}
}

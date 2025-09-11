/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jarregui <jarregui@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/11 13:28:43 by jarregui          #+#    #+#             */
/*   Updated: 2025/09/11 13:51:58 by jarregui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	ft_redirect(t_shell *shell, int i)
{
	shell->fd = open(shell->tokens[i], O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (shell->fd < 0)
	{
		perror(shell->tokens[i]);
		return (-1);
	}
	dup2(shell->fd, STDOUT_FILENO);
	close(shell->fd);
	return (0);
}

int	ft_adding(t_shell *shell, int i)
{
	shell->fd = open(shell->tokens[i], O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (shell->fd < 0)
	{
		perror(shell->tokens[i]);
		return (-1);
	}
	dup2(shell->fd, STDOUT_FILENO);
	close(shell->fd);
	return (0);
}

int	ft_left(t_shell *shell, int i)
{
	shell->fd = open(shell->tokens[i], O_RDONLY);
	if (shell->fd < 0)
	{
		perror(shell->tokens[i]);
		return (-1); 
	}
	dup2(shell->fd, STDIN_FILENO);
	close(shell->fd);
	return (0);
}

int	ft_leftleft(t_shell *shell, int i)
{
	int pipefd[2];
	if (pipe(pipefd) == -1)
	{
		perror("pipe");
		return -1;
	}
	while (1)
	{
		write(1, "> ", 2);
		shell->nread = getline(&shell->line2, &shell->len, stdin);
		if (shell->nread == -1)
			break ;
		shell->line[shell->nread-1] = '\0';
		if (strcmp(shell->line2, shell->tokens[i]) == 0)
			break ;
		write(pipefd[1], shell->line2, strlen(shell->line2));
		write(pipefd[1], "\n", 1);
	}
	free(shell->line2);
    shell->line2 = NULL;
	close(pipefd[1]);
	dup2(pipefd[0], STDIN_FILENO);
	close(pipefd[0]);
	return (0);
}

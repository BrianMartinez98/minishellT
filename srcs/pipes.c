/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jarregui <jarregui@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/10 19:14:30 by jarregui          #+#    #+#             */
/*   Updated: 2025/09/12 11:37:17 by jarregui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	count_pipes(t_shell *shell)
{
	int count = 0, i = 0;
	while (shell->tokens[i])
	{
		if (strcmp(shell->tokens[i], "|") == 0)
			count++;
		i++;
	}
	return count;
}

// Divide tokens en arrays de comandos separados por '|'
static char	***split_by_pipe(int *num_cmds, t_shell *shell)
{
	int pipes = count_pipes(shell);
	*num_cmds = pipes + 1;
	ft_free_matrix(&shell->cmds);
	shell->cmds = malloc(sizeof(char **) * (*num_cmds + 1));
	int i = 0, j = 0, k = 0, start = 0;
	while (shell->tokens[i])
	{
		if (strcmp(shell->tokens[i], "|") == 0)
		{
			shell->cmds[k] = malloc(sizeof(char *) * (i - start + 1));
			for (j = start; j < i; j++)
				shell->cmds[k][j - start] = shell->tokens[j];
			shell->cmds[k][i - start] = NULL;
			k++;
			start = i + 1;
		}
		i++;
	}
	shell->cmds[k] = malloc(sizeof(char *) * (i - start + 1));
	for (j = start; j < i; j++)
		shell->cmds[k][j - start] = shell->tokens[j];
	shell->cmds[k][i - start] = NULL;
	shell->cmds[k + 1] = NULL;
	return (shell->cmds);
}

int	ft_execute_pipes(t_shell *shell)
{
	int num_cmds, i, status;
	int fd[2], in_fd = 0, pid;
	int has_pipe = 0;

	for (i = 0; shell->tokens[i]; i++)
		if (strcmp(shell->tokens[i], "|") == 0)
			has_pipe = 1;
	if (!has_pipe)
		return (ft_execute(shell));
	shell->cmds = split_by_pipe(&num_cmds, shell);
	for (i = 0; i < num_cmds; i++)
	{
		if (i < num_cmds - 1)
			pipe(fd);
		pid = fork();
		if (pid == 0)
		{
			if (i > 0)
			{
				dup2(in_fd, STDIN_FILENO);
				close(in_fd);
			}
			if (i < num_cmds - 1)
			{
				close(fd[0]);
				dup2(fd[1], STDOUT_FILENO);
				close(fd[1]);
			}
			handle_redirections(shell);
			filter_args(&shell->cmds[i], &shell->clean_args);
			execvp(shell->clean_args[0], shell->clean_args);
			perror(shell->clean_args[0]);
			free(shell->clean_args);
			exit(127);
		}
		if (i > 0)
			close(in_fd);
		if (i < num_cmds - 1)
		{
			close(fd[1]);
			in_fd = fd[0];
		}
	}
	for (i = 0; i < num_cmds; i++)
		wait(&status);
	return WEXITSTATUS(status);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jarregui <jarregui@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/07 18:41:31 by jarregui          #+#    #+#             */
/*   Updated: 2025/11/07 18:55:06 by jarregui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	handle_sig(t_shell *shell, char *msg, int status)
{
	write(1, msg, ft_strlen(msg));
	shell->last_status = status;
}

void	ft_wait_children(t_shell *shell, pid_t *pids, int n)
{
	int	j;
	int	sig;

	j = 0;
	shell->status = 0;
	while (j < n)
	{
		if (waitpid(pids[j], &shell->status, 0) == -1)
			perror("waitpid");
		else if (WIFSIGNALED(shell->status))
		{
			sig = WTERMSIG(shell->status);
			if (sig == SIGINT)
				handle_sig(shell, "\n", 130);
			else if (sig == SIGQUIT)
				handle_sig(shell, "Quit (core dumped)\n", 131);
		}
		else if (WIFEXITED(shell->status))
			shell->last_status = WEXITSTATUS(shell->status);
		j++;
	}
}

void	restore_stdio(t_shell *shell)
{
	if (shell->stdin_save != STDIN_FILENO)
	{
		dup2(shell->stdin_save, STDIN_FILENO);
		close(shell->stdin_save);
		shell->stdin_save = STDIN_FILENO;
	}
	if (shell->stdout_save != STDOUT_FILENO)
	{
		dup2(shell->stdout_save, STDOUT_FILENO);
		close(shell->stdout_save);
		shell->stdout_save = STDOUT_FILENO;
	}
	if (!shell->builtin)
	{
		if (WIFEXITED(shell->status))
			shell->last_status = WEXITSTATUS(shell->status);
		else if (WIFSIGNALED(shell->status))
			shell->last_status = 128 + WTERMSIG(shell->status);
	}
	shell->builtin = 0;
}

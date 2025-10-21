/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jarregui <jarregui@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/02 19:47:59 by jarregui          #+#    #+#             */
/*   Updated: 2025/10/21 09:47:33 by jarregui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	pid_child(char **tokens, char **cmd, t_shell *shell)
{
	char	**paths;
	char	*pathname;

	ft_setup_signals_child();
	if (shell->stdin_save != STDIN_FILENO)
		dup2(shell->stdin_save, STDIN_FILENO);
	if (shell->stdout_save != STDOUT_FILENO)
		dup2(shell->stdout_save, STDOUT_FILENO);
	handle_redirections(cmd, shell);
	paths = paths_finder(shell->env);
	pathname = command_finder(tokens, paths);
	if (!pathname && !is_builtin(tokens))
	{
		printf("minishell: Error: Command not found!\n");
		exit(0);
	}
	execve(pathname, tokens, shell->env);
	perror(tokens[0]);
	exit(127);
}

static int	ft_execute(char **tokens, char **cmd, t_shell *shell)
{
	pid_t	pid;
	int		status;

	if (!tokens || !tokens[0])
		return (0);
	pid = fork();
	if (pid < 0)
	{
		perror("fork");
		shell->last_status = 1;
		return (1);
	}
	if (pid == 0)
		pid_child(tokens, cmd, shell);
	else
	{
		waitpid(pid, &status, 0);
		if (WIFEXITED(status))
			shell->last_status = WEXITSTATUS(status);
		else if (WIFSIGNALED(status))
			shell->last_status = 128 + WTERMSIG(status);
	}
	return (shell->last_status);
}

static void	execute_command(t_shell *shell, char **cmd,
	char **tokens, int has_next)
{
	int	saved_stdin;
	int	saved_stdout;

	if (is_builtin(tokens) && !has_next)
	{
		saved_stdin = dup(STDIN_FILENO);
		saved_stdout = dup(STDOUT_FILENO);
		handle_redirections(cmd, shell);
		ft_execute_builtin(tokens, shell);
		dup2(saved_stdin, STDIN_FILENO);
		dup2(saved_stdout, STDOUT_FILENO);
		close(saved_stdin);
		close(saved_stdout);
	}
	else
		ft_execute(tokens, cmd, shell);
}

void	ft_execute_pipes(t_shell *shell)
{
	int		fd[2];
	char	**tokens;
	int		i;
	int		has_next;

	i = 0;
	while (shell->cmds[i])
	{
		has_next = (shell->cmds[i + 1] != NULL);
		if (shell->cmds[i + 1])
		{
			if (pipe(fd) == -1)
				handle_error(PIPES, shell);
			shell->stdout_save = fd[1];
		}
		filter_args(shell->cmds[i], &tokens, shell);
		execute_command(shell, shell->cmds[i], tokens, has_next);
		free(tokens);
		if (shell->cmds[i + 1])
		{
			close(fd[1]);
			shell->stdin_save = fd[0];
		}
		i++;
	}
}

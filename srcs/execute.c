/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jarregui <jarregui@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/02 19:47:59 by jarregui          #+#    #+#             */
/*   Updated: 2025/11/07 15:14:59 by jarregui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	close_fds_except(int keep1, int keep2, int keep3)
{
	long	max_fd;
	int		fd;

	max_fd = sysconf(_SC_OPEN_MAX);
	if (max_fd == -1)
		max_fd = 1024;
	for (fd = 3; fd < (int)max_fd; fd++)
	{
		if (fd == keep1 || fd == keep2 || fd == keep3)
			continue;
		close(fd);
	}
}

void	redirection_fail(int in_fd, int out_fd)
{
	if (in_fd != -1 && in_fd != STDIN_FILENO)
	{
		if (dup2(in_fd, STDIN_FILENO) == -1)
		{
			close(in_fd);
			perror("dup2 in_fd");
			exit(1);
		}
	}
	if (out_fd != -1 && out_fd != STDOUT_FILENO)
	{
		if (dup2(out_fd, STDOUT_FILENO) == -1)
		{
			close(out_fd);
			perror("dup2 out_fd");
			exit(1);
		}
	}
}

static void	pid_child(char **tokens, char **cmd, t_shell *shell, int in_fd, int out_fd)
{
	char	*pathname;

	ft_setup_signals_child();
	redirection_fail(in_fd, out_fd);
	if (shell->heredoc_fd != -1)
	{
		dup2(shell->heredoc_fd, STDIN_FILENO);
		close(shell->heredoc_fd);
		shell->heredoc_fd = -1;
	}
	handle_redirections(cmd, shell);
	close_fds_except(STDIN_FILENO, STDOUT_FILENO, STDERR_FILENO);
	if (is_builtin(tokens))
	{
		ft_execute_builtin(tokens, shell);
		exit(shell->last_status % 256);
	}
	pathname = ft_execve_prep(tokens, shell);
	execve(pathname, tokens, shell->env);
	perror(tokens[0]);
	if (errno == EACCES)
		exit(126);
	exit(127);
}

static pid_t	fork_and_exec(char **tokens, char **cmd, t_shell *shell,
	int in_fd, int out_fd)
{
	pid_t	pid;

	pid = fork();
	if (pid < 0)
	{
		error_custom(shell, 1, "fork error", NULL);
		return (-1);
	}
	if (pid == 0)
	{
		shell->is_child = 1;
		pid_child(tokens, cmd, shell, in_fd, out_fd);
	}
	else
		shell->is_child = 0;
	return (pid);
}

pid_t	execute_command(t_shell *shell, char **cmd, char **tokens, int has_next, int in_fd, int out_fd)
{
	int		saved_stdin;
	int		saved_stdout;
	pid_t	pid;

	if (!tokens || !tokens[0])
		return (-2);
	if (is_builtin(tokens) && !has_next && in_fd == -1)
	{
		shell->builtin = 1;
		saved_stdin = dup(STDIN_FILENO);
		saved_stdout = dup(STDOUT_FILENO);
		if (saved_stdin == -1 || saved_stdout == -1)
			return (error_check(saved_stdin, saved_stdout));
		dup2(STDIN_FILENO, shell->stdin_save);
		dup2(STDOUT_FILENO, shell->stdout_save);
		handle_redirections(cmd, shell);
		ft_execute_builtin(tokens, shell);
		dup2(saved_stdin, STDIN_FILENO);
		dup2(saved_stdout, STDOUT_FILENO);
		close_saved_fd(saved_stdin, saved_stdout);
		return (-2);
	}
	else
		pid = fork_and_exec(tokens, cmd, shell, in_fd, out_fd);
	return (pid);
}

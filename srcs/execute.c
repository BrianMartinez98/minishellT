/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jarregui <jarregui@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/02 19:47:59 by jarregui          #+#    #+#             */
/*   Updated: 2025/11/07  by ChatGPT_fixed         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	check_path_errors(t_shell *shell, char *pathname)
{
	struct stat	st;

	if (stat(pathname, &st) == -1)
	{
		if (errno == ENOENT)
			error_custom(shell, 127, "no such file or directory:", pathname);
	}
	else if (S_ISDIR(st.st_mode))
		error_custom(shell, 126, "is a directory:", pathname);
	else if (access(pathname, X_OK) != 0)
		error_custom(shell, 126, "permission denied:", pathname);
}

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

static void	pid_child(char **tokens, char **cmd, t_shell *shell, int in_fd, int out_fd)
{
	char	**paths;
	char	*pathname;

	ft_setup_signals_child();
	if (in_fd != -1 && in_fd != STDIN_FILENO)
	{
		if (dup2(in_fd, STDIN_FILENO) == -1)
		{
			perror("dup2 in_fd");
			exit(1);
		}
	}
	if (out_fd != -1 && out_fd != STDOUT_FILENO)
	{
		if (dup2(out_fd, STDOUT_FILENO) == -1)
		{
			perror("dup2 out_fd");
			exit(1);
		}
	}
	if (in_fd != -1 && in_fd != STDIN_FILENO)
		close(in_fd);
	if (out_fd != -1 && out_fd != STDOUT_FILENO)
		close(out_fd);
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
		exit(shell->last_status & 0xFF);
	}

	paths = paths_finder(shell->env);
	pathname = command_finder(tokens, paths);
	free_paths(paths);

	if (DEBUG)
		printf(COL_MAGENTA "\npathname = %s\n" COL_RESET, pathname);

	if (!pathname && !is_builtin(tokens))
		error_custom(shell, 127, "minishell: command not found:", tokens[0]);
	if (!tokens[0] || !*tokens[0])
		exit(0);
	if (is_path_like(pathname))
		check_path_errors(shell, pathname);
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

static pid_t	execute_command(t_shell *shell, char **cmd, char **tokens, int has_next, int in_fd, int out_fd)
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
		{
			perror("dup");
			if (saved_stdin != -1)
				close(saved_stdin);
			if (saved_stdout != -1)
				close(saved_stdout);
			return (-1);
		}
		dup2(STDIN_FILENO, shell->stdin_save);
		dup2(STDOUT_FILENO, shell->stdout_save);
		handle_redirections(cmd, shell);
		ft_execute_builtin(tokens, shell);
		dup2(saved_stdin, STDIN_FILENO);
		dup2(saved_stdout, STDOUT_FILENO);
		close(saved_stdin);
		close(saved_stdout);
		return (-2);
	}
	else
		pid = fork_and_exec(tokens, cmd, shell, in_fd, out_fd);
	return (pid);
}

void	ft_execute_pipes(t_shell *shell)
{
	int		pipefd[2];
	char	**tokens;
	int		i;
	int		n;
	int		has_next;
	pid_t	pids[256] = {0};
	int		in_fd = -1;

	n = 0;
	i = 0;
	while (shell->cmds[i])
	{
		has_next = (shell->cmds[i + 1] != NULL);
		if (has_next && pipe(pipefd) == -1)
			handle_error(PIPES, shell);
		if (check_heredoc(shell->cmds[i], shell) == -1)
		{
			if (in_fd != -1)
				close(in_fd);
			if (has_next)
			{
				close(pipefd[0]);
				close(pipefd[1]);
			}
			return ;
		}
		filter_args(shell->cmds[i], &tokens, shell);
		int out_fd = (has_next ? pipefd[1] : -1);
		pid_t pid = execute_command(shell, shell->cmds[i], tokens, has_next, in_fd, out_fd);
		if (pid > 0)
			pids[n++] = pid;
		ft_free_array(&tokens);
		if (has_next)
		{
			if (pipefd[1] >= 0)
				close(pipefd[1]);
			if (in_fd != -1)
				close(in_fd);
			in_fd = pipefd[0];
		}
		else
		{
			if (in_fd != -1)
			{
				close(in_fd);
				in_fd = -1;
			}
		}
		i++;
	}
	if (in_fd != -1)
	{
		close(in_fd);
		in_fd = -1;
	}

	ft_wait_children(shell, pids, n);
	restore_stdio(shell);
}

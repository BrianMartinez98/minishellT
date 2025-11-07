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
#include <sys/wait.h>
#include <errno.h>
#include <unistd.h>
#include <limits.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>

void	free_tokens(char **tokens)
{
	int i;

	if (!tokens)
		return;
	for (i = 0; tokens[i]; i++)
		free(tokens[i]);
	free(tokens);
}

/* ---------- debug helpers ---------- */
void	print_tokens(char **tokens)
{
	int	i;

	if (!tokens)
	{
		printf("[DEBUG] tokens = (null)\n");
		return ;
	}
	printf("\033[0;36m[DEBUG] Tokens:\033[0m\n");
	for (i = 0; tokens[i]; i++)
		printf("  [%d] '%s'\n", i, tokens[i]);
}

void	print_shellenv(char **env)
{
	int	i;

	if (!env)
	{
		printf("[DEBUG] env = (null)\n");
		return ;
	}
	printf("\033[0;36m[DEBUG] Environment variables:\033[0m\n");
	for (i = 0; env[i]; i++)
		printf("  %s\n", env[i]);
}

void	print_pathname(const char *pathname)
{
	printf("\033[0;36m[DEBUG] Pathname:\033[0m ");
	if (pathname)
		printf("'%s'\n", pathname);
	else
		printf("(null)\n");
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
	handle_redirections(cmd, shell);
	close_fds_except(STDIN_FILENO, STDOUT_FILENO, STDERR_FILENO);
	if (is_builtin(tokens))
	{
		ft_execute_builtin(tokens, shell);
		exit(shell->last_status & 0xFF);
	}
	paths = paths_finder(shell->env);
	pathname = command_finder(tokens, paths);
	if (DEBUG)
	{
		printf("\033[0;35m\nDEBUG execve:\npathname = %s\n", pathname);
		printf("\033[0m\n");
	}
	if (!pathname)
	{
		ft_putstr_fd("minishell: ", STDERR_FILENO);
		ft_putstr_fd(tokens[0], STDERR_FILENO);
		ft_putstr_fd(": command not found\n", STDERR_FILENO);
		exit(127);
	}
	execve(pathname, tokens, shell->env);
	perror(tokens[0]);
	exit(127);
}

static pid_t	fork_and_exec(char **tokens, char **cmd, t_shell *shell, int in_fd, int out_fd)
{
	pid_t	pid;

	pid = fork();
	if (pid < 0)
	{
		perror("fork");
		shell->last_status = 1;
		return (-1);
	}
	if (pid == 0)
		pid_child(tokens, cmd, shell, in_fd, out_fd);
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
	int		status = 0;
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
		free_tokens(tokens);
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
	for (int j = 0; j < n; j++)
	{
		if (waitpid(pids[j], &status, 0) == -1)
			perror("waitpid");
	}
	/* Restaurar correctamente stdin/stdout del shell */
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
		if (WIFEXITED(status))
			shell->last_status = WEXITSTATUS(status);
		else if (WIFSIGNALED(status))
			shell->last_status = 128 + WTERMSIG(status);
	}
	shell->builtin = 0;
}

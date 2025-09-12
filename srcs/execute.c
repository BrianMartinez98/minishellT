/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jarregui <jarregui@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/02 19:47:59 by jarregui          #+#    #+#             */
/*   Updated: 2025/09/12 12:54:34 by jarregui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	ft_pid(pid_t pid, t_shell *shell)
{
	int	status;

	signal(SIGINT, SIG_IGN);
	waitpid(pid, &status, 0);
	ft_setup_signals_prompt();
	if (WIFEXITED(status))
		shell->last_status = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
		shell->last_status = 128 + WTERMSIG(status);
}

static int	ft_pid_zero(t_shell *shell)
{
	filter_args(&shell->tokens, &shell->child_args);
	ft_setup_signals_child();
	handle_redirections(shell);
	execvp(shell->child_args[0], shell->child_args);
	if (errno == ENOENT)
		printf("command not found: %s\n", shell->tokens[0]);
	else
		perror(shell->tokens[0]);
	ft_free_array(&shell->child_args);
	exit(127);
}

int	ft_is_builtin(t_shell *shell)
{
	if (ft_strcmp(shell->tokens[0], "exit") == 0
		|| ft_strcmp(shell->tokens[0], "pwd") == 0
		|| ft_strcmp(shell->tokens[0], "echo") == 0
		|| ft_strcmp(shell->tokens[0], "cd") == 0
		|| ft_strcmp(shell->tokens[0], "export") == 0
		|| ft_strcmp(shell->tokens[0], "unset") == 0
		|| ft_strcmp(shell->tokens[0], "history") == 0
		|| ft_strcmp(shell->tokens[0], "env") == 0)
		return (1);
	return (0);
}

static void	ft_execute_std(t_shell *shell)
{
	filter_args(&shell->tokens, &shell->clean_args);
	shell->stdin_save = dup(STDIN_FILENO);
	shell->stdout_save = dup(STDOUT_FILENO);
	handle_redirections(shell);
	if (ft_strcmp(shell->clean_args[0], "exit") == 0)
		shell->exit = 1;
	else if (ft_strcmp(shell->clean_args[0], "pwd") == 0)
		printf("%s\n", shell->cwd);
	else if (ft_strcmp(shell->clean_args[0], "echo") == 0)
		ft_echo(shell);
	else if (ft_strcmp(shell->clean_args[0], "cd") == 0)
		change_path(shell, shell->clean_args);
	else if (ft_strcmp(shell->clean_args[0], "export") == 0)
		ft_export(shell);
	else if (ft_strcmp(shell->clean_args[0], "unset") == 0)
		ft_unset(shell);
	else if (ft_strcmp(shell->clean_args[0], "history") == 0)
		ft_print_history(shell);
	else if (ft_strcmp(shell->clean_args[0], "env") == 0)
		ft_print_env(shell);
	dup2(shell->stdin_save, STDIN_FILENO);
	dup2(shell->stdout_save, STDOUT_FILENO);
	close(shell->stdin_save);
	close(shell->stdout_save);
}

int	ft_execute(t_shell *shell)
{
	pid_t	pid;

	if (!shell->tokens || !shell->tokens[0])
		return (0);
	if (ft_is_builtin(shell))
		ft_execute_std(shell);
	else
	{
		pid = fork();
		if (pid == -1)
		{
			perror("Error at fork");
			shell->last_status = 1;
			return (shell->last_status);
		}
		if (pid == 0)
			ft_pid_zero(shell);
		else
			ft_pid(pid, shell);
	}
	return (shell->last_status);
}

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

static int	ft_execute_builtin(char **tokens, t_shell *shell)
{
	if (ft_strcmp(tokens[0], "exit") == 0)
		shell->exit = 1;
	else if (ft_strcmp(tokens[0], "pwd") == 0)
		printf("%s\n", shell->cwd);
	else if (ft_strcmp(tokens[0], "echo") == 0)
		ft_echo(tokens, shell);
	else if (ft_strcmp(tokens[0], "cd") == 0)
		change_path(tokens, shell);
	else if (ft_strcmp(tokens[0], "export") == 0)
		ft_export(tokens, shell);
	else if (ft_strcmp(tokens[0], "unset") == 0)
		ft_unset(tokens, shell);
	else if (ft_strcmp(tokens[0], "history") == 0)
		ft_print_history(shell);
	else if (ft_strcmp(tokens[0], "env") == 0)
		ft_print_env(shell);
	else
		return (0);
	return (1);
}

static void	pid_child(char **tokens, char **cmd, t_shell *shell)
{
	ft_setup_signals_child();
	if (shell->stdin_save != STDIN_FILENO)
		dup2(shell->stdin_save, STDIN_FILENO);
	if (shell->stdout_save != STDOUT_FILENO)
		dup2(shell->stdout_save, STDOUT_FILENO);
	handle_redirections(cmd, shell);
	if (ft_execute_builtin(tokens, shell))
		exit(127);
	execvp(tokens[0], tokens);
	perror(tokens[0]);
	exit(127);

}

static int	ft_execute(char **tokens, char **cmd, t_shell *shell)
{
	pid_t	pid;
	int		status;

	if (!tokens || !tokens[0])
		return 0;
	pid = fork();
	if (pid < 0)
	{
		perror("fork");
		shell->last_status = 1;
		return 1;
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
		filter_args(shell->cmds[i], &tokens, shell);
		ft_execute(tokens, shell->cmds[i], shell);
		free(tokens);
		if (shell->cmds[i + 1])
		{
			close(fd[1]);
			shell->stdin_save = fd[0];
		}
		i++;
	}
}

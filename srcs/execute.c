/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jarregui <jarregui@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/02 19:47:59 by jarregui          #+#    #+#             */
/*   Updated: 2025/10/28 22:45:25 by jarregui         ###   ########.fr       */
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
	printf("El problema es en la pathfinder");
	pathname = command_finder(tokens, paths);
	printf("El problema es en la command_finder");
	if (DEBUG)
	{
		printf("\033[0;35m\nDEBUG execve:\npathname = %s\n", pathname);
		printf("\033[0m\n");
	}
	if (!pathname && !is_builtin(tokens))
		printf("minishell: Error: Command not found!\n");
	//implementarprint_tokens(tokens);			
	//implementarprint_shellenv(shell->env);		
	//implementarprint_pathname(pathname);		
	execve(pathname, tokens, shell->env);
	printf("El problema es en la execve");
	perror(tokens[0]);
	exit(127);
}

static int	ft_execute(char **tokens, char **cmd, t_shell *shell)
{
	pid_t	pid;

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
	return (pid);
}

static pid_t	execute_command(t_shell *shell, char **cmd,
	char **tokens, int has_next)
{
	int		saved_stdin;
	int		saved_stdout;
	pid_t	pid;

	pid = -1;
	if (!tokens || !tokens[0])
		return (-1);
	if (is_builtin(tokens) && !has_next)
	{
		shell->builtin = 1;
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
		pid = ft_execute(tokens, cmd, shell);
	return (pid);
}

void	ft_execute_pipes(t_shell *shell)
{
	int		fd[2];
	char	**tokens;
	int		i;
	int		n;
	int		has_next;
	int		status = 0;
	pid_t	pids[256] = {0};

	n = 0;
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
		if (check_heredoc(shell->cmds[i], shell) == -1)
			return ;
		filter_args(shell->cmds[i], &tokens, shell);
		//printf("El problema es en filter_args");
		pid_t pid = execute_command(shell, shell->cmds[i], tokens, has_next);
		//printf("El problema es execute");
		if (pid > 0)
			pids[n++] = pid;
		free(tokens);
		if (shell->cmds[i + 1])
		{
			close(fd[1]);
			shell->stdin_save = fd[0];
		}
		i++;
	}
	//printf("Ejecucion completada");
	for (int j = 0; j < n; j++)
		waitpid(pids[j], &status, 0);
	if (!shell->builtin)
	{
		if (WIFEXITED(status))
			shell->last_status = WEXITSTATUS(status);
		else if (WIFSIGNALED(status))
			shell->last_status = 128 + WTERMSIG(status);
	}
	shell->builtin = 0;
}

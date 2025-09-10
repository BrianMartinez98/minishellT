/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jarregui <jarregui@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/02 19:47:59 by jarregui          #+#    #+#             */
/*   Updated: 2025/09/10 22:37:58 by jarregui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

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



int	ft_execute(t_shell *shell)
{
	pid_t	pid;
	char	*command;
	int		stdin_save;
	int		stdout_save;
	int		is_builtin;

	shell->clean_args = filter_args(shell->tokens);
	if (!shell->tokens || !shell->tokens[0])
		return (0);
	is_builtin = ft_is_builtin(shell);
	command = shell->tokens[0];
	if (is_builtin)
	{
		stdin_save = dup(STDIN_FILENO);
		stdout_save = dup(STDOUT_FILENO);
		handle_redirections(shell->tokens);
		if (ft_strcmp(command, "exit") == 0)
			shell->exit = 1;
		else if (ft_strcmp(command, "pwd") == 0)
			printf("%s\n", shell->cwd);
		else if (ft_strcmp(command, "echo") == 0)
			ft_echo(shell);
		else if (ft_strcmp(command, "cd") == 0)
			change_path(shell, shell->clean_args);
		else if (ft_strcmp(command, "export") == 0)
			ft_export(shell);
		else if (ft_strcmp(command, "unset") == 0)
			ft_unset(shell);
		else if (ft_strcmp(command, "history") == 0)
			ft_print_history(shell);
		else if (ft_strcmp(command, "env") == 0)
			ft_print_env(shell);

		// Restaura stdin y stdout
		dup2(stdin_save, STDIN_FILENO);
		dup2(stdout_save, STDOUT_FILENO);
		close(stdin_save);
		close(stdout_save);
	}
	else
	{
		pid = fork();
		if (pid == -1)
		{
			perror("Error at fork");
			shell->last_status = 1; // Error al crear el proceso hijo
			return (shell->last_status);
		}
		if (pid == 0)
		{
			// Proceso hijo
			ft_setup_signals_child(); // ← para Ctrl+C y Ctrl+\ se comporten como en bash
			handle_redirections(shell->tokens);

			shell->child_args = filter_args(shell->tokens);
			execvp(shell->child_args[0], shell->child_args);

			// execvp(command, shell->tokens); // Busca el binario en el PATH antiguo juancho
			if (errno == ENOENT)
				printf("command not found: %s\n", command);
			else
				perror(command);
			free(shell->child_args);
			shell->child_args = NULL;
			exit(127);
		}
		else
		{
			// Proceso padre
			int	status;

			signal(SIGINT, SIG_IGN);
			waitpid(pid, &status, 0);
			ft_setup_signals_prompt(); // ← Restauras señales de minishell tras el hijo
			if (WIFEXITED(status))
				shell->last_status = WEXITSTATUS(status); // ← valor real del $?
			// si murió por señal:
			else if (WIFSIGNALED(status))
				shell->last_status = 128 + WTERMSIG(status);  // por ejemplo 130 si SIGINT
		}
	}
	free(shell->clean_args);
	shell->clean_args = NULL;
	return (shell->last_status);
}

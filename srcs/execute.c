/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jarregui <jarregui@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/02 19:47:59 by jarregui          #+#    #+#             */
/*   Updated: 2025/09/09 13:47:40 by jarregui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	ft_execute(t_shell *shell)
{
	pid_t	pid;
	char	*command;

	if (!shell->tokens || !shell->tokens[0])
		return (0);
	command = shell->tokens[0];
	if (ft_strcmp(command, "exit") == 0)
		shell->exit = 1;
	else if (ft_strcmp(command, "pwd") == 0)
		printf("%s\n", shell->cwd);
	else if (ft_strcmp(command, "echo") == 0)
		ft_echo(shell); 
	// else if (ft_strcmp(command, "cd") == 0) // a implementar solo con una ruta relativa o absoluta.
	// 	printf("cd command not implemented yet.\n");
	else if (ft_strcmp(command, "export") == 0)
		ft_export(shell);
	else if (ft_strcmp(command, "unset") == 0)
		ft_unset(shell);
	else if (ft_strcmp(command, "history") == 0)
		ft_print_history(shell);
	else if (ft_strcmp(command, "env") == 0)
		ft_print_env(shell);
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
			execvp(command, shell->tokens); // Busca el binario en el PATH
			if (errno == ENOENT)
				printf("command not found: %s\n", command);
			else
				perror(command); // imprime mensaje detallado del sistema
			exit(127);// Código estándar: comando no encontrado
		}
		else
		{
			// Proceso padre
			int	status;

			ft_setup_signals_prompt(); // ← Restauras señales de minishell tras el hijo
			waitpid(pid, &status, 0);
			if (WIFEXITED(status))
				shell->last_status = WEXITSTATUS(status); // ← valor real del $?
			// si murió por señal:
			else if (WIFSIGNALED(status))
				shell->last_status = 128 + WTERMSIG(status);  // por ejemplo 130 si SIGINT
		}
	}
	return (shell->last_status);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jarregui <jarregui@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/02 19:47:59 by jarregui          #+#    #+#             */
/*   Updated: 2025/08/04 00:43:36 by jarregui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_execute(char **array, t_shell *shell)
{
	int		result;
	pid_t	pid;

	result = 0;
	if (ft_strcmp(array[0], "exit") == 0)
		shell->exit = 1;
	else if (ft_strcmp(array[0], "pwd") == 0)
		printf("%s\n", shell->cwd);
	else if (ft_strcmp(array[0], "echo") == 0)
		ft_echo(shell, array); 
	// else if (ft_strcmp(array[0], "cd") == 0) // a implementar solo con una ruta relativa o absoluta.
	// 	printf("cd command not implemented yet.\n");
	// else if (ft_strcmp(array[0], "export") == 0) // a implementar sin opciones.
	// 	printf("export command not implemented yet.\n");
	// else if (ft_strcmp(array[0], "unset") == 0) // a implementar sin opciones.
	// 	printf("unset command not implemented yet.\n");
	else if (ft_strcmp(array[0], "history") == 0)
		ft_print_history(shell);
	else if (ft_strcmp(array[0], "env") == 0)
		ft_print_env(shell);
	else
	{
		pid = fork();
		if (pid == -1)
		{
			perror("fork");
			return (1);
		}
		if (pid == 0)
		{
			// Proceso hijo
			ft_setup_signals_child(); // ← para Ctrl+C y Ctrl+\ se comporten como en bash
			execvp(array[0], array); // Busca el binario en el PATH
			if (errno == ENOENT)
				printf("command not found: %s\n", array[0]);
			else
				perror(array[0]); // imprime mensaje detallado del sistema
			exit(127); // Código estándar: comando no encontrado
		}
		else
		{
			// Proceso padre
			int	status;

			ft_setup_signals_prompt(); // ← Restauras señales de minishell tras el hijo
			waitpid(pid, &status, 0);
		}
	}
		// printf("command not found: %s\n", array[0]);
	return (result);
}

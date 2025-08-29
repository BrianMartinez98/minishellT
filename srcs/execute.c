/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jarregui <jarregui@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/02 19:47:59 by jarregui          #+#    #+#             */
/*   Updated: 2025/08/04 20:10:16 by jarregui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	ft_execute(char **array, t_shell *shell)
{
	pid_t	pid;

	if (ft_strcmp(array[0], "exit") == 0)
		shell->exit = 1;	
	else if (strcmp(array[0], ">") == 0)
        dupp(array[0 + 1], STDOUT_FILENO, O_WRONLY | O_CREAT | O_TRUNC);
    else if (strcmp(array[0], "<") == 0)
        dupp(array[0 + 1], STDIN_FILENO, O_RDONLY);
	else if (ft_strcmp(array[0], "pwd") == 0)
		printf("%s\n", shell->cwd);
	else if (ft_strcmp(array[0], "echo") == 0)
		ft_echo(shell, array); 
	else if (ft_strcmp(array[0], "cd") == 0)
		change_path(shell, array);
	else if (ft_strcmp(array[0], "export") == 0)
		ft_export(shell, array);
	else if (ft_strcmp(array[0], "unset") == 0)
		ft_unset(shell, array);
	else if (ft_strcmp(array[0], "history") == 0)
		ft_print_history(shell);
	else if (ft_strcmp(array[0], "env") == 0)
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
			execvp(array[0], array); // Busca el binario en el PATH
			if (errno == ENOENT)
				printf("command not found: %s\n", array[0]);
			else
				perror(array[0]); // imprime mensaje detallado del sistema
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

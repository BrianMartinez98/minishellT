/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jarregui <jarregui@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/10 19:14:30 by jarregui          #+#    #+#             */
/*   Updated: 2025/09/10 19:14:39 by jarregui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

// Función auxiliar para dividir por '|'
static int	count_pipes(char **tokens) {
	int count = 0, i = 0;
	while (tokens[i]) {
		if (strcmp(tokens[i], "|") == 0)
			count++;
		i++;
	}
	return count;
}

// Divide tokens en arrays de comandos separados por '|'
static char	***split_by_pipe(char **tokens, int *num_cmds) {
	int pipes = count_pipes(tokens);
	*num_cmds = pipes + 1;
	char ***cmds = malloc(sizeof(char **) * (*num_cmds + 1));
	int i = 0, j = 0, k = 0, start = 0;
	while (tokens[i]) {
		if (strcmp(tokens[i], "|") == 0) {
			cmds[k] = malloc(sizeof(char *) * (i - start + 1));
			for (j = start; j < i; j++)
				cmds[k][j - start] = tokens[j];
			cmds[k][i - start] = NULL;
			k++;
			start = i + 1;
		}
		i++;
	}
	// Último comando
	cmds[k] = malloc(sizeof(char *) * (i - start + 1));
	for (j = start; j < i; j++)
		cmds[k][j - start] = tokens[j];
	cmds[k][i - start] = NULL;
	cmds[k + 1] = NULL;
	return cmds;
}

int	ft_execute_pipes(t_shell *shell)
{
	int num_cmds, i, status;
	char ***cmds;
	int fd[2], in_fd = 0, pid;
		
	// Detecta si hay pipes
	int has_pipe = 0;
	for (i = 0; shell->tokens[i]; i++)
		if (strcmp(shell->tokens[i], "|") == 0)
			has_pipe = 1;

	if (!has_pipe)
	{
		return ft_execute(shell); // función auxiliar con tu lógica actual
	}

	// Si hay pipes:
	cmds = split_by_pipe(shell->tokens, &num_cmds);
	for (i = 0; i < num_cmds; i++)
	{
		if (i < num_cmds - 1)
			pipe(fd);
		pid = fork();
		if (pid == 0)
		{
			if (i > 0) {
				dup2(in_fd, STDIN_FILENO);
				close(in_fd);
			}
			if (i < num_cmds - 1) {
				close(fd[0]);
				dup2(fd[1], STDOUT_FILENO);
				close(fd[1]);
			}
			handle_redirections(cmds[i]);
			char **clean_args = filter_args(cmds[i]);
			execvp(clean_args[0], clean_args);
			perror(clean_args[0]);
			free(clean_args);
			exit(127);
		}
		if (i > 0)
			close(in_fd);
		if (i < num_cmds - 1) {
			close(fd[1]);
			in_fd = fd[0];
		}
	}
	for (i = 0; i < num_cmds; i++)
		wait(&status);
	// libera memoria de cmds si lo necesitas
	return WEXITSTATUS(status);
}
// ...existing code...
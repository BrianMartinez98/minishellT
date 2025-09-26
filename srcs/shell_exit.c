/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_exit.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jarregui <jarregui@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/09 15:00:40 by jarregui          #+#    #+#             */
/*   Updated: 2025/09/12 12:55:12 by jarregui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	ft_free_array(char ***array)
{
	size_t	i;

	if (!array || !*array)
		return ;
	i = 0;
	while ((*array)[i])
	{
		free((*array)[i]);
		i++;
	}
	free(*array);
	*array = NULL;
}

void	ft_free_matrix(char ****matrix)
{
	size_t	i;

	if (!matrix || !*matrix)
		return ;
	i = 0;
	while ((*matrix)[i])
	{
		ft_free_array(&(*matrix)[i]);
		i++;
	}
	free(*matrix);
	*matrix = NULL;
}

int	ft_exit_shell(t_shell *shell)
{
	if (shell->prompt)
		free(shell->prompt);
	if (shell->cwd)
		free(shell->cwd);
	if (shell->hist)
		ft_free_history(shell);
	if (shell->env)
		ft_free_env(shell);
	if (shell->line)
		ft_free_line(shell);
	if (shell->line2)
		ft_free_line(shell);
	if (shell->cmds)
		ft_free_matrix(&shell->cmds);
	if (shell->stdin_save != -1)
		close(shell->stdin_save);
	if (shell->stdout_save != -1)
		close(shell->stdout_save);
	ft_restore_term_settings(shell);
	if (DEBUG && shell->eof)
		write(1, "\n	Pulsada tecla Ctrl+D (EOF)\n", 30);
	printf("\033[1;32mExiting minishell...\033[0m\n");
	return (0);
}

static void print_errors(t_errorlst error, char *error_msg[])
{
	write(2, "ERROR: ", 7);
	if (error == FD)
		perror("open");
	else if (error >= 0 && error_msg[error])
		write(2, error_msg[error], ft_strlen(error_msg[error]));
	else
		write(2, "Unknown error\n", 14);
}

void	handle_error(t_errorlst error, t_shell *shell)
{
	char	*error_msg[42];

	error_msg[PIPES] = "Error en el pipe!\n";
	error_msg[MALLOCERROR] = "Error Malloc!\n";
	error_msg[FD] = "Fallo en el fd!\n";
	error_msg[GETCWD] = "Error en GETCWD!\n";
	error_msg[MSG] = "Error en el MSG!\n";
	print_errors(error, error_msg);
	ft_exit_shell(shell);
	exit(1);
}
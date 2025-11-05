/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jarregui <jarregui@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/03 12:42:53 by jarregui          #+#    #+#             */
/*   Updated: 2025/11/03 14:25:45 by jarregui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

//CUSTOM ERROR FUNCTION
void	error_custom(t_shell *shell, int err_code, char *err_msg, char *err_key)
{
	char	*err;
	char	*temp_err;

	shell->last_status = err_code;
	if (err_key)
		temp_err = ft_strjoin3(err_msg, " ", err_key);
	else
		temp_err = err_msg;
	err = ft_strjoin(temp_err, "\n");
	if (DEBUG)
	{
		printf("\033[1;35mExit Error Code: %d\n", err_code);
		printf("Error Message: %s\033[0m", err);
	}
	ft_putstr_fd(err, STDERR);
	if (err_key)
		free(temp_err);
	free(err);
}

//SYSTEM ERROR FUNCTION
static void	print_errors(t_errorlst error, char *error_msg[])
{
	write(2, "ERROR: ", 7);
	if (error == FD)
		perror("open");
	else if (error >= 0 && error_msg[error])
		write(STDERR, error_msg[error], ft_strlen(error_msg[error]));
	else
		write(STDERR, "Unknown error\n", 14);
}

void	handle_error(t_errorlst error, t_shell *shell)
{
	char	*error_msg[42];

	printf("Hubo un error");

	error_msg[PIPES] = "Error en el pipe!\n";
	error_msg[MALLOCERROR] = "Error Malloc!\n";
	error_msg[FD] = "Fallo en el fd!\n";
	error_msg[GETCWD] = "Error en GETCWD!\n";
	error_msg[MSG] = "Error en el MSG!\n";
	error_msg[NCMD] = "Command not found!\n";
	print_errors(error, error_msg);
	ft_exit_shell(shell);
	exit(1);
}
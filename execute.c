/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jarregui <jarregui@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/02 19:47:59 by jarregui          #+#    #+#             */
/*   Updated: 2025/08/03 00:23:49 by jarregui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_execute(char **array, t_shell *shell)
{
	int	result;

	result = 0;
	if (ft_strcmp(array[0], "exit") == 0) //sin opciones.
		shell->exit = 1;
	else if (ft_strcmp(array[0], "echo") == 0) // a implementar con la opción -n.
		printf("echo command not implemented yet.\n");
	else if (ft_strcmp(array[0], "cd") == 0) // a implementar solo con una ruta relativa o absoluta.
		printf("cd command not implemented yet.\n");
	else if (ft_strcmp(array[0], "pwd") == 0) //sin opciones.
		printf("%s\n", shell->cwd);
	else if (ft_strcmp(array[0], "export") == 0) // a implementar sin opciones.
		printf("export command not implemented yet.\n");
	else if (ft_strcmp(array[0], "unset") == 0) // a implementar sin opciones.
		printf("unset command not implemented yet.\n");
	else if (ft_strcmp(array[0], "history") == 0)
		printf("history command not implemented yet.\n");
	else if (ft_strcmp(array[0], "env") == 0) //a implementar sin opciones o argumentos.
		printf("env command not implemented yet.\n");
	else
		printf("command not found: %s\n", array[0]);
	return (result);
}

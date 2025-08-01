/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jarregui <jarregui@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/01 23:47:36 by jarregui          #+#    #+#             */
/*   Updated: 2025/08/02 00:12:02 by jarregui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_execute(char **array, t_shell *shell)
{
	int	result;

	result = 0;
	if (ft_strcmp(array[0], "exit") == 0)
		ft_exit_shell(shell);

	return (result);
}

int	print_tokens(char **tokens)
{
	if (!tokens) return 1;
	for (size_t i = 0; tokens[i] != NULL; i++)
	{
		printf("Token[%zu]: '%s'\n", i, tokens[i]);
		free(tokens[i]);
	}
	return 0;
}

int	main(int ac, char **av, char **env)
{
	int		status;
	char	*line;
	char	**array;
	pid_t	pid;
	t_hist	**lst;

	t_shell	shell;

	(void)ac; //ignoramos estos parámetros que no usamos
	(void)av; //ignoramos estos parámetros que no usamos

	shell.prompt = NULL;
	shell.in = dup(STDIN);
	shell.out = dup(STDOUT);
	shell.exit = 0;
	shell.ret = 0;
	shell.no_exec = 0;
	env_init(&shell, env); //copiamos las variables de entorno.
	
	lst = (t_hist **)malloc(sizeof(t_hist));
	while (shell.exit == 0)
	{
		ft_build_prompt(&shell); // Construimos el prompt con el directorio actual
		line = readline(shell.prompt); //aquí imprimimos el prompt y se queda a la espera para leer la línea que introduzcamos. Es una función estandar de C con readline.h
		printf("estoy despues de line, se supone que se queda esperando y no se imprime esto si no escribo nada\n");
		if (line && *line)
			add_history(line);

		//Nota: readline ya maneja la memoria de la línea introducida, no es necesario liberarla aquí.
		// ft_add_history(lst, line);
		// if (!line)
		// {
		// 	printf("exit\n");
		// 	break;
		// }


		array = split_line(line);
		if (!array || !array[0])
		{
			free(line);
			continue;
		}
		ft_execute(array, &shell); // Ejecutamos el comando introducido
		
		//print_history(lst); //esto ya no hace falta


		//print_tokens(array);		  // Comprobar que los tokens se parsean correctamente

		/*						   Usar esto para ejecutar comandos como ls -la
		pid = fork();
		if (pid < 0)
		{
			perror("fork failed");
			free(line);
			continue;
		}
		else if (pid == 0)
		{
			execvp(array[0], array);
			perror("execvp failed");
			exit(1);
		}
		wait(&status);
		*/
		free(line);
		free(array);
	}

	
	return 0;
}

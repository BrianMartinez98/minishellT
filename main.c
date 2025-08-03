/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jarregui <jarregui@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/01 23:47:36 by jarregui          #+#    #+#             */
/*   Updated: 2025/08/03 17:11:57 by jarregui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


// TODO:

//Continuar con las señales


#include "minishell.h"

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
	t_shell	shell;

	(void)ac; //ignoramos estos parámetros que no usamos
	(void)av; //ignoramos estos parámetros que no usamos
	ft_init_shell(&shell, env); // Inicializamos la estructura shell

	//lst = (t_hist **)malloc(sizeof(t_hist));
	while (shell.exit == 0)
	{
		ft_build_prompt(&shell); // Construimos el prompt con el directorio actual
		ft_setup_signals_prompt();
		line = readline(shell.prompt); //aquí imprimimos el prompt y se queda a la espera para leer la línea que introduzcamos. Es una función estandar de C con readline.h
		if (line && *line)
			ft_add_history(line, &shell); // si introducimos algo que no sea nulo o vacío, lo añadimos al historial.
		if (!line) // Ctrl+D
			break;

		array = split_line(line);
		if (!array || !array[0])
		{
			free(line);
			continue;
		}
		ft_execute(array, &shell); // Ejecutamos el comando introducido
		
		//print_tokens(array);		  // Comprobar que los tokens se parsean correctamente

		free(line);
		free(array);
	}
	ft_exit_shell(&shell);
}

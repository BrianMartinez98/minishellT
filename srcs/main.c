/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jarregui <jarregui@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/01 23:47:36 by jarregui          #+#    #+#             */
/*   Updated: 2025/08/05 17:40:55 by jarregui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */



//HECHO
//Historial: funciona con flechas, con el comando history y se libera todo crrectamente al salir.
//Ejecución de comandos externos: funciona, pero falta redirección de entrada y salida. Hace bien el fork y la ejecución de comandos externos. Los procesos hijos se cierran bien en caso de cuelgue con CTRL+D sin matar al padre
//Salida limpia: al salir de la minishell se libera toda la memoria asignada a la estructura shell y sus componentes.
//Builtins: 
	//exit: funciona correctamente, libera memoria y sale de la minishell.
	//pwd: funciona correctamente, imprime el directorio actual.
	//history: funciona correctamente, imprime el historial de comandos y libera memoria al salir.
	//env : funciona correctamente, imprime las variables de entorno y libera memoria al salir.
	//echo: imprime argumentos y variables $ (faltaría posible redirección)
	//echo -n: implementada la opción -n para no añadir salto de línea al final.
	//$?: actualizado en shell->last_status tras la ejecución de un comando externo y los builtins pero hay que checarlo bien
//Makefile y añadido libft



//PENDIENTE BRIAN
//builtin cd
//redirecciones > >> < <<
//pipes |
//revisar tokenización y split_line

//PENDIENTE JUANCHO
//Make: que no haga RELINK el makefile
//Make: añadir versión Debug para checar leaks y errores de memoria
//$crear y borrar variables de entorno (revisar export)
//Señales. Lo tengo casi. Continuar con las señales
//Norminette: quitar comentarios y ver longitudes de funciones y num variables

//PENDIENTE EXTRA
//parseo " "\" ; `

#include "../minishell.h"

int	main(int ac, char **av, char **env)
{
	char	*line;
	char	**array;
	t_shell	shell;

	(void)ac; //ignoramos estos parámetros que no usamos
	(void)av; //ignoramos estos parámetros que no usamos
	ft_init_shell(&shell, env); // Inicializamos la estructura shell

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
		ft_execute_pipes(array, &shell); // Ejecutamos el comando introducido
		
		//print_tokens(array);		  // Comprobar que los tokens se parsean correctamente

		free(line);
		free(array);
	}
	ft_exit_shell(&shell);
}

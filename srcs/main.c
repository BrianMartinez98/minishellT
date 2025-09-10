/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jarregui <jarregui@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/01 23:47:36 by jarregui          #+#    #+#             */
/*   Updated: 2025/09/10 10:25:55 by jarregui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	main(int ac, char **av, char **env)
{
	t_shell	shell;

	(void)ac;
	(void)av;
	ft_init_shell(&shell, env);
	while (shell.exit == 0)
	{
		ft_build_prompt(&shell);
		ft_setup_signals_prompt();
		ft_readline(&shell);
		if (!shell.line)
		{
			shell.eof = 1;
			break ;
		}
		if (*shell.line == '\0')
			continue ;
		ft_add_history(&shell);
		split_line(&shell);
		if (!shell.tokens || !shell.tokens[0])
			continue ;
		ft_execute(&shell);
	}
	ft_exit_shell(&shell);
}

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
//Make: que no haga RELINK el makefile
//Make: añadir versión Debug para checar leaks y errores de memoria
//he modificado tokens y line y las he incluido en la estructura shell, para que se liberen al salir de la minishell y no haya fugas de memoria.
//Señales: funcionan ok


//PENDIENTE BRIAN
//builtin cd
//redirecciones > >> < <<
//pipes |
//revisar tokenización y split_line

//PENDIENTE JUANCHO
//$crear y borrar variables de entorno (revisar export)

//PENDIENTE EXTRA
//Norminette: quitar comentarios y ver longitudes de funciones y num variables
//parseo " "\" ; `
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jarregui <jarregui@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/02 00:14:01 by jarregui          #+#    #+#             */
/*   Updated: 2025/08/03 00:19:12 by jarregui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_init_shell(t_shell *shell, char **env)
{
	env_init(shell, env); //copiamos las variables de entorno.
	shell->cwd = NULL;
	shell->prompt = NULL;
	shell->exit = 0;
	//revisar estas que de momento no uso
	shell->in = dup(STDIN);
	shell->out = dup(STDOUT);
	shell->ret = 0;
	shell->no_exec = 0;
	//Nota: revisar el resto de variables que copié para shell, si son necesarias y su inicialización.
	shell->start = NULL;
	shell->env = NULL;
	shell->secret_env = NULL;
	shell->fdin = -1;
	shell->fdout = -1;
	shell->pipin = -1;
	shell->pipout = -1;
	shell->pid = 0;
	shell->charge = 0;
	shell->parent = 0;
	shell->last = 0;
	shell->ret = 0;
}


int	ft_exit_shell(t_shell *shell)
{
	if (shell->prompt)
		free(shell->prompt);
	if (shell->cwd)
		free(shell->cwd);
	printf("\033[1;32mExiting minishell...\033[0m\n");
	exit(0);
}

//IMPORTANTE: Esta función se encarga de liberar la memoria de la estructura shell y sus componentes antes de salir.
//Pensar que más mallocs usamos. 
//Checar los strjoin
//en caso de que sigamos usando la lista de historial, también habría que liberar la memoria de cada nodo de la lista.
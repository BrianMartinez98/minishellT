/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jarregui <jarregui@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/02 00:14:01 by jarregui          #+#    #+#             */
/*   Updated: 2025/08/02 00:28:58 by jarregui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_exit_shell(t_shell *shell)
{
	if (shell->prompt)
		free(shell->prompt);
	printf("\033[1;32mExiting minishell...\033[0m\n");
	exit(0);
}


//IMPORTANTE: Esta función se encarga de liberar la memoria de la estructura shell y sus componentes antes de salir.
//Pensar que más mallocs usamos. 
//Checar los strjoin
//en caso de que sigamos usando la lista de historial, también habría que liberar la memoria de cada nodo de la lista.
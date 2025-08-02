/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jarregui <jarregui@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/02 13:27:30 by jarregui          #+#    #+#             */
/*   Updated: 2025/08/02 19:43:47 by jarregui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void ft_sigint_handler(int sig)
{
	(void)sig;
	write(1, "\n", 1);
	printf("\nPulsada tecla Ctrl+C\n");
	rl_on_new_line();               // readline: mueve cursor a nueva línea
	rl_replace_line("", 0);         // limpia la línea actual
	rl_redisplay();                 // muestra prompt de nuevo
}

void ft_sigquit_handler(int sig)
{
	(void)sig;
	printf("\nPulsada tecla Ctrl+\\\n");
}

void	ft_setup_signals_prompt(void)
{
	signal(SIGINT, ft_sigint_handler);   // Ctrl+C: limpiar línea
	signal(SIGQUIT, ft_sigquit_handler);        // Ctrl+\: ignorado
}

void	ft_setup_signals_child(void)
{
	signal(SIGINT, SIG_DFL);         // Ctrl+C: comportamiento por defecto (mata)
	signal(SIGQUIT, SIG_DFL);        // Ctrl+\: comportamiento por defecto
}

// NOTA: podemos testar la combinación de teclas haciendo desde otra terminal
// kill -SIGINT <pid> para Ctrl+C
// kill -SIGQUIT <pid> para Ctrl+\
// kill -SIGTERM <pid> para Ctrl+D (EOF)
// donde <pid> es el ID del proceso de la minishell que queremos matar.
// Para obtener el PID: ps -x | grep minishell
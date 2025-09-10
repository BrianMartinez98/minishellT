/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jarregui <jarregui@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/02 13:27:30 by jarregui          #+#    #+#             */
/*   Updated: 2025/09/10 09:51:08 by jarregui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	ft_sigint_handler(int sig)
{
	(void)sig;
	if (DEBUG)
		write(1, "\n	Pulsada tecla Ctrl+C\n", 24);
	write(1, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

void	ft_sigquit_handler(int sig)
{
	(void)sig;
	if (DEBUG)
		write(1, "\n	Pulsada tecla Ctrl+\\\n", 24);
}

void	ft_sigterm_handler(int sig)
{
	t_shell	*shell;

	(void)sig;
	shell = ft_get_shell_address(NULL);
	if (DEBUG)
		write(1, "\n	Recibida señal Ctrl+D (EOF)\n", 32);
	ft_exit_shell(shell);
}

void	ft_setup_signals_prompt(void)
{
	signal(SIGINT, ft_sigint_handler);
	signal(SIGQUIT, ft_sigquit_handler);
	signal(SIGTERM, ft_sigterm_handler);
}

void	ft_setup_signals_child(void)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	signal(SIGTERM, SIG_DFL);

}

// NOTA 1: las tres señales que debemos gestionar son:
// SIGINT para Ctrl+C (valor 2) limpiar línea
// SIGQUIT para Ctrl+"\" (valor 3) ignorado
// SIGTERM para Ctrl+D (valor 15) EOF salir de la minishell
//
// NOTA 2: podemos testar la combinación de teclas haciendo desde otra terminal
// kill -SIGINT <pid> para Ctrl+C
// kill -SIGQUIT <pid> para Ctrl+"\"
// kill -SIGTERM <pid> para Ctrl+D (EOF)
// donde <pid> es el ID del proceso de la minishell que queremos matar.
// Para obtener el PID: ps -x | grep minishell
// en caso de hacer make debug usar el pid que incluya "valgrind.bin"
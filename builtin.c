/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jarregui <jarregui@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/02 00:14:01 by jarregui          #+#    #+#             */
/*   Updated: 2025/08/04 01:34:35 by jarregui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_init_shell(t_shell *shell, char **env)
{
	ft_env_init(shell, env); //copiamos las variables de entorno.
	shell->cwd = NULL;
	shell->prompt = NULL;
	shell->hist = NULL;
	shell->last_status = 0;
	shell->exit = 0;
	
	//revisar estas que de momento no uso
	shell->in = dup(STDIN);
	shell->out = dup(STDOUT);
	shell->ret = 0;
	shell->no_exec = 0;
	//Nota: revisar el resto de variables que copié para shell, si son necesarias y su inicialización.
	shell->start = NULL;
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
	ft_free_history(shell);
	ft_free_env(shell);
	printf("\033[1;32mExiting minishell...\033[0m\n");
	exit(0);
}
//IMPORTANTE: Esta función se encarga de liberar la memoria de la estructura shell y sus componentes antes de salir.
//Pensar que más mallocs usamos. 
//Checar los strjoin


void	ft_echo(t_shell *shell, char **tokens)
{
	// a implementar con la opción -n.
	int	i;


	//printf("imprimiendo tokens que nos llegan: \n");
	//print_tokens(tokens);


	i = 1;
	while (tokens[i])
	{
		if (tokens[i][0] == '$')
		{
			if (tokens[i][1] == '?' && tokens[i][2] == '\0')
				printf("%d", shell->last_status);  // echo $?
			else
			{
				char *val = ft_getenv(shell, tokens[i] + 1); // salta '$'
				if (val)
					printf("%s", val);
			}
		}
		else
			printf("%s", tokens[i]);
		if (tokens[i + 1])
			printf(" ");
		i++;
	}
	printf("\n");
}

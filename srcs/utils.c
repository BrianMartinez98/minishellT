/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jarregui <jarregui@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/02 00:12:49 by jarregui          #+#    #+#             */
/*   Updated: 2025/08/07 01:29:52 by jarregui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	ft_init_shell(t_shell *shell, char **env)
{
	ft_env_init(shell, env); //copiamos las variables de entorno.
	shell->cwd = NULL;
	shell->prompt = NULL;
	shell->hist = NULL;
	shell->last_status = 0;
	shell->exit = 0;
	shell->line = NULL;
	shell->tokens = NULL;
}

int	ft_exit_shell(t_shell *shell)
{
	if (shell->prompt)
		free(shell->prompt);
	if (shell->cwd)
		free(shell->cwd);
	ft_free_history(shell);
	ft_free_env(shell);
	ft_free_line(shell);
	ft_free_tokens(shell->tokens);
	printf("\033[1;32mExiting minishell...\033[0m\n");
	exit(0);
}
//IMPORTANTE: Esta función se encarga de liberar la memoria de la estructura shell y sus componentes antes de salir.
//Pensar que más mallocs usamos. 
//Checar los strjoin

void ft_build_prompt(t_shell *shell)
{
	char	*tmp;

	if (shell->prompt)
		free(shell->prompt);
	if (shell->cwd)
		free(shell->cwd);
	shell->prompt = NULL;
	shell->cwd = getcwd(NULL, 0); // Get current working directory - dinamic allocation
	if (!shell->cwd)
		perror("Getcwd failed");
	shell->prompt = ft_strjoin("\001\033[0;36m\033[1m\002", shell->cwd); //formato negrita y cyan
	tmp = shell->prompt;
	shell->prompt = ft_strjoin(tmp, " > \001\033[0m\002"); //restauramos formato normal
	free(tmp); 

	//IDEA: esto recalcula siempre el cwd, más adelante mirar que solo se haga esto si hay un cd exitoso.
}

int	ft_error(const char *msg)
{
	perror(msg);
	return (1);
}

int ft_isspace(char c)
{
	return (c == ' ' || c == '\t' || c == '\n' ||
			c == '\v' || c == '\f' || c == '\r');
}
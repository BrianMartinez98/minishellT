/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jarregui <jarregui@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/09 15:00:40 by jarregui          #+#    #+#             */
/*   Updated: 2025/09/09 15:05:35 by jarregui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_shell	*ft_get_shell_address(t_shell *new_shell)
{
	static t_shell	*shell_ptr = NULL;

	if (new_shell)
		shell_ptr = new_shell;
	return (shell_ptr);
}

void	ft_init_shell(t_shell *shell, char **env)
{
	ft_env_init(shell, env);
	shell->cwd = NULL;
	shell->prompt = NULL;
	shell->hist = NULL;
	shell->last_status = 0;
	shell->exit = 0;
	shell->line = NULL;
	shell->tokens = NULL;
	ft_get_shell_address(shell);
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


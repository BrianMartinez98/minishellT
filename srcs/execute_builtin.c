/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_builtin.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jarregui <jarregui@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/26 18:38:39 by jarregui          #+#    #+#             */
/*   Updated: 2025/09/26 18:42:48 by jarregui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	is_builtin(char **tokens)
{
	return (ft_strcmp(tokens[0], "exit") == 0
		|| ft_strcmp(tokens[0], "pwd") == 0
		|| ft_strcmp(tokens[0], "echo") == 0
		|| ft_strcmp(tokens[0], "cd") == 0
		|| ft_strcmp(tokens[0], "export") == 0
		|| ft_strcmp(tokens[0], "unset") == 0
		|| ft_strcmp(tokens[0], "history") == 0
		|| ft_strcmp(tokens[0], "env") == 0);
}

int	ft_execute_builtin(char **tokens, t_shell *shell)
{
	if (ft_strcmp(tokens[0], "exit") == 0)
		shell->exit = 1;
	else if (ft_strcmp(tokens[0], "pwd") == 0)
		printf("%s\n", shell->cwd);
	else if (ft_strcmp(tokens[0], "echo") == 0)
		ft_echo(tokens, shell);
	else if (ft_strcmp(tokens[0], "cd") == 0)
		change_path(tokens, shell);
	else if (ft_strcmp(tokens[0], "export") == 0)
		ft_export(tokens, shell);
	else if (ft_strcmp(tokens[0], "unset") == 0)
		ft_unset(tokens, shell);
	else if (ft_strcmp(tokens[0], "history") == 0)
		ft_print_history(shell);
	else if (ft_strcmp(tokens[0], "env") == 0)
		ft_print_env(shell);
	else
		return (0);
	return (1);
}

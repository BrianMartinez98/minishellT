/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jarregui <jarregui@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/10 19:46:54 by jarregui          #+#    #+#             */
/*   Updated: 2025/09/10 19:47:47 by jarregui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	change_path(char **tokens, t_shell *shell)
{
	const char	*path;

	if (!tokens[1])
	{
		path = getenv("HOME");
		if (!path)
		{
			ft_putendl_fd("minishell: cd: HOME not set", STDERR);
			shell->last_status = 1;
			return ;
		}
	}
	else if (tokens[2])
	{
		ft_putendl_fd("minishell: cd: too many arguments", STDERR);
		shell->last_status = 1;
		return ;
	}
	else
		path = tokens[1];
	if (chdir(path) != 0)
	{
		perror("minishell: cd");
		shell->last_status = 1;
		return;
	}
	free(shell->cwd);
	shell->cwd = getcwd(NULL, 0);
	if (!shell->cwd)
	{
		perror("minishell: cd: getcwd failed");
		shell->last_status = 1;
		return ;
	}
	ft_build_prompt(shell);
}

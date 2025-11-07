/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jarregui <jarregui@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/10 19:46:54 by jarregui          #+#    #+#             */
/*   Updated: 2025/11/07 19:20:10 by jarregui         ###   ########.fr       */
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
			return (error_custom(shell, 1, "cd: HOME not set", NULL));
	}
	else if (tokens[2])
		return (error_custom(shell, 1, "cd: too many arguments", NULL));
	else
		path = tokens[1];
	if (chdir(path) != 0)
	{
		error_custom(shell, 1, "cd: no such file or directory:", tokens[1]);
		return ;
	}
	free(shell->cwd);
	shell->cwd = getcwd(NULL, 0);
	if (!shell->cwd)
		return (error_custom(shell, 1, "cd: getcwd failed", NULL));
	ft_build_prompt(shell);
}

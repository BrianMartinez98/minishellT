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

	if (tokens[1] == NULL)
	{
		path = getenv("HOME");
		if (!path)
			perror("cd: HOME not set");
	}
	else
		path = tokens[1];
	if (chdir(path) != 0)
		perror("cd");
	if (shell->cwd)
		free(shell->cwd);
	shell->cwd = getcwd(NULL, 0);
	if (!shell->cwd)
		perror("getcwd failed");
	printf("Changed directory to %s\n", shell->cwd);
}

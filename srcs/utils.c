/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jarregui <jarregui@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/02 00:12:49 by jarregui          #+#    #+#             */
/*   Updated: 2025/10/06 23:00:26 by jarregui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	ft_build_prompt(t_shell *shell)
{
	char	*tmp;

	if (shell->prompt)
		free(shell->prompt);
	if (shell->cwd)
		free(shell->cwd);
	shell->prompt = NULL;
	shell->cwd = getcwd(NULL, 0);
	if (!shell->cwd)
		handle_error(GETCWD, shell);
	shell->prompt = ft_strjoin("\001\033[0;36m\033[1m\002", shell->cwd);
	tmp = shell->prompt;
	shell->prompt = ft_strjoin(tmp, " > \001\033[0m\002");
	free(tmp);
}

int	ft_error(const char *msg)
{
	if (msg)
		handle_error(MSG, NULL);
	return (1);
}

int	ft_isspace(char c)
{
	return (c == ' ' || c == '\t' || c == '\n'
		|| c == '\v' || c == '\f' || c == '\r');
}

void	filter_args(char **args, char ***tokens, t_shell *shell)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (args[i])
		i++;
	*tokens = malloc(sizeof(char *) * (i + 1));
	if (!*tokens)
		handle_error(MALLOCERROR, shell);
	i = 0;
	while (args[i])
	{
		if (!ft_strcmp(args[i], "<") || !ft_strcmp(args[i], ">")
			|| !ft_strcmp(args[i], ">>") || !ft_strcmp(args[i], "<<"))
		{
			i++;
		}
		else
			(*tokens)[j++] = args[i];
		i++;
	}
	(*tokens)[j] = NULL;
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jarregui <jarregui@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/02 00:12:49 by jarregui          #+#    #+#             */
/*   Updated: 2025/09/12 13:10:29 by jarregui         ###   ########.fr       */
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
		perror("Getcwd failed");
	shell->prompt = ft_strjoin("\001\033[0;36m\033[1m\002", shell->cwd);
	tmp = shell->prompt;
	shell->prompt = ft_strjoin(tmp, " > \001\033[0m\002");
	free(tmp);
}

int	ft_error(const char *msg)
{
	perror(msg);
	return (1);
}

int	ft_isspace(char c)
{
	return (c == ' ' || c == '\t' || c == '\n'
		|| c == '\v' || c == '\f' || c == '\r');
}

int	ft_next_span(char *s, size_t *i, t_span *sp)
{
	while (s[*i] && ft_isspace((unsigned char)s[*i]))
		(*i)++;
	if (!s[*i])
		return (0);
	sp->start = *i;
	while (s[*i] && !ft_isspace((unsigned char)s[*i]))
		(*i)++;
	sp->end = *i;
	return (1);
}

int	print_tokens(char **tokens)
{
	size_t	i;

	if (!tokens)
		return (1);
	i = 0;
	if (DEBUG)
	{
		printf("DEBUG: imprimiendo tokens que nos llegan: \n");
		while (tokens[i])
		{
			printf("Token[%zu]: '%s'\n", i, tokens[i]);
			i++;
		}
	}
	return (0);
}

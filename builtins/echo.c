/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jarregui <jarregui@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/02 00:14:01 by jarregui          #+#    #+#             */
/*   Updated: 2025/10/03 13:59:05 by jarregui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	ft_echo_var(t_shell *shell, char *token)
{
	char	*val;

	if (token[1] == '?' && token[2] == '\0')
		printf("%d", shell->last_status);
	else
	{
		val = ft_getenv(shell, token + 1);
		if (val)
			printf("%s", val);
	}
}

static int	ft_echo_token(t_shell *shell, char *token, int *suppress_nl,
		int *supress_first)
{
	if (ft_strcmp(token, "-n") == 0 && *supress_first)
	{
		*suppress_nl = 1;
		return (0);
	}
	*supress_first = 0;
	if (token[0] == '$')
		ft_echo_var(shell, token);
	else
		printf("%s", token);
	return (1);
}

static void	ft_strip_quotes(char **tokens)
{
	size_t	num_t;
	size_t	len;
	size_t	i;

	num_t = 0;
	len = 0;
	while (tokens[num_t])
		num_t++;
	if (num_t >= 2)
	{
		len = ft_strlen(tokens[num_t - 1]);
		if ((tokens[1][0] == '"' && tokens[num_t - 1][len - 1] == '"')
				|| (tokens[1][0] == '\'' && tokens[num_t - 1][len - 1] == '\''))
		{
			i = 0;
			while (tokens[1][i])
			{
				tokens[1][i] = tokens[1][i + 1];
				i++;
			}
			len = ft_strlen(tokens[num_t - 1]);
			tokens[num_t - 1][len - 1] = '\0';
		}
	}
}

void	ft_echo(char **tokens, t_shell *shell)
{
	int		i;
	int		suppress_nl;
	int		supress_first;
	int		printed;

	i = 1;
	suppress_nl = 0;
	supress_first = 1;
	ft_strip_quotes(tokens);
	while (tokens[i])
	{
		printed = ft_echo_token(shell, tokens[i],
				&suppress_nl, &supress_first);
		if (printed && tokens[i + 1])
			printf(" ");
		i++;
	}
	if (!suppress_nl)
		printf("\n");
}

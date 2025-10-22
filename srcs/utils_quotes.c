/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_quotes.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jarregui <jarregui@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/06 23:08:35 by jarregui          #+#    #+#             */
/*   Updated: 2025/10/22 11:16:17 by jarregui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	closed_quotes(char *line, int i)
{
	int		e;
	char	quote_char;
	int		in_quotes;

	quote_char = line[i];
	e = i + 1;
	in_quotes = 0;
	while (line[e])
	{
		if (line[e] == quote_char)
		{
			in_quotes = 1;
			break ;
		}
		else
			e++;
	}
	if (in_quotes)
		return (e);
	return (i);
}

int	is_quoted(char *cmd)
{
	size_t	len;

	len = ft_strlen(cmd);
	if (len > 1 && cmd[0] == '\'' && cmd[len - 1] == '\'')
		return (1);
	else if (len > 1 && cmd[0] == '"' && cmd[len - 1] == '"')
		return (2);
	return (0);
}

void	remove_quotes(char *cmd)
{
	size_t	i;

	i = 0;
	while (cmd[i])
	{
		cmd[i] = cmd[i + 1];
		i++;
	}
	cmd[i - 2] = '\0';
}

char	*ft_strjoin3(const char *s1, const char *s2, const char *s3)
{
	char	*tmp;
	char	*res;

	tmp = ft_strjoin(s1, s2);
	res = ft_strjoin(tmp, s3);
	free(tmp);
	return (res);
}

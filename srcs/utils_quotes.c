/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_quotes.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jarregui <jarregui@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/06 23:08:35 by jarregui          #+#    #+#             */
/*   Updated: 2025/10/06 23:08:42 by jarregui         ###   ########.fr       */
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

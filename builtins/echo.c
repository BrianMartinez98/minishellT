/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jarregui <jarregui@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/02 00:14:01 by jarregui          #+#    #+#             */
/*   Updated: 2025/10/22 09:47:53 by jarregui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	ft_echo_token(char *token, int *suppress_nl, int *supress_first)
{
	if (ft_strcmp(token, "-n") == 0 && *supress_first)
	{
		*suppress_nl = 1;
		return (0);
	}
	*supress_first = 0;
	printf("%s", token);
	return (1);
}

void	ft_echo(char **tokens)
{
	int		i;
	int		suppress_nl;
	int		supress_first;
	int		printed;

	i = 1;
	suppress_nl = 0;
	supress_first = 1;
	while (tokens[i])
	{
		printed = ft_echo_token(tokens[i], &suppress_nl, &supress_first);
		if (printed && tokens[i + 1])
			printf(" ");
		i++;
	}
	if (!suppress_nl)
		printf("\n");
}

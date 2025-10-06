/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_print.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jarregui <jarregui@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/06 22:59:24 by jarregui          #+#    #+#             */
/*   Updated: 2025/10/06 23:06:18 by jarregui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	print_tokens(char **tokens)
{
	size_t	i;

	if (!tokens)
		return (1);
	i = 0;
	while (tokens[i])
	{
		printf("		Token[%zu]: '%s'\n", i, tokens[i]);
		i++;
	}
	return (0);
}

void	print_tokens_array(t_shell *shell)
{
	size_t	i;

	if (DEBUG)
	{
		if (!shell->cmds)
			printf("\nDEBUG: No hay array de tokens!\n");
		else
		{
			printf("DEBUG: imprimiendo tokens que nos llegan:\n");
			printf("shell->cmds = \n[\n");
			i = 0;
			while (shell->cmds[i])
			{
				printf("	[\n");
				print_tokens(shell->cmds[i]);
				printf("	]\n");
				i++;
			}
			printf("];\n\n");
		}
	}
}

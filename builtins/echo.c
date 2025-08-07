/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jarregui <jarregui@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/02 00:14:01 by jarregui          #+#    #+#             */
/*   Updated: 2025/08/06 18:45:27 by jarregui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	ft_echo(t_shell *shell)
{
	int	i;
	int	suppress_nl;
	int	supress_first;

	print_tokens(shell->tokens);
	i = 1;
	suppress_nl = 0;
	supress_first = 1;
	while (shell->tokens[i])
	{
		if (ft_strcmp(shell->tokens[i], "-n") == 0 && supress_first)
			suppress_nl = 1;
		else
		{
			supress_first = 0;
			if (shell->tokens[i][0] == '$')
			{
				if (shell->tokens[i][1] == '?' && shell->tokens[i][2] == '\0')
					printf("%d", shell->last_status);  // echo $?
				else
				{
					char *val = ft_getenv(shell, shell->tokens[i] + 1); // salta '$'
					if (val)
						printf("%s", val);
				}
			}
			else
				printf("%s", shell->tokens[i]);
			if (shell->tokens[i + 1])
				printf(" ");
		}
		i++;
	}
	if (!suppress_nl)
		printf("\n");
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jarregui <jarregui@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/02 00:14:01 by jarregui          #+#    #+#             */
/*   Updated: 2025/09/10 19:41:51 by jarregui         ###   ########.fr       */
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

void	ft_echo(t_shell *shell)
{
	int		i;
	int		suppress_nl;
	int		supress_first;
	int		printed;

	i = 1;
	suppress_nl = 0;
	supress_first = 1;
	while (shell->tokens[i])
	{
		printed = ft_echo_token(shell, shell->tokens[i],
				&suppress_nl, &supress_first);
		if (printed && shell->tokens[i + 1])
			printf(" ");
		i++;
	}
	if (!suppress_nl)
		printf("\n");
}

// void	ft_echo(t_shell *shell)
// {
// 	int		i;
// 	int		suppress_nl;
// 	int		supress_first;
// 	char	*val;

// 	print_tokens(shell->tokens);
// 	i = 1;
// 	suppress_nl = 0;
// 	supress_first = 1;
// 	while (shell->tokens[i])
// 	{
// 		if (ft_strcmp(shell->tokens[i], "-n") == 0 && supress_first)
// 			suppress_nl = 1;
// 		else
// 		{
// 			supress_first = 0;
// 			if (shell->tokens[i][0] == '$')
// 			{
// 				if (shell->tokens[i][1] == '?' && shell->tokens[i][2] == '\0')
// 					printf("%d", shell->last_status);
// 				else
// 				{
// 					val = ft_getenv(shell, shell->tokens[i] + 1);
// 					if (val)
// 						printf("%s", val);
// 				}
// 			}
// 			else
// 				printf("%s", shell->tokens[i]);
// 			if (shell->tokens[i + 1])
// 				printf(" ");
// 		}
// 		i++;
// 	}
// 	if (!suppress_nl)
// 		printf("\n");
// }

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jarregui <jarregui@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/10 19:51:04 by jarregui          #+#    #+#             */
/*   Updated: 2025/09/11 14:20:03 by jarregui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	**filter_args(char **args)
{
	char	**res;
	int		count;
	int		i;
	int		j;

	i = 0;
	j = 0;
	count = 0;
	while (args[count])
		count++;
	res = malloc(sizeof(char *) * (count + 1));
	if (!res)
		return (NULL);
	while (args[i])
	{
		if ((strcmp(args[i], "<") == 0 || strcmp(args[i], ">") == 0
				|| strcmp(args[i], ">>") == 0
				|| strcmp(args[i], "<<") == 0) && args[i + 1])
			i += 2;
		else
			res[j++] = args[i++];
	}
	res[j] = NULL;
	return (res);
}

int	handle_redirections(t_shell *shell)
{
	int	i;
	int	value;

	i = 0;
	value = 0;
	while (shell->tokens[i])
	{
		if (shell->tokens[i + 1] && strcmp(shell->tokens[i], "<") == 0)
		{
			value = ft_left(shell, i + 1);
			i += 2;
		}
		else if (shell->tokens[i + 1] && strcmp(shell->tokens[i], ">") == 0)
		{
			value = ft_redirect(shell, i + 1);
			i += 2;
		}
		else if (shell->tokens[i + 1] && strcmp(shell->tokens[i], ">>") == 0)
		{
			value = ft_adding(shell, i + 1);
			i += 2;
		}
		else if (shell->tokens[i + 1] && strcmp(shell->tokens[i], "<<") == 0)
		{
			value = ft_leftleft(shell, i + 1);
			i += 2;
		}
		else
			i++;
	}
	return (value);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jarregui <jarregui@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/10 19:51:04 by jarregui          #+#    #+#             */
/*   Updated: 2025/09/12 12:45:22 by jarregui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	is_redirect(char *arg)
{
	if (!arg)
		return (0);
	if (strcmp(arg, "<") == 0
		|| strcmp(arg, ">") == 0
		|| strcmp(arg, ">>") == 0
		|| strcmp(arg, "<<") == 0)
		return (1);
	return (0);
}

static int	safe_strdup(char ***dst, int j, char *src)
{
	(*dst)[j] = ft_strdup(src);
	if (!(*dst)[j])
	{
		ft_free_array(dst);
		return (0);
	}
	return (1);
}

static void	fill_filtered(char ***args, char ***filt_args)
{
	int	i;
	int	j;
	int	skip_done;

	i = 0;
	j = 0;
	skip_done = 0;
	while ((*args)[i])
	{
		if (is_redirect((*args)[i]) && (*args)[i + 1] && !skip_done)
		{
			skip_done = 1;
			i += 2;
		}
		else
		{
			if (!safe_strdup(filt_args, j, (*args)[i]))
				return ;
			j++;
			i++;
		}
	}
	(*filt_args)[j] = NULL;
}

void	filter_args(char ***args, char ***filt_args)
{
	int		count;

	count = 0;
	ft_free_array(filt_args);
	while ((*args)[count])
		count++;
	if (count == 0)
		return ;
	*filt_args = malloc(sizeof(char *) * (count + 1));
	if (!*filt_args)
		return ;
	fill_filtered(args, filt_args);
}

int	handle_redirections(t_shell *shell)
{
	int	i;
	int	value;

	i = 0;
	value = 0;
	while (shell->tokens[i])
	{
		if (shell->tokens[i + 1] && is_redirect(shell->tokens[i]))
		{
			if (strcmp(shell->tokens[i], "<") == 0)
				value = ft_left(shell, i + 1);
			else if (strcmp(shell->tokens[i], ">") == 0)
				value = ft_redirect(shell, i + 1);
			else if (strcmp(shell->tokens[i], ">>") == 0)
				value = ft_adding(shell, i + 1);
			else if (strcmp(shell->tokens[i], "<<") == 0)
				value = ft_leftleft(shell, i + 1);
			i += 2;
		}
		else
			i++;
	}
	return (value);
}

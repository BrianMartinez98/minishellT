/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokens.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jarregui <jarregui@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/26 18:30:09 by jarregui          #+#    #+#             */
/*   Updated: 2025/11/07 15:04:11 by jarregui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	init_cmds(t_shell *shell)
{
	size_t	n;

	ft_free_matrix(&shell->cmds);
	n = count_pipes(shell) + 2;
	shell->cmds = ft_calloc(n, sizeof(char **));
	if (!shell->cmds)
		handle_error(MALLOCERROR, shell);
}

static void	fill_cmds(t_shell *shell)
{
	size_t	i;
	int		j;

	i = 0;
	j = 0;
	while (shell->line[i])
	{
		while (shell->line[i] == ' ')
			i++;
		if (shell->line[i] == '|')
		{
			i++;
			j++;
			continue ;
		}
		if (!shell->cmds[j])
		{
			shell->cmds[j] = malloc(sizeof(char *) * (MAX_TOKENS + 1));
			if (!shell->cmds[j])
				handle_error(MALLOCERROR, shell);
		}
		i += alloc_tokens(shell->cmds[j], &shell->line[i]);
	}
	shell->cmds[j + 1] = NULL;
}

void	split_line(t_shell *shell)
{
	init_cmds(shell);
	fill_cmds(shell);
	if (DEBUG)
	{
		printf(COL_MAGENTA "Tokens que nos llegan:\n" COL_RESET);
		print_tokens_array(shell);
	}
	parse_cmds(shell);
	remove_initial_empty_cmds(shell);
	if (DEBUG)
	{
		printf(COL_MAGENTA "Tras Elimin Quotes & Expan Variables:\n" COL_RESET);
		print_tokens_array(shell);
	}
}

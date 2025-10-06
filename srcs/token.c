/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jarregui <jarregui@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/26 18:30:09 by jarregui          #+#    #+#             */
/*   Updated: 2025/10/06 11:54:48 by jarregui         ###   ########.fr       */
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

void	fix_heredoc_no_command(char **cmd_tokens, t_shell *shell)
{
	int	i;

	if (!cmd_tokens)
		return ;
	if (cmd_tokens[0] && ft_strcmp(cmd_tokens[0], "<<") == 0)
	{
		i = 0;
		while (cmd_tokens[i])
			i++;
		while (i > 0)
		{
			cmd_tokens[i] = cmd_tokens[i - 1];
			i--;
		}
		cmd_tokens[0] = ft_strdup("cat");
		if (!cmd_tokens[0])
			handle_error(MALLOCERROR, shell);
	}
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
		fix_heredoc_no_command(shell->cmds[j], shell);
	}
	shell->cmds[j + 1] = NULL;
}

void	split_line(t_shell *shell)
{
	init_cmds(shell);
	fill_cmds(shell);
	if (DEBUG)
		print_tokens_array(shell);
}

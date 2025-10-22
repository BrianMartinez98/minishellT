/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jarregui <jarregui@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/26 18:30:09 by jarregui          #+#    #+#             */
/*   Updated: 2025/10/22 11:09:54 by jarregui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	expand_variables(t_shell *shell)
{
	size_t	i;
	size_t	j;
	int		expanded;

	expanded = 0;
	i = 0;
	while (shell->cmds[i])
	{
		j = 0;
		while (shell->cmds[i][j])
		{
			if (is_quoted(shell->cmds[i][j]) == 1)
				remove_quotes(shell->cmds[i][j]);
			else
			{
				if (is_quoted(shell->cmds[i][j]) == 2)
					remove_quotes(shell->cmds[i][j]);
				expanded += expand_var(shell, &shell->cmds[i][j]);
			}
			j++;
		}
		i++;
	}
	return (expanded);
}

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
	// int	expanded;

	init_cmds(shell);
	fill_cmds(shell);
	if (DEBUG)
	{
		printf("\033[0;35mDEBUG: split_line, tokens que nos llegan:\033[0m\n");
		print_tokens_array(shell);
	}
	expand_variables(shell);
	// expanded = expand_variables(shell);
	// if (DEBUG && expanded)
	if (DEBUG)
	{
		printf("\033[0;35mExpansión de variables:\033[0m\n");
		print_tokens_array(shell);
	}
}

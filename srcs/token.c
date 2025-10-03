/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jarregui <jarregui@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/26 18:30:09 by jarregui          #+#    #+#             */
/*   Updated: 2025/09/26 18:30:11 by jarregui         ###   ########.fr       */
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

// Inserta "cat" como primer token si el comando empieza por "<<"
void fix_heredoc_no_command(char **cmd_tokens)
{
	int i;

	if (!cmd_tokens)
		return;
	if (cmd_tokens[0] && ft_strcmp(cmd_tokens[0], "<<") == 0)
	{
		// Cuenta tokens
		i = 0;
		while (cmd_tokens[i])
			i++;
		
		// Mueve punteros de derecha a izquierda (sin sobreescribir)
		while (i > 0)
		{
			cmd_tokens[i] = cmd_tokens[i - 1];
			i--;
		}

		// Duplica "cat" para evitar problemas con free
		cmd_tokens[0] = strdup("cat");
		if (!cmd_tokens[0])
			// Manejar error malloc
			exit(1);
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
		fix_heredoc_no_command(shell->cmds[j]);
	}
	shell->cmds[j + 1] = NULL;
}

void	split_line(t_shell *shell)
{
	init_cmds(shell);
	fill_cmds(shell);
}

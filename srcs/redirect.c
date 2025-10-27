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
	if (ft_strcmp(arg, "<") == 0
		|| ft_strcmp(arg, ">") == 0
		|| ft_strcmp(arg, ">>") == 0)
		return (1);
	return (0);
}

int	handle_redirections(char **cmd, t_shell *shell)
{
	int	i;
	int	value;

	i = 0;
	value = 0;
	while (cmd[i])
	{
		if (cmd[i + 1] && is_redirect(cmd[i]))
		{
			if (ft_strcmp(cmd[i], "<") == 0)
				value = ft_left(shell, cmd, i + 1);
			else if (ft_strcmp(cmd[i], ">") == 0)
				value = ft_redirect(shell, cmd, i + 1);
			else if (ft_strcmp(cmd[i], ">>") == 0)
				value = ft_adding(shell, cmd, i + 1);
		}
		i++;
	}
	return (value);
}

int check_heredoc(char **cmd, t_shell *shell)
{
    int i = 0;
    int value = 0;

    while (cmd[i])
    {
        if (strcmp(cmd[i], "<<") == 0)
        {
            if (!cmd[i + 1] || cmd[i + 1][0] == '\0' || is_redir_token(cmd[i + 1]))
            {
                fprintf(stderr, "minishell: syntax error near unexpected token `newline'\n");
                return (-1);
            }
            value = ft_leftleft(shell, cmd, i + 1);
        }
        i++;
    }
    return (value);
}

void	restore_std(t_shell *shell)
{
	dup2(shell->stdin_save, STDIN_FILENO);
	dup2(shell->stdout_save, STDOUT_FILENO);
}

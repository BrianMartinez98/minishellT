/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokens_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jarregui <jarregui@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/10 22:52:05 by jarregui          #+#    #+#             */
/*   Updated: 2025/10/31 10:17:55 by jarregui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	count_pipes(t_shell *shell)
{
	int		count;
	int		i;

	i = 0;
	count = 0;
	while (shell->line[i])
	{
		if (shell->line[i] == '"' || shell->line[i] == '\'')
			i = closed_quotes(shell->line, i);
		else if (shell->line[i] == '|')
			count++;
		i++;
	}
	return (count);
}

char	*copy_token(char *line, t_span sp)
{
	size_t	len;
	char	*token;

	len = sp.end - sp.start;
	token = malloc(len + 1);
	if (!token)
		return (NULL);
	ft_strncpy(token, &line[sp.start], len);
	token[len] = '\0';
	return (token);
}

void	add_token(char **cmds, int *token_idx, char *line, t_span sp)
{
	char	*tok;

	tok = copy_token(line, sp);
	if (!tok)
	{
		perror("malloc token");
		return ;
	}
	cmds[(*token_idx)++] = tok;
	cmds[*token_idx] = NULL;
}

int	ft_next_span(char *s, size_t *i, t_span *sp)
{
	while (s[*i] && ft_isspace((unsigned char)s[*i]))
		(*i)++;
	if (!s[*i] || s[*i] == '|')
		return (0);

	sp->start = *i;

	// Detectar operadores de redirección (<, <<, >, >>)
	if (s[*i] == '<' || s[*i] == '>')
	{
		char op = s[*i];
		(*i)++;
		if (s[*i] == op) // << o >>
			(*i)++;
		sp->end = *i;
		return (1);
	}

	// Leer palabra normal
	while (s[*i] && !ft_isspace((unsigned char)s[*i]) && s[*i] != '|')
	{
		if (s[*i] == '<' || s[*i] == '>')
			break;

		if (s[*i] == '"' || s[*i] == '\'')
		{
			size_t prev = *i;
			*i = closed_quotes(s, *i);
			if (*i == prev) // sin cierre -> evitamos loop
				break;
		}
		(*i)++;
	}
	sp->end = *i;
	return (1);
}


/* Version anterior

int	ft_next_span(char *s, size_t *i, t_span *sp)
{
	while (s[*i] && ft_isspace((unsigned char)s[*i]))
		(*i)++;
	if (!s[*i] || s[*i] == '|')
		return (0);
	sp->start = *i;
	if (s[*i] == '<' || s[*i] == '>')
	{
		char	op = s[*i];
		(*i)++;
		if (s[*i] == op)
			(*i)++;
		sp->end = *i;
		return (1);
	}
	while (s[*i] && !ft_isspace((unsigned char)s[*i]) && s[*i] != '|')
	{
		if (s[*i] == '<' || s[*i] == '>')
			break ;
		if (s[*i] == '"' || s[*i] == '\'')
			*i = closed_quotes(s, *i);
		(*i)++;
	}
	sp->end = *i;
	return (1);
}
*/

size_t	alloc_tokens(char **cmds, char *line)
{
	size_t	i;
	int		token_idx;
	t_span	sp;

	i = 0;
	token_idx = 0;
	while (line[i] && line[i] != '|')
	{
		if (!ft_next_span(line, &i, &sp))
			break ;
		add_token(cmds, &token_idx, line, sp);
	}
	cmds[token_idx] = NULL;
	return (i);
}

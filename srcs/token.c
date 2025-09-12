/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jarregui <jarregui@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/10 22:52:05 by jarregui          #+#    #+#             */
/*   Updated: 2025/09/12 13:12:19 by jarregui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	init_tokens(t_shell *shell, size_t bufsize)
{
	if (shell->tokens)
		ft_free_array(&shell->tokens);
	shell->tokens = malloc(bufsize * sizeof(char *));
	if (!shell->tokens)
	{
		perror("malloc shell->tokens");
		return (0);
	}
	return (1);
}

static char	*copy_token(char *line, size_t start, size_t end)
{
	size_t	len;
	char	*token;

	len = end - start;
	token = malloc(len + 1);
	if (!token)
		return (NULL);
	ft_strncpy(token, &line[start], len);
	token[len] = '\0';
	return (token);
}

static char	**resize_tokens(char **tokens, size_t old_size, size_t new_size)
{
	char	**new_tokens;
	size_t	i;

	new_tokens = malloc(new_size * sizeof(char *));
	if (!new_tokens)
		return (NULL);
	i = 0;
	while (i < old_size)
	{
		new_tokens[i] = tokens[i];
		i++;
	}
	free(tokens);
	return (new_tokens);
}

static int	add_token(t_shell *shell, size_t *pos, size_t *bs, t_span sp)
{
	char	*tok;

	tok = copy_token(shell->line, sp.start, sp.end);
	if (!tok)
	{
		ft_free_array(&shell->tokens);
		return (0);
	}
	shell->tokens[(*pos)++] = tok;
	if (*pos >= *bs)
	{
		*bs *= 2;
		shell->tokens = resize_tokens(shell->tokens, *pos, *bs);
		if (!shell->tokens)
		{
			perror("malloc new_tokens");
			return (0);
		}
	}
	return (1);
}

void	split_line(t_shell *shell)
{
	size_t	pos;
	size_t	bs;
	size_t	i;
	t_span	sp;

	bs = BUFSIZ;
	pos = 0;
	i = 0;
	if (!init_tokens(shell, bs))
		return ;
	while (ft_next_span(shell->line, &i, &sp))
		if (!add_token(shell, &pos, &bs, sp))
			return ;
	shell->tokens[pos] = NULL;
}

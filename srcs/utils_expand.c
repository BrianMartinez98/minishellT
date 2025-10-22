/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_expand.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jarregui <jarregui@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/21 22:30:31 by jarregui          #+#    #+#             */
/*   Updated: 2025/10/22 11:42:47 by jarregui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"


static int	expand(t_shell *shell, char **cmd, const char *key,
	size_t start, size_t end)
{
	char	*value;
	char	*prefix;
	char	*suffix;
	char	*tmp;
	char	*new_cmd;

	if (!cmd || !*cmd || !key)
		return (0);
	if (key[0] == '?')
		value = ft_itoa(shell->last_status);
	else
		value = ft_getenv(shell, key);
	if (!value)
		return (0);
	prefix = ft_substr(*cmd, 0, start);
	suffix = ft_substr(*cmd, end, ft_strlen(*cmd) - end);
	tmp = ft_strjoin(prefix, value);
	if (ft_strcmp(key, "?") == 0) 
		free(value);
	free(*cmd);
	new_cmd = ft_strjoin(tmp, suffix);
	free(prefix);
	free(suffix);
	free(tmp);
	*cmd = new_cmd;
	return (1);
}

int	expand_var(t_shell *shell, char **cmd)
{
	size_t		start;
	size_t		end;
	size_t		i;
	const char	*key;
	int			expanded;

	expanded = 0;
	i = 0;
	while ((*cmd)[i])
	{
		if ((*cmd)[i] == '$')
		{
			start = i;
			i++;
			if ((*cmd)[i] == '?' )
				end = i + 1;
			else
			{
				while ((*cmd)[i] && (ft_isalnum((*cmd)[i]) || (*cmd)[i] == '_'))
					i++;
				end = i;
			}
			if (end > start)
			{
				key = ft_substr((*cmd), start + 1, end - (start + 1));
				expanded += expand(shell, cmd, key, start, end);
				free((void *)key);
				i = start + 1;
			}
		}
		else
			i++;
	}
	return (expanded);
}

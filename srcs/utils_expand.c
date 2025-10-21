/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_expand.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jarregui <jarregui@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/21 22:30:31 by jarregui          #+#    #+#             */
/*   Updated: 2025/10/21 23:21:34 by jarregui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	is_single_quoted(char *cmd)
{
	size_t	len;

	len = ft_strlen(cmd);
	if (len > 1 && cmd[0] == '\'' && cmd[len - 1] == '\'')
		return (1);
	return (0);
}

int	is_double_quoted(char *cmd)
{
	size_t	len;

	len = ft_strlen(cmd);
	if (len > 1 && cmd[0] == '"' && cmd[len - 1] == '"')
		return (1);
	return (0);
}

void	remove_quotes(char *cmd)
{
	size_t	i;

	i = 0;
	while (cmd[i])
	{
		cmd[i] = cmd[i + 1];
		i++;
	}
	cmd[i - 2] = '\0';
}

static int	expand(t_shell *shell, char **cmd, const char *key, size_t start, size_t end)
{
	char	*value;
	char	*prefix;
	char	*suffix;
	char	*tmp;
	char	*new_cmd;

	value = ft_getenv(shell, key);
	if (value)
	{
		printf("VALOR CLAVE: '%s'\n", value);

		prefix = ft_substr(*cmd, 0, start);
		suffix = ft_substr(*cmd, end, ft_strlen(*cmd) - end);
		tmp = ft_strjoin(prefix, value);
		new_cmd = ft_strjoin(tmp, suffix);
		free(prefix);
		free(suffix);
		free(tmp);
		free(*cmd);
		*cmd = new_cmd;
		return (1);
	}
	else
	{
		printf("VALOR CLAVE NO EXISTE\n");
		return (0);
	}
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
			while ((*cmd)[i] && (ft_isalnum((*cmd)[i]) || (*cmd)[i] == '_'))
				i++;
			end = i;
			printf("START: %zu, END: %zu\n", start, end);
			
			if (end > start)
			{
				key = ft_substr((*cmd), start + 1, end - (start + 1));
				printf("CLAVE ENCONTRADA: '%s'\n", key);
				expanded += expand(shell, cmd, key, start, end);
				free((void *)key);
				i = start + 1; // Restart scanning after expansion
			}
		}
		else
			i++;
	}
	return (expanded);
}

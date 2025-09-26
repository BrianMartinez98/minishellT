/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jarregui <jarregui@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 13:54:59 by jarregui          #+#    #+#             */
/*   Updated: 2025/09/10 19:32:02 by jarregui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	is_valid_key(const char *key)
{
	int	i;

	i = 0;
	if (!key || (!ft_isalpha(key[0]) && key[0] != '_'))
		return (0);
	while (key[i] && key[i] != '=')
	{
		if (!ft_isalnum(key[i]) && key[i] != '_')
			return (0);
		i++;
	}
	return (1);
}

static t_env	*find_env_node(t_env *head, const char *key, size_t key_len,
	t_env **plast)
{
	t_env	*cur;

	*plast = NULL;
	cur = head;
	while (cur)
	{
		if (ft_strncmp(cur->value, key, key_len) == 0
			&& cur->value[key_len] == '=')
			return (cur);
		*plast = cur;
		cur = cur->next;
	}
	return (NULL);
}

static char	*dup_key(const char *entry)
{
	char	*eq;

	eq = ft_strchr(entry, '=');
	if (!eq)
		return (NULL);
	return (ft_substr(entry, 0, eq - entry));
}

void	ft_env_set(t_shell *shell, const char *entry)
{
	t_env	*last;
	t_env	*node;
	char	*key;
	size_t	key_len;

	key = dup_key(entry);
	if (!key)
		return ;
	key_len = ft_strlen(key);
	node = find_env_node(shell->env, key, key_len, &last);
	if (node)
	{
		free(node->value);
		node->value = ft_strdup(entry);
		free(key);
		return ;
	}
	append_env(shell, entry, last);
	free(key);
}

void	ft_export(char **tokens, t_shell *shell)
{
	char	*equal;
	int		i;

	i = 1;
	while (tokens[i])
	{
		equal = ft_strchr(tokens[i], '=');
		if (equal)
		{
			if (!is_valid_key(tokens[i]))
			{
				printf("export: `%s': not a valid identifier\n",
					tokens[i]);
				shell->last_status = 1;
			}
			else
			{
				ft_env_set(shell, tokens[i]);
				shell->last_status = 0;
			}
		}
		i++;
	}
}

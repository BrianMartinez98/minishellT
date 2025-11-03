/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jarregui <jarregui@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 13:54:59 by jarregui          #+#    #+#             */
/*   Updated: 2025/11/03 13:47:23 by jarregui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	is_valid_start(char c)
{
	return (ft_isalpha(c) || c == '_');
}

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

static int	find_env_index(char **envp, const char *key)
{
	int		i;
	size_t	key_len;

	if (!envp)
		return (-1);
	i = 0;
	key_len = ft_strlen(key);
	while (envp[i])
	{
		if (ft_strncmp(envp[i], key, key_len) == 0 && envp[i][key_len] == '=')
			return (i);
		i++;
	}
	return (-1);
}

static char	*get_key(const char *entry)
{
	char	*eq;

	eq = ft_strchr(entry, '=');
	if (!eq)
		return (NULL);
	return (ft_substr(entry, 0, eq - entry));
}

void	ft_env_set(t_shell *shell, const char *entry, char *equal)
{
	int		index;
	char	*key;
	char	*value;
	char	*new_entry;

	if (!shell || !entry)
		return ;
	key = get_key(entry);
	if (!key)
		return ;
	value = ft_strdup(equal + 1);
	new_entry = NULL;
	index = find_env_index(shell->env, key);
	if (index != -1)
	{
		free(shell->env[index]);
		shell->env[index] = ft_strdup(entry);
	}
	else
		append_env(shell, entry);
	free(value);
	free(key);
	free(new_entry);
}

void	ft_export(char **tokens, t_shell *shell)
{
	char	*equal;
	int		i;
	char	*key;

	i = 1;
	shell->last_status = 0;
	if (!tokens[i])
		return (ft_print_env(shell));
	while (tokens[i])
	{
		equal = ft_strchr(tokens[i], '=');
		if (equal)
			key = ft_substr(tokens[i], 0, equal - tokens[i]);
		else
			key = ft_strdup(tokens[i]);
		if (!is_valid_start(key[0]))
			error_custom(shell, 1, "export: not a valid identifier", key);
		else if (!is_valid_key(key))
			error_custom(shell, 1, "export: not valid in this context:", key);
		else
			ft_env_set(shell, tokens[i], equal);
		free(key);
		i++;
	}
}

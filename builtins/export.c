/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jarregui <jarregui@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 13:54:59 by jarregui          #+#    #+#             */
/*   Updated: 2025/08/04 19:29:50 by jarregui         ###   ########.fr       */
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

void	ft_env_set(t_shell *shell, const char *entry)
{
	t_env	*env = shell->env;
	t_env	*new;
	char	*key = ft_substr(entry, 0, ft_strchr(entry, '=') - entry);

	while (env)
	{
		if (ft_strncmp(env->value, key, ft_strlen(key)) == 0 &&
			env->value[ft_strlen(key)] == '=')
		{
			free(env->value);
			env->value = ft_strdup(entry); // actualiza
			free(key);
			return ;
		}
		env = env->next;
	}
	free(key);
	// si no existe, lo añadimos
	new = malloc(sizeof(t_env));
	new->value = ft_strdup(entry);
	new->next = shell->env;
	shell->env = new;
}

void	ft_export(t_shell *shell, char **tokens)
{
	int	i = 1;

	while (tokens[i])
	{
		if (!is_valid_key(tokens[i]))
		{
			printf("export: `%s': not a valid identifier\n", tokens[i]);
			shell->last_status = 1;
		}
		else
		{
			// busca si ya existe
			ft_env_set(shell, tokens[i]); // actualiza o añade
			shell->last_status = 0;
		}
		i++;
	}
}

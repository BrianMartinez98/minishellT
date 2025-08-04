/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environment.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jarregui <jarregui@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 13:54:59 by jarregui          #+#    #+#             */
/*   Updated: 2025/08/04 18:09:39 by jarregui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_env_init(t_shell *shell, char **env_array)
{
	t_env	*current;
	t_env	*new;
	int		i;

	current = malloc(sizeof(t_env));
	if (!current)
		return (ft_error("Mem alloc failed for env node"));
	current->value = ft_strdup(env_array[0]);
	current->next = NULL;
	shell->env = current;
	i = 1;
	while (env_array && env_array[0] && env_array[i])
	{
		new = malloc(sizeof(t_env));
		if (!new)
			return (ft_error("Mem alloc failed for env node"));
		new->value = ft_strdup(env_array[i]);
		new->next = NULL;
		current->next = new;
		current = new;
		i++;
	}
	return (0);
}

void	ft_print_env(t_shell *shell)
{
	t_env	*current;

	if (!shell->env)
	{
		printf("No environment variables set.\n");
		return ;
	}
	current = shell->env;
	while (current)
	{
		printf("%s\n", current->value);
		current = current->next;
	}
}

char	*ft_getenv(t_shell *shell, const char *key)
{
	t_env	*current;
	size_t	len;

	current = shell->env;
	len = ft_strlen(key);
	while (current)
	{
		if (ft_strncmp(current->value, key, len) == 0
			&& current->value[len] == '=')
			return (current->value + len + 1); // salta "KEY="
		current = current->next;
	}
	return (NULL);
}

void	ft_free_env(t_shell *shell)
{
	t_env	*next;

	while (shell->env)
	{
		next = shell->env->next;
		free(shell->env->value);
		free(shell->env);
		shell->env = next;
	}
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

void	ft_unset(t_shell *shell, char **tokens)
{
	int		i = 1;
	t_env	*prev = NULL;
	t_env	*curr = shell->env;

	while (tokens[i])
	{
		if (!is_valid_key(tokens[i]))
		{
			printf("minishell: unset: `%s': not a valid identifier\n", tokens[i]);
			shell->last_status = 1;
			i++;
			continue;
		}
		prev = NULL;
		curr = shell->env;
		while (curr)
		{
			if (ft_strncmp(curr->value, tokens[i], ft_strlen(tokens[i])) == 0
				&& curr->value[ft_strlen(tokens[i])] == '=')
			{
				if (prev)
					prev->next = curr->next;
				else
					shell->env = curr->next;
				free(curr->value);
				free(curr);
				break;
			}
			prev = curr;
			curr = curr->next;
		}
		shell->last_status = 0;
		i++;
	}
}

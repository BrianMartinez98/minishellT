/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environment.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jarregui <jarregui@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 13:54:59 by jarregui          #+#    #+#             */
/*   Updated: 2025/09/10 19:34:06 by jarregui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

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
			return (current->value + len + 1);
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

int	append_env(t_shell *shell, const char *entry, t_env *last)
{
	t_env	*new;

	new = (t_env *)malloc(sizeof(t_env));
	if (!new)
		return (0);
	new->value = ft_strdup(entry);
	if (!new->value)
	{
		free(new);
		return (0);
	}
	new->next = NULL;
	if (last)
		last->next = new;
	else
		shell->env = new;
	return (1);
}

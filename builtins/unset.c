/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jarregui <jarregui@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 13:54:59 by jarregui          #+#    #+#             */
/*   Updated: 2025/09/09 13:43:30 by jarregui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	is_token_valid_or_warn(char *key, t_shell *shell)
{
	if (!is_valid_key(key))
	{
		printf("unset: `%s': not a valid identifier\n", key);
		shell->last_status = 1;
		return (0);
	}
	return (1);
}

void	unset_key_from_env(char *key, t_shell *shell)
{
	t_env	*prev;
	t_env	*curr;
	size_t	key_len;

	prev = NULL;
	curr = shell->env;
	key_len = ft_strlen(key);
	while (curr)
	{
		if (ft_strncmp(curr->value, key, key_len) == 0
			&& curr->value[key_len] == '=')
		{
			if (prev)
				prev->next = curr->next;
			else
				shell->env = curr->next;
			free(curr->value);
			free(curr);
			return ;
		}
		prev = curr;
		curr = curr->next;
	}
}

void	ft_unset(char **tokens, t_shell *shell)
{
	int	i;

	i = 1;
	while (tokens[i])
	{
		if (!is_token_valid_or_warn(tokens[i], shell))
		{
			i++;
			continue ;
		}
		unset_key_from_env(tokens[i], shell);
		shell->last_status = 0;
		i++;
	}
}

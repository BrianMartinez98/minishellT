/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jarregui <jarregui@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 13:54:59 by jarregui          #+#    #+#             */
/*   Updated: 2025/08/04 19:29:50 by jarregui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	ft_unset(t_shell *shell, char **tokens)
{
	int		i = 1;
	t_env	*prev = NULL;
	t_env	*curr = shell->env;

	while (tokens[i])
	{
		if (!is_valid_key(tokens[i]))
		{
			printf("unset: `%s': not a valid identifier\n", tokens[i]);
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

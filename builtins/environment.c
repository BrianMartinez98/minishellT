/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environment.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jarregui <jarregui@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 13:54:59 by jarregui          #+#    #+#             */
/*   Updated: 2025/10/20 22:45:52 by jarregui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	ft_env_init(t_shell *shell, char **env_array)
{
	int		i;
	int		num_variables;

	i = 0;
	num_variables = 0;
	if (!env_array)
		shell->env = NULL;
	else
	{
		while (env_array[num_variables])
			num_variables++;
		shell->env = malloc(sizeof(char *) * (num_variables + 1));
		if (!shell->env)
			return (ft_error("Memory allocation failed for env copy"));
		while (i < num_variables)
		{
			shell->env[i] = ft_strdup(env_array[i]);
			if (!shell->env[i])
				return (ft_error("Memory allocation failed for env entry"));
			i++;
		}
		shell->env[num_variables] = NULL;
	}
	return (num_variables);
}

void	ft_print_env(t_shell *shell)
{
	int	i;

	if (!shell->env)
	{
		printf("No environment variables set.\n");
		return ;
	}
	i = 0;
	if (DEBUG)
		printf("\033[0;35m[\033[0m\n");
	while (shell->env[i])
	{
		if (DEBUG)
			printf("\033[0;35m  \"\033[0m");
		if (DEBUG)
			printf("%s\033[0;35m\"\033[0m\n", shell->env[i]);
		else
			printf("%s\n", shell->env[i]);
		i++;
	}
	if (DEBUG)
		printf("\033[0;35m  NULL\n]\033[0m\n");
}

char	*ft_getenv(t_shell *shell, const char *key)
{
	int		i;
	size_t	len;

	if (!shell || !shell->env || !key)
		return (NULL);
	i = 0;
	len = ft_strlen(key);
	while (shell->env[i])
	{
		if (ft_strncmp(shell->env[i], key, len) == 0
			&& shell->env[i][len] == '=')
			return (shell->env[i] + len + 1);
		i++;
	}
	return (NULL);
}

void	ft_free_env(t_shell *shell)
{
	int	i;

	if (!shell->env)
		return ;
	i = 0;
	while (shell->env[i])
	{
		free(shell->env[i]);
		i++;
	}
	free(shell->env);
	shell->env = NULL;
}

int	append_env(t_shell *shell, const char *entry)
{
	size_t	count;
	char	**new_vars;
	size_t	i;

	if (!shell || !entry)
		return (0);
	count = 0;
	while (shell->env && shell->env[count])
		count++;
	new_vars = malloc(sizeof(char *) * (count + 2));
	if (!new_vars)
		return (ft_error("Malloc failed for append_env"));
	i = 0;
	while (i < count)
	{
		new_vars[i] = shell->env[i];
		i++;
	}
	new_vars[count] = ft_strdup(entry);
	if (!new_vars[count])
	{
		free(new_vars);
		return (ft_error("Malloc failed for append_env"));
	}
	new_vars[count + 1] = NULL;
	free(shell->env);
	shell->env = new_vars;
	return (1);
}

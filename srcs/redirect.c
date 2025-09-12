/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jarregui <jarregui@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/10 19:51:04 by jarregui          #+#    #+#             */
/*   Updated: 2025/09/12 11:32:02 by jarregui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	filter_args(char ***args, char ***filt_args)
{
	int		count;
	int		i;
	int		j;

	i = 0;
	j = 0;
	count = 0;
	ft_free_array(filt_args);
	while ((*args)[count])
		count++;
	if (count == 0)
		return ;
	*filt_args = malloc(sizeof(char *) * (count + 1));
	if (!*filt_args )
		return ;
	while ((*args)[i])
	{
		if ((strcmp((*args)[i], "<") == 0 || strcmp((*args)[i], ">") == 0
				|| strcmp((*args)[i], ">>") == 0
				|| strcmp((*args)[i], "<<") == 0) && (*args)[i + 1])
			i += 2;
		else
		{
			(*filt_args)[j] = ft_strdup((*args)[i]);   // 👈 copiar string
			if (!(*filt_args)[j])
			{
				ft_free_array(filt_args);      // liberar en caso de error
				return ;
			}
			j++;
			i++;
		}
	}
	(*filt_args)[j] = NULL;
}


// char	**filter_args(char **args)
// {
// 	char	**res;
// 	int		count;
// 	int		i;
// 	int		j;

// 	i = 0;
// 	j = 0;
// 	count = 0;
// 	while (args[count])
// 		count++;
// 	res = malloc(sizeof(char *) * (count + 1));
// 	if (!res)
// 		return (NULL);
// 	while (args[i])
// 	{
// 		if ((strcmp(args[i], "<") == 0 || strcmp(args[i], ">") == 0
// 				|| strcmp(args[i], ">>") == 0
// 				|| strcmp(args[i], "<<") == 0) && args[i + 1])
// 			i += 2;
// 		else
// 		{
// 			res[j] = ft_strdup(args[i]);   // 👈 copiar string
// 			if (!res[j])
// 			{
// 				ft_free_array(&res);      // liberar en caso de error
// 				return (NULL);
// 			}
// 			j++;
// 			i++;
// 		}
// 	}
// 	res[j] = NULL;
// 	return (res);
// }

int	handle_redirections(t_shell *shell)
{
	int	i;
	int	value;

	i = 0;
	value = 0;
	while (shell->tokens[i])
	{
		if (shell->tokens[i + 1] && strcmp(shell->tokens[i], "<") == 0)
		{
			value = ft_left(shell, i + 1);
			i += 2;
		}
		else if (shell->tokens[i + 1] && strcmp(shell->tokens[i], ">") == 0)
		{
			value = ft_redirect(shell, i + 1);
			i += 2;
		}
		else if (shell->tokens[i + 1] && strcmp(shell->tokens[i], ">>") == 0)
		{
			value = ft_adding(shell, i + 1);
			i += 2;
		}
		else if (shell->tokens[i + 1] && strcmp(shell->tokens[i], "<<") == 0)
		{
			value = ft_leftleft(shell, i + 1);
			i += 2;
		}
		else
			i++;
	}
	return (value);
}

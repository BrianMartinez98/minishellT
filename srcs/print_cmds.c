/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_cmds.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jarregui <jarregui@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/26 18:28:18 by jarregui          #+#    #+#             */
/*   Updated: 2025/09/26 18:29:26 by jarregui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>

void	print_cmds(char ***cmds)
{
	int	i;
	int	j;

	i = 0;
	if (!cmds)
	{
		printf("cmds is NULL\n");
		return ;
	}
	while (cmds[i])
	{
		printf("Comando %d:\n", i);
		j = 0;
		while (cmds[i][j])
		{
			printf("  Arg %d: '%s'\n", j, cmds[i][j]);
			j++;
		}
		i++;
	}
}

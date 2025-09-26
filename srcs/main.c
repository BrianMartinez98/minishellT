/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jarregui <jarregui@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/01 23:47:36 by jarregui          #+#    #+#             */
/*   Updated: 2025/09/10 19:49:34 by jarregui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	main(int ac, char **av, char **env)
{
	t_shell	shell;

	(void)ac;
	(void)av;
	ft_init_shell(&shell, env);
	while (shell.exit == 0)
	{
		ft_build_prompt(&shell);
		ft_setup_signals_prompt();
		ft_readline(&shell);
		if (!shell.line)
		{
			shell.eof = 1;
			break ;
		}
		if (*shell.line == '\0')
			continue ;
		ft_add_history(&shell);
		split_line(&shell); // DONE
		if (!shell.cmds || !shell.cmds[0])
			continue ;
		ft_execute_pipes(&shell); // wORKING hERE; pANDORA
	}
	ft_exit_shell(&shell);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jarregui <jarregui@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/01 23:47:36 by jarregui          #+#    #+#             */
/*   Updated: 2025/10/07 23:54:11 by jarregui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	main(int ac, char **av, char **env)
{
	t_shell	shell;

	(void)ac;
	(void)av;
	ft_init_shell(&shell, env);
	ft_build_prompt(&shell);
	while (shell.exit == 0)
	{
		ft_setup_signals_prompt();
		ft_readline(&shell);
		if (!shell.line && !shell.exit)
			shell.eof = 1;
		if (!shell.line || shell.exit)
			break ;
		if (*shell.line == '\0')
			continue ;
		ft_add_history(&shell);
		split_line(&shell);
		if (!shell.cmds || !shell.cmds[0])
			continue ;
		ft_execute_pipes(&shell);
	}
	ft_exit_shell(&shell);
}

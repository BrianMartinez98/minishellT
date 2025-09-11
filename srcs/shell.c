/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jarregui <jarregui@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/09 15:00:40 by jarregui          #+#    #+#             */
/*   Updated: 2025/09/11 13:53:33 by jarregui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_shell	*ft_get_shell_address(t_shell *new_shell)
{
	static t_shell	*shell_ptr = NULL;

	if (new_shell)
		shell_ptr = new_shell;
	return (shell_ptr);
}

void	ft_disable_echoctl(t_shell *shell)
{
	struct termios	term;

	tcgetattr(STDIN_FILENO, &term);
	shell->termios_saved = term;
	term.c_lflag &= ~ECHOCTL;
	tcsetattr(STDIN_FILENO, TCSANOW, &term);
}

void	ft_restore_term_settings(t_shell *shell)
{
	tcsetattr(STDIN_FILENO, TCSANOW, &shell->termios_saved);
}

void	ft_init_shell(t_shell *shell, char **env)
{
	ft_env_init(shell, env);
	shell->cwd = NULL;
	shell->prompt = NULL;
	shell->hist = NULL;
	shell->last_status = 0;
	shell->exit = 0;
	shell->eof = 0;
	shell->line = NULL;
	shell->tokens = NULL;
	shell->clean_args = NULL;
	shell->stdin_save = 0;
	shell->stdout_save = 0;
	shell->fd = 0;
	shell->line2 = NULL;
	shell->len = 0;
	shell->nread = 0;
	shell->cmds = NULL;
	ft_get_shell_address(shell);
	ft_disable_echoctl(shell);
}

int	ft_exit_shell(t_shell *shell)
{
	if (shell->prompt)
		free(shell->prompt);
	if (shell->cwd)
		free(shell->cwd);
	ft_free_history(shell);
	ft_free_env(shell);
	ft_free_line(shell);
	ft_free_tokens(shell->tokens);
	ft_free_tokens(shell->clean_args);
	ft_restore_term_settings(shell);
	if (DEBUG && shell->eof)
		write(1, "\n	Pulsada tecla Ctrl+D (EOF)\n", 30);
	printf("\033[1;32mExiting minishell...\033[0m\n");
	exit(0);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jarregui <jarregui@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/07 14:13:22 by jarregui          #+#    #+#             */
/*   Updated: 2025/09/10 19:30:42 by jarregui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# define SPACE	"\t\n\v\f\r "

# define STDIN 0
# define STDOUT 1
# define STDERR 2

# ifndef DEBUG
#  define DEBUG 0
# endif

# ifndef ECHOCTL
#  define ECHOCTL 0001000
# endif

# include "libs/libft/libft.h"
# include <fcntl.h>
# include <unistd.h>
# include <string.h>
# include <stdio.h>
# include <ctype.h>
# include <string.h>
# include <stdlib.h>
# include <unistd.h>
# include <signal.h>
# include <errno.h>
# include <termios.h>
# include <sys/wait.h>
# include <readline/readline.h>
# include <readline/history.h>

typedef struct s_token //para los tokens que nos pasen
{
	char			*str;
	int				type;
	struct s_token	*prev;
	struct s_token	*next;
}	t_token;

typedef struct s_env //para las variables de entorno
{
	char			*value;
	struct s_env	*next;
}	t_env;

typedef struct s_hist //para el historial de comandos
{
	char			*line;
	struct s_hist	*next;
	struct s_hist	*prev;
}	t_hist;

typedef struct s_shell //para los datos que necesitaremos en la minishell
{
	t_env			*env;
	t_hist			*hist;
	struct termios	termios_saved;
	char			*cwd;
	char			*prompt;
	int				exit;
	int				eof;
	int				last_status;
	char			*line;
	char			**tokens;
}	t_shell;

//builtins/cd.c
void	change_path(t_shell *shell, char **tokens);

//builtins/echo.c
void	ft_echo(t_shell *shell);

//builtins/environment.c
int		ft_env_init(t_shell *shell, char **env_array);
void	ft_print_env(t_shell *shell);
char	*ft_getenv(t_shell *shell, const char *key);
void	ft_free_env(t_shell *shell);
int		append_env(t_shell *shell, const char *entry, t_env *last);

//builtins/export.c
int		is_valid_key(const char *key);
void	ft_env_set(t_shell *shell, const char *entry);
void	ft_export(t_shell *shell);

//builtins/history.c
void	ft_add_history(t_shell *shell);
void	ft_print_history(t_shell *shell);
void	ft_free_history(t_shell *shell);

//builtins/unset.c
void	ft_unset(t_shell *shell);

//srcs/execute.c
int		ft_execute(t_shell *shell);

//srcs/readline.c
void	ft_readline(t_shell *shell);
void	ft_free_line(t_shell *shell);

//srcs/redirect.c
// static int	duplication(char *filename);
// void		redirect(char **array);

//shell.c
t_shell	*ft_get_shell_address(t_shell *shell);
void	ft_disable_echoctl(t_shell *shell);
void	ft_restore_term_settings(t_shell *shell);
void	ft_init_shell(t_shell *shell, char **env);
int		ft_exit_shell(t_shell *shell);

//signals.c
void	ft_sigint_handler(int sig);
void	ft_sigquit_handler(int sig);
void	ft_setup_signals_prompt(void);
void	ft_setup_signals_child(void);

//token.c
void	split_line(t_shell *shell);
int		print_tokens(char **tokens);
void	ft_free_tokens(char **tokens);

//utils.c
void	ft_build_prompt(t_shell *shell);
int		ft_error(const char *msg);
int		ft_isspace(char c);

//redirect.c
int		dupp(char *filename, int dest, int flags);
char	**filter_args(char **array);

//redirection_utils.c
char	**filter_args(char **args);
int		handle_redirections(char **args);

//pipes.c
int		ft_execute_pipes(t_shell *shell);

#endif
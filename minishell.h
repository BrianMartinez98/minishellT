/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jarregui <jarregui@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/07 14:13:22 by jarregui          #+#    #+#             */
/*   Updated: 2025/10/07 22:48:35 by jarregui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# define SPACE			"\t\n\v\f\r "

# define MAX_TOKENS 256

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

typedef struct s_span
{
	size_t			start;
	size_t			end;
}	t_span;

typedef enum e_errorlst //para los errores que pueden surgir
{
	PIPES,
	MALLOCERROR,
	FD,
	GETCWD,
	MSG,
	NCMD,
}	t_errorlst;

typedef struct s_shell //para los datos que necesitaremos en la minishell
{
	char			**env;
	t_hist			*hist;
	struct termios	termios_saved;
	char			*cwd;
	char			*prompt;
	int				exit;
	int				eof;
	int				last_status;
	char			*line;
	char			*line2;
	int				stdin_save;
	int				stdout_save;
	int				fd;
	size_t			len;
	ssize_t			nread;
	char			***cmds;
}	t_shell;

//builtins/cd.c
void	change_path(char **tokens, t_shell *shell);

//builtins/echo.c
void	ft_echo(char **tokens, t_shell *shell);

//builtins/environment.c
int		ft_env_init(t_shell *shell, char **env_array);
void	ft_print_env(t_shell *shell);
char	*ft_getenv(t_shell *shell, const char *key);
void	ft_free_env(t_shell *shell);
int		append_env(t_shell *shell, const char *entry);

//builtins/export.c
int		is_valid_key(const char *key);
void	ft_env_set(t_shell *shell, const char *entry);
void	ft_export(char **tokens, t_shell *shell);

//builtins/history.c
void	ft_add_history(t_shell *shell);
void	ft_print_history(t_shell *shell);
void	ft_free_history(t_shell *shell);

//builtins/unset.c
void	ft_unset(char **tokens, t_shell *shell);

//srcs/execute_builtin.c
int		is_builtin(char **tokens);
int		ft_execute_builtin(char **tokens, t_shell *shell);

//srcs/execute.c
void	ft_execute_pipes(t_shell *shell);

//srcs/readline.c
void	ft_readline(t_shell *shell);
void	ft_free_line(t_shell *shell);

//srcs/redirect.c
int		handle_redirections(char **cmd, t_shell *shell);
void	restore_std(t_shell *shell);

//srcs/shell_exit.c
void	ft_free_array(char ***array);
void	ft_free_matrix(char ****matrix);
int		ft_exit_shell(t_shell *shell);
void	handle_error(t_errorlst error, t_shell *shell);

//srcs/shell.c
t_shell	*ft_get_shell_address(t_shell *shell);
void	ft_disable_echoctl(t_shell *shell);
void	ft_restore_term_settings(t_shell *shell);
void	ft_init_shell(t_shell *shell, char **env);

//srcs/signals.c
void	ft_sigint_handler(int sig);
void	ft_sigquit_handler(int sig);
void	ft_setup_signals_prompt(void);
void	ft_setup_signals_child(void);

//srcs/token.c
void	split_line(t_shell *shell);

//src/utils_pipex.c
char *command_finder(char **command, char **paths);
char **paths_finder(char **env);

//src/utils_print.c
void	print_tokens_array(t_shell *shell);

//srcs/utils_quotes.c
int		closed_quotes(char *line, int i);

//srcs/util_redirects.c
int		ft_redirect(t_shell *shell, char **cmd, int i);
int		ft_adding(t_shell *shell, char **cmd, int i);
int		ft_left(t_shell *shell, char **cmd, int i);
int		ft_leftleft(t_shell *shell, char **cmd, int i);

//srcs/utils_token.c
int		count_pipes(t_shell *shell);
char	*copy_token(char *line, t_span sp);
void	add_token(char **cmds, int *token_idx, char *line, t_span sp);
int		ft_next_span(char *s, size_t *i, t_span *sp);
size_t	alloc_tokens(char **cmds, char *line);

//srcs/utils.c
void	ft_build_prompt(t_shell *shell);
int		ft_error(const char *msg);
int		ft_isspace(char c);
void	filter_args(char **args, char ***tokens, t_shell *shell);

#endif
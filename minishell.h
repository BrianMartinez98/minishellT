#ifndef MINISHELL
# define MINISHELL

# define SPACE	"\t\n\v\f\r "

# define STDIN 0
# define STDOUT 1
# define STDERR 2

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
	char			*cwd;
	char			*prompt;
	int				exit;
	int				last_status; //para almacenar el último estado de salida de un comando $?
}	t_shell;

//builtins/cd.c
void    change_path(t_shell *shell, char **tokens);

//builtins/echo.c
void	ft_echo(t_shell *shell, char **tokens);


void	ft_init_shell(t_shell *shell, char **env);
int		ft_exit_shell(t_shell *shell);

//builtins/environment.c
int		ft_env_init(t_shell *shell, char **env_array);
void	ft_print_env(t_shell *shell);
char	*ft_getenv(t_shell *shell, const char *key);
void	ft_free_env(t_shell *shell);

//builtins/export.c
int		is_valid_key(const char *key);
void	ft_env_set(t_shell *shell, const char *entry);
void	ft_export(t_shell *shell, char **tokens);

//builtins/history.c
void	ft_add_history(char *line, t_shell *shell);
void	ft_print_history(t_shell *shell);
void	ft_free_history(t_shell *shell);

//builtins/unset.c
void	ft_unset(t_shell *shell, char **tokens);


//srcs/execute.c
int		ft_execute(char **array, t_shell *shell);

//srcs/redirect.c
// static int	duplication(char *filename);
// void		redirect(char **array);

//signals.c
void	ft_sigint_handler(int sig);
void	ft_sigquit_handler(int sig);
void	ft_setup_signals_prompt(void);
void	ft_setup_signals_child(void);

//token.c
char	**split_line(char *line);
int		print_tokens(char **tokens);

//utils.c
void	ft_init_shell(t_shell *shell, char **env);
int		ft_exit_shell(t_shell *shell);
void	ft_build_prompt(t_shell *shell);
int		ft_error(const char *msg);
int		ft_isspace(char c);

//redirect.c
int 	dupp(char *filename, int dest, int flags);
char 	**filter_args(char **array);

//redirection_utils.c
char 	**filter_args(char **args);
int 	handle_redirections(char **args);

//pipes.c
int		ft_execute_pipes(char **array, t_shell *shell);

#endif
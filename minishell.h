#ifndef MINISHELL
# define MINISHELL

# define SPACE	"\t\n\v\f\r "

# define STDIN 0
# define STDOUT 1
# define STDERR 2

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
	t_token			*start;
	t_env			*secret_env;

	t_env			*env;//ok
	t_hist			*hist;//ok
	char			*cwd;//ok
	char			*prompt;//ok
	int				exit;//ok
	int				last_status; //ok para almacenar el último estado de salida de un comando
	
	int				in;
	int				out;
	int				fdin;
	int				fdout;
	int				pipin;
	int				pipout;
	int				pid;
	int				charge;
	int				parent;
	int				last;
	int				ret;
	int				no_exec;
}	t_shell;

//builtin.c
void	ft_init_shell(t_shell *shell, char **env);
int		ft_exit_shell(t_shell *shell);
void	ft_echo(t_shell *shell, char **tokens);

//environment.c
int		ft_env_init(t_shell *shell, char **env_array);
void	ft_print_env(t_shell *shell);
char	*ft_getenv(t_shell *shell, const char *key);
void	ft_free_env(t_shell *shell);

//ft_split.c
char	**ft_split(char const *s, char c);

//ft_strjoin.c
char	*ft_strjoin(char const *s1, char const *s2);

//history.c
void	ft_add_history(char *line, t_shell *shell);
void	ft_print_history(t_shell *shell);
void	ft_free_history(t_shell *shell);

//libft.c
size_t	ft_strlen(const char	*str);
size_t	ft_strncpy(char *dest, const char *src, size_t size);
int		ft_isspace(char c);
int		ft_strcmp(const char *s1, const char *s2);
int		ft_strncmp(const char *s1, const char *s2, unsigned int n);
char	*ft_strdup(const char *s1);

//main.c
int		ft_execute(char **array, t_shell *shell);

//signals.c
void	ft_sigint_handler(int sig);
void	ft_sigquit_handler(int sig);

void	ft_setup_signals_prompt(void);
void	ft_setup_signals_child(void);

//token.c
char	**split_line(char *line);
int		print_tokens(char **tokens);

//utils.c
void	ft_build_prompt(t_shell *shell);
int		ft_error(const char *msg);

#endif
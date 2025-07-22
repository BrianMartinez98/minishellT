#ifndef MINISHELL
# define MINISHELL

# define SPACE	"\t\n\v\f\r "

# include <stdio.h>
# include <ctype.h>
# include <string.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/wait.h>
# include <readline/readline.h>
# include <readline/history.h>

typedef struct s_hist
{
    char *line;
    struct s_hist *next;
    struct s_hist *prev;
} t_hist;

char	**ft_split(char const *s, char c);
char	*ft_strjoin(char const *s1, char const *s2);
size_t	ft_strlen(const char	*str);
char    **split_line(char *line);
size_t	ft_strncpy(char *dest, const char *src, size_t size);
int     ft_isspace(char c);
int     ft_exit();
int     ft_execute(char **array);
int	    ft_strcmp(const char *s1, const char *s2);
char	*ft_getcwd(char *buf, size_t size);
void    ft_add_history(t_hist **lst, char *new);
void    print_history(t_hist **lst);

#endif
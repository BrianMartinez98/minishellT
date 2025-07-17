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

char	**ft_split(char const *s, char c);
char	*ft_strjoin(char const *s1, char const *s2);
size_t	ft_strlen(const char	*str);
char    **split_line(char *line);
size_t	ft_strncpy(char *dest, const char *src, size_t size);
int     ft_isspace(char c);

#endif
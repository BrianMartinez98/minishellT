/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_redirect.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jarregui <jarregui@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/11 13:28:43 by jarregui          #+#    #+#             */
/*   Updated: 2025/11/07 17:37:18 by jarregui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	ft_redirect(t_shell *shell, char **cmd, int i)
{
	shell->fd = open(cmd[i], O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (shell->fd < 0)
	{
		perror(cmd[i]);
		return (-1);
	}
	dup2(shell->fd, STDOUT_FILENO);
	close(shell->fd);
	return (0);
}

int	ft_adding(t_shell *shell, char **cmd, int i)
{
	shell->fd = open(cmd[i], O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (shell->fd < 0)
	{
		perror(cmd[i]);
		return (-1);
	}
	dup2(shell->fd, STDOUT_FILENO);
	close(shell->fd);
	return (0);
}

int	ft_left(t_shell *shell, char **cmd, int i)
{
	shell->fd = open(cmd[i], O_RDONLY);
	if (shell->fd < 0)
	{
		perror(cmd[i]);
		return (-1);
	}
	dup2(shell->fd, STDIN_FILENO);
	close(shell->fd);
	return (0);
}

#include "../minishell.h"

/* ──────────────────────────────────────────────── */
/* Helper: muestra warning si el heredoc termina con EOF sin delimitador */
static void	print_heredoc_warning(const char *delimiter)
{
	char	buffer[256];

	ft_strlcpy(buffer, "minishell: warning: here-document delimited by end-of-file (wanted `", sizeof(buffer));
	ft_strlcat(buffer, delimiter, sizeof(buffer));
	ft_strlcat(buffer, "')", sizeof(buffer));
	ft_putendl_fd(buffer, STDERR_FILENO);
}

/* ──────────────────────────────────────────────── */
/* Helper: detecta si el delimitador tiene comillas y las elimina */
static int	remove_quotes_and_check_expand(char **delimiter)
{
	int	expand = 1;
	char	*new_delim;

	if (!delimiter || !*delimiter)
		return (expand);
	new_delim = *delimiter;
	if ((new_delim[0] == '\'' && new_delim[ft_strlen(new_delim) - 1] == '\'') ||
		(new_delim[0] == '"' && new_delim[ft_strlen(new_delim) - 1] == '"'))
	{
		expand = 0;
		new_delim = ft_substr(new_delim, 1, ft_strlen(new_delim) - 2);
		free(*delimiter);
		*delimiter = new_delim;
	}
	return (expand);
}

#include "../minishell.h"

/* ────────────────────────────── */
/* Helpers pequeños para concatenar */
char	*ft_strjoin_free(char *s1, char *s2, int free_flag)
{
	char	*joined;

	if (!s1 && !s2)
		return (NULL);
	if (!s1)
		return (ft_strdup(s2));
	if (!s2)
		return (ft_strdup(s1));
	joined = ft_strjoin(s1, s2);
	if (free_flag == 1 || free_flag == 3)
		free(s1);
	if (free_flag == 2 || free_flag == 3)
		free(s2);
	return (joined);
}

char	*ft_strjoin_char(char *s, char c)
{
	char	buf[2];
	char	*tmp;

	buf[0] = c;
	buf[1] = '\0';
	tmp = ft_strjoin(s, buf);
	free(s);
	return (tmp);
}

/* ────────────────────────────── */
/* Busca una variable en el entorno */
static char	*get_env_value(char **env, const char *var_name)
{
	int		i;
	size_t	len;

	if (!env || !var_name)
		return (NULL);
	len = ft_strlen(var_name);
	i = 0;
	while (env[i])
	{
		if (!ft_strncmp(env[i], var_name, len) && env[i][len] == '=')
			return (ft_strdup(env[i] + len + 1));
		i++;
	}
	return (NULL);
}

/* ────────────────────────────── */
/* Expande variables $VAR y $? en el heredoc */
char	*expand_heredoc_line(const char *line, t_shell *shell)
{
	char	*result;
	size_t	i = 0;
	size_t	start;
	char	*var_name;
	char	*var_value;

	if (!line)
		return (NULL);
	result = ft_strdup("");

	while (line[i])
	{
		if (line[i] == '$')
		{
			i++;
			if (line[i] == '?')
			{
				var_value = ft_itoa(shell->last_status);
				result = ft_strjoin_free(result, var_value, 3);
				i++;
				continue;
			}
			else if (ft_isalpha(line[i]) || line[i] == '_')
			{
				start = i;
				while (ft_isalnum(line[i]) || line[i] == '_')
					i++;
				var_name = ft_substr(line, start, i - start);
				var_value = get_env_value(shell->env, var_name);
				if (var_value)
					result = ft_strjoin_free(result, var_value, 1);
				free(var_name);
				continue;
			}
			else
			{
				result = ft_strjoin_char(result, '$');
				continue;
			}
		}
		result = ft_strjoin_char(result, line[i]);
		i++;
	}
	return (result);
}


/* ──────────────────────────────────────────────── */
/* Expande variables dentro del heredoc si está permitido */
static char	*expand_line(char *line, t_shell *shell)
{
	// Aquí puedes usar tu propia función de expansión si ya la tienes
	// Ejemplo placeholder:
	return (expand_heredoc_line(line,  shell));  // <-- adapta a tu función real
}

/* ──────────────────────────────────────────────── */
/* Proceso hijo del heredoc: lee líneas y escribe al pipe */
static void	heredoc_child(t_shell *shell, int *pipefd, char *delimiter, int expand)
{
	char	*line;
	char	*expanded;

	signal(SIGINT, SIG_DFL);   // Ctrl+C mata solo al hijo
	signal(SIGQUIT, SIG_IGN);  // ignorar Ctrl+D
	close(pipefd[0]); // el hijo no lee del pipe
	while (1)
	{
		write(STDOUT_FILENO, "> ", 2);
		line = NULL;
		shell->nread = getline(&line, &shell->len, stdin);
		if (shell->nread == -1) // Ctrl+D o EOF
		{
			print_heredoc_warning(delimiter);
			free(line);
			break;
		}
		line[shell->nread - 1] = '\0';
		if (ft_strcmp(line, delimiter) == 0)
		{
			free(line);
			break;
		}
		if (expand)
			expanded = expand_line(line, shell);
		else
			expanded = ft_strdup(line);
		write(pipefd[1], expanded, ft_strlen(expanded));
		write(pipefd[1], "\n", 1);
		free(line);
		free(expanded);
	}
	close(pipefd[1]);
	exit(0);
}

/* ──────────────────────────────────────────────── */
/* Función principal: crea pipe, fork y gestiona señales */
int	ft_leftleft(t_shell *shell, char **cmd, int i)
{
	int		pipefd[2];
	pid_t	pid;
	int		status;
	char	*delimiter;
	int		expand;

	delimiter = ft_strdup(cmd[i + 1]);
	if (!delimiter)
		return (-1);
	expand = remove_quotes_and_check_expand(&delimiter);

	if (pipe(pipefd) == -1)
	{
		perror("pipe");
		free(delimiter);
		return (-1);
	}

	pid = fork();
	if (pid < 0)
	{
		perror("fork");
		close(pipefd[0]);
		close(pipefd[1]);
		free(delimiter);
		return (-1);
	}

	if (pid == 0)
		heredoc_child(shell, pipefd, delimiter, expand);

	free(delimiter);
	close(pipefd[1]); // cerramos escritura en el padre

	waitpid(pid, &status, 0);

	if (WIFSIGNALED(status) && WTERMSIG(status) == SIGINT)
	{
		// heredoc cancelado con Ctrl+C
		close(pipefd[0]);
		shell->last_status = 130;
		return (-1);
	}

	// 🔥 NO hacemos dup2 aquí
	// devolvemos el fd al padre para usarlo más tarde
	return (pipefd[0]);
}


int	is_redir_token(const char *t)
{
	if (!t)
		return (0);
	return (strcmp(t, "<") == 0
		|| strcmp(t, ">") == 0
		|| strcmp(t, "<<") == 0
		|| strcmp(t, ">>") == 0
		|| strcmp(t, "2>") == 0
		|| strcmp(t, "|") == 0);
}

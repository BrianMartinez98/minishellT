#include "../minishell.h"

void	split_line(t_shell *shell)
{
	size_t		start;
	size_t		length;
	size_t		token_length;
	size_t		end;
	size_t		bufsize = BUFSIZ;
	size_t		position = 0;
	size_t		i;
	char 		*token;
	char		**new_tokens;

	if (shell->tokens)
		ft_free_tokens(shell->tokens); // Limpiar tokens anteriores
	shell->tokens = NULL; // Inicializar tokens a NULL
	start = 0;
	length = ft_strlen(shell->line);
	shell->tokens = malloc(bufsize * sizeof(char *));
	if (!shell->tokens)
	{
		perror("malloc shell->tokens");
		return ;
	}
	while (start < length)
	{
		// Saltar espacios en blanco
		while (start < length && ft_isspace((unsigned char)shell->line[start]))
			start++;

		/*
		if (line[start] == '"' || line[start] == '\'')
			while (line[start] == '"' || line[start] == '\'')					No funciona bien esta parte, deberia respetar las comillas
				start++;
		*/

		if (start == length)
			break ;
		end = start;
		while (end < length && !ft_isspace((unsigned char)shell->line[end]))
			end++;
		token_length = end - start;
		token = malloc(token_length + 1);
		if (!token)															   // Reconstruir esto, crear la parte de limpieza
		{
			perror("malloc token");
			ft_free_tokens(shell->tokens);
			return ;
		}
		ft_strncpy(token, &shell->line[start], token_length);
		token[token_length] = '\0';
		shell->tokens[position++] = token;
		if (position >= bufsize)
		{
			bufsize *= 2;
			new_tokens = malloc(bufsize * sizeof(char *));
			if (!new_tokens)
			{
				perror("malloc new_tokens");
				ft_free_tokens(shell->tokens);
				return ;
			}
			i = 0;
			while (i < position)
			{
				new_tokens[i] = shell->tokens[i];
				i++;
			}
			free(shell->tokens);
			shell->tokens = new_tokens;
		}
		start = end;
	}
	shell->tokens[position] = NULL; // Terminar con NULL
	return ;
}

int	print_tokens(char **tokens)
{
	size_t	i;

	if (!tokens)
		return (1);
	i = 0;
	if (DEBUG)
	{
		printf("DEBUG: imprimiendo tokens que nos llegan: \n");
		while (tokens[i])
		{
			printf("Token[%zu]: '%s'\n", i, tokens[i]);
			i++;
		}
	}
	return (0);
}

void	ft_free_tokens(char **tokens)
{
	size_t	i;

	if (!tokens)
		return ;
	i = 0;
	while (tokens[i])
	{
		free(tokens[i]);
		i++;
	}
	free(tokens);
	tokens = NULL;
}

// Para probar:
/*
int main(void)
{
	char line[] = "echo \"hola mundo\"";
	char **tokens = cell_split_line(line);
	if (!tokens) return 1;

	for (size_t i = 0; tokens[i] != NULL; i++)
	{
		printf("Token[%zu]: '%s'\n", i, tokens[i]);
		free(tokens[i]);
	}
	free(tokens);
	return 0;
}
*/

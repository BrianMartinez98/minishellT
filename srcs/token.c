#include "../minishell.h"

char **split_line(char *line)
{
	size_t		start;
	size_t		length;
	size_t		token_length;
	char 		*token;
	size_t		end;
	size_t		bufsize = BUFSIZ;
	size_t		position = 0;
	char		**tokens;

	start = 0;
	length = ft_strlen(line);
	tokens = malloc(bufsize * sizeof(char *));
	if (!tokens)
	{
		perror("malloc");
		return NULL;
	}
	while (start < length)
	{
		// Saltar espacios en blanco
		while (start < length && ft_isspace((unsigned char)line[start]))
			start++;

		/*
		if (line[start] == '"' || line[start] == '\'')
			while (line[start] == '"' || line[start] == '\'')					No funciona bien esta parte, deberia respetar las comillas
				start++;
		*/

		if (start == length)
			break;
		end = start;
		while (end < length && !ft_isspace((unsigned char)line[end]))
			end++;
		token_length = end - start;
		token = malloc(token_length + 1);
		if (!token)															   // Reconstruir esto, crear la parte de limpieza
		{
			perror("malloc");
			for (size_t i = 0; i < position; i++)
				free(tokens[i]);
			free(tokens);
			return NULL;
		}
		strncpy(token, &line[start], token_length);						   // mi ft_strcpy tiene algun fallo porque no funciona bien y por eso uso este
		token[token_length] = '\0';
		tokens[position++] = token;
		if (position >= bufsize)
		{
			bufsize *= 2;
			char **new_tokens = realloc(tokens, bufsize * sizeof(char *));	 // cambiar esto, no puedo usar realloc ni for!!!!!
			if (!new_tokens)
			{
				perror("realloc");
				// Liberar tokens previos
				for (size_t i = 0; i < position; i++)
					free(tokens[i]);
				free(tokens);
				return NULL;
			}
			tokens = new_tokens;
		}
		start = end;
	}
	tokens[position] = NULL; // Terminar con NULL
	return (tokens);
}

int	print_tokens(char **tokens)
{
	if (!tokens)
		return 1;
	for (size_t i = 0; tokens[i] != NULL; i++)
	{
		printf("Token[%zu]: '%s'\n", i, tokens[i]);
		// free(tokens[i]);
	}
	return 0;
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

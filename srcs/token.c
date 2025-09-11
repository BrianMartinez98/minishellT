/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jarregui <jarregui@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/10 22:52:05 by jarregui          #+#    #+#             */
/*   Updated: 2025/09/11 14:39:59 by jarregui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	init_tokens(t_shell *shell, size_t bufsize)
{
	if (shell->tokens)
		ft_free_array(shell->tokens);
	shell->tokens = malloc(bufsize * sizeof(char *));
	if (!shell->tokens)
	{
		perror("malloc shell->tokens");
		return (0);
	}
	return (1);
}

static char	*copy_token(char *line, size_t start, size_t end)
{
	size_t	len;
	char	*token;

	len = end - start;
	token = malloc(len + 1);
	if (!token)
		return (NULL);
	ft_strncpy(token, &line[start], len);
	token[len] = '\0';
	return (token);
}

static char	**resize_tokens(char **tokens, size_t old_size, size_t new_size)
{
	char	**new_tokens;
	size_t	i;

	new_tokens = malloc(new_size * sizeof(char *));
	if (!new_tokens)
		return (NULL);
	i = 0;
	while (i < old_size)
	{
		new_tokens[i] = tokens[i];
		i++;
	}
	free(tokens);
	return (new_tokens);
}

void	split_line(t_shell *shell)
{
	size_t	start;
	size_t	end;
	size_t	position;
	size_t	bufsize;
	char	*token;

	bufsize = BUFSIZ;
	position = 0;
	if (!init_tokens(shell, bufsize))
		return ;
	start = 0;
	while (shell->line[start])
	{
		while (ft_isspace((unsigned char)shell->line[start]))
			start++;
		if (!shell->line[start])
			break ;
		end = start;
		while (shell->line[end] && !ft_isspace((unsigned char)shell->line[end]))
			end++;
		token = copy_token(shell->line, start, end);
		if (!token)
			return (ft_free_array(shell->tokens));
		shell->tokens[position++] = token;
		if (position >= bufsize)
		{
			bufsize *= 2;
			shell->tokens = resize_tokens(shell->tokens, position, bufsize);
			if (!shell->tokens)
				return (perror("malloc new_tokens"));
		}
		start = end;
	}
	shell->tokens[position] = NULL;
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

// void	split_line(t_shell *shell)
// {
// 	size_t		start;
// 	size_t		length;
// 	size_t		token_length;
// 	size_t		end;
// 	size_t		bufsize = BUFSIZ;
// 	size_t		position = 0;
// 	size_t		i;
// 	char 		*token;
// 	char		**new_tokens;

// 	if (shell->tokens)
// 		ft_free_array(shell->tokens); // Limpiar tokens anteriores
// 	shell->tokens = NULL; // Inicializar tokens a NULL
// 	start = 0;
// 	length = ft_strlen(shell->line);
// 	shell->tokens = malloc(bufsize * sizeof(char *));
// 	if (!shell->tokens)
// 	{
// 		perror("malloc shell->tokens");
// 		return ;
// 	}
// 	while (start < length)
// 	{
// 		// Saltar espacios en blanco
// 		while (start < length && ft_isspace((unsigned char)shell->line[start]))
// 			start++;

// 		/*
// 		if (line[start] == '"' || line[start] == '\'')
// 			while (line[start] == '"' || line[start] == '\'')					No funciona bien esta parte, deberia respetar las comillas
// 				start++;
// 		*/

// 		if (start == length)
// 			break ;
// 		end = start;
// 		while (end < length && !ft_isspace((unsigned char)shell->line[end]))
// 			end++;
// 		token_length = end - start;
// 		token = malloc(token_length + 1);
// 		if (!token)															   // Reconstruir esto, crear la parte de limpieza
// 		{
// 			perror("malloc token");
// 			ft_free_array(shell->tokens);
// 			return ;
// 		}
// 		ft_strncpy(token, &shell->line[start], token_length);
// 		token[token_length] = '\0';
// 		shell->tokens[position++] = token;
// 		if (position >= bufsize)
// 		{
// 			bufsize *= 2;
// 			new_tokens = malloc(bufsize * sizeof(char *));
// 			if (!new_tokens)
// 			{
// 				perror("malloc new_tokens");
// 				ft_free_array(shell->tokens);
// 				return ;
// 			}
// 			i = 0;
// 			while (i < position)
// 			{
// 				new_tokens[i] = shell->tokens[i];
// 				i++;
// 			}
// 			free(shell->tokens);
// 			shell->tokens = new_tokens;
// 		}
// 		start = end;
// 	}
// 	shell->tokens[position] = NULL; // Terminar con NULL
// 	return ;
// }
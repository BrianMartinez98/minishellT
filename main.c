#include "minishell.h"

static char	*ft_getcwd(char *buf, size_t size)
{
	char	*result;

	result = getcwd(buf, size);
	if (!result)
		perror("Getcwd failed");
	return (result);
}

int print_tokens(char **tokens)
{
    if (!tokens) return 1;
    for (size_t i = 0; tokens[i] != NULL; i++)
    {
        printf("Token[%zu]: '%s'\n", i, tokens[i]);
        free(tokens[i]);
    }
    return 0;
}

int main()
{
    int     status;
    char    *line;
    char    **array;
    pid_t   pid;
    char    cwd[1024];
    char    *prompt;

    while (1)
    {
        ft_getcwd(cwd, sizeof(cwd));
        prompt = ft_strjoin(cwd, " > ");
        line = readline(prompt);
        if (!line)
        {
            printf("exit\n");
            break;
        }
        array = split_line(line);
        if (!array || !array[0])
        {
            free(line);
            continue;
        }
        print_tokens(array);          // Comprobar que los tokens se parsean correctamente

        /*                           Usar esto para ejecutar comandos como ls -la
        pid = fork();
        if (pid < 0)
        {
            perror("fork failed");
            free(line);
            continue;
        }
        else if (pid == 0)
        {
            execvp(array[0], array);
            perror("execvp failed");
            exit(1);
        }
        wait(&status);
        */
        free(line);
        free(array);
    }
    return 0;
}

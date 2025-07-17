#include "minishell.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <readline/readline.h>

static char	*ft_getcwd(char *buf, size_t size)
{
	char	*result;

	result = getcwd(buf, size);
	if (!result)
		perror("Getcwd failed");
	return (result);
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
        array = ft_split(line, ' ');
        if (!array || !array[0])
        {
            free(line);
            continue;
        }
        pid = fork();
        if (pid < 0)
        {
            perror("fork failed");
            free(line);
            // liberar array aquí si es necesario
            continue;
        }
        else if (pid == 0)
        {
            execvp(array[0], array);
            perror("execvp failed");
            exit(1);
        }
        wait(&status);
        free(line);
        // liberar array si ft_split usa malloc para cada string
        for (int i = 0; array[i]; i++)  //Convertir esto en una funcion de limpieza
            free(array[i]);
        free(array);
    }

    return 0;
}

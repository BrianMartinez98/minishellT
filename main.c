#include "minishell.h"

int	ft_execute(char **array)
{
	int	result;

	result = 0;
	if (ft_strcmp(array[0], "exit") == 0)
		result = ft_exit();
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

int main(int ac, char **av, char **env)
{
	t_shell	shell;

	(void)ac; //ignoramos estos parámetros que no usamos
	(void)av; //ignoramos estos parámetros que no usamos

	env_init(&shell, env); //copiamos las variables de entorno.
	shell.in = dup(STDIN);
	shell.out = dup(STDOUT);
	shell.exit = 0;
	shell.ret = 0;
	shell.no_exec = 0;
	
	int		status;
	char	*line;
	char	**array;
	pid_t	pid;
	char	cwd[1024]; // esta permitido???
	char	*prompt;
	t_hist	**lst;

	lst = (t_hist **)malloc(sizeof(t_hist));
	while (1)
	{
		prompt = "\001\033[0;36m\033[1m\002";
		ft_getcwd(cwd, sizeof(cwd));
		prompt = ft_strjoin(prompt, cwd);
		prompt = ft_strjoin(prompt, " > \001\033[0m\002");
		line = readline(prompt); //aquí imprimimos el prompt y se queda a la espera para leer la línea que introduzcamos. Es una función estandar de C con readline.h
		
		ft_add_history(lst, line);
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
		ft_execute(array);
		print_history(lst);
		//print_tokens(array);		  // Comprobar que los tokens se parsean correctamente

		/*						   Usar esto para ejecutar comandos como ls -la
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

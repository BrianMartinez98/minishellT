/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jarregui <jarregui@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/02 19:47:59 by jarregui          #+#    #+#             */
/*   Updated: 2025/11/05  by ChatGPT_fix            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include <sys/wait.h>
#include <errno.h>

/* ---------- debug helpers (mantengo) ---------- */
void	print_tokens(char **tokens)
{
	int	i;

	if (!tokens)
	{
		printf("[DEBUG] tokens = (null)\n");
		return ;
	}
	printf("\033[0;36m[DEBUG] Tokens:\033[0m\n");
	for (i = 0; tokens[i]; i++)
		printf("  [%d] '%s'\n", i, tokens[i]);
}

void	print_shellenv(char **env)
{
	int	i;

	if (!env)
	{
		printf("[DEBUG] env = (null)\n");
		return ;
	}
	printf("\033[0;36m[DEBUG] Environment variables:\033[0m\n");
	for (i = 0; env[i]; i++)
		printf("  %s\n", env[i]);
}

void	print_pathname(const char *pathname)
{
	printf("\033[0;36m[DEBUG] Pathname:\033[0m ");
	if (pathname)
		printf("'%s'\n", pathname);
	else
		printf("(null)\n");
}

/*
 * pid_child: código que corre en el hijo tras fork.
 * - in_fd: fd que debe convertirse en STDIN (o -1 para no tocar)
 * - out_fd: fd que debe convertirse en STDOUT (o -1 para no tocar)
 *
 * Nota: aquí se deben aplicar redirecciones, ejecutar builtin (si aplica en child)
 * o llamar a execve. Nunca regresar al padre: terminar con _exit(status).
 */
static void	pid_child(char **tokens, char **cmd, t_shell *shell, int in_fd, int out_fd)
{
	char	**paths;
	char	*pathname;

	/* configurar señales en hijo */
	ft_setup_signals_child();

	/* redirigir stdin/stdout si necesitamos */
	if (in_fd != -1 && in_fd != STDIN_FILENO)
	{
		if (dup2(in_fd, STDIN_FILENO) == -1)
		{
			perror("dup2 in_fd");
			_exit(1);
		}
	}
	if (out_fd != -1 && out_fd != STDOUT_FILENO)
	{
		if (dup2(out_fd, STDOUT_FILENO) == -1)
		{
			perror("dup2 out_fd");
			_exit(1);
		}
	}

	/* cerrar fds originales que ya no se usan */
	if (in_fd != -1)
		close(in_fd);
	if (out_fd != -1)
		close(out_fd);

	/* aplicar redirecciones específicas del comando (files, heredoc) */
	handle_redirections(cmd, shell);

	/* Si el token es builtin en el contexto de child (p. ej. parte de pipeline),
	   ejecutarlo aquí y terminar con su status */
	if (is_builtin(tokens))
	{
		ft_execute_builtin(tokens, shell);
		_exit(shell->last_status & 0xFF);
	}

	/* buscar ejecutable solo si no es builtin */
	paths = paths_finder(shell->env);
	pathname = command_finder(tokens, paths);

	if (DEBUG)
	{
		printf("\033[0;35m\nDEBUG execve:\npathname = %s\n", pathname);
		printf("\033[0m\n");
	}

	/* si no encontramos comando, informar y salir con 127 */
	if (!pathname)
	{
		ft_putstr_fd("minishell: ", STDERR_FILENO);
		ft_putstr_fd(tokens[0], STDERR_FILENO);
		ft_putstr_fd(": command not found\n", STDERR_FILENO);
		_exit(127);
	}

	/* ejecutar */
	execve(pathname, tokens, shell->env);
	/* si execve retorna: error */
	perror(tokens[0]);
	_exit(127);
}

/*
 * fork_and_exec: hace fork y en hijo llama a pid_child con los fds
 * Devuelve pid del child (>0), -1 en error.
 */
static pid_t	fork_and_exec(char **tokens, char **cmd, t_shell *shell, int in_fd, int out_fd)
{
	pid_t	pid;

	pid = fork();
	if (pid < 0)
	{
		perror("fork");
		shell->last_status = 1;
		return (-1);
	}
	if (pid == 0)
		pid_child(tokens, cmd, shell, in_fd, out_fd);
	/* padre */
	return (pid);
}

/*
 * execute_command: ejecuta el comando:
 * - si es builtin y no tiene next (no está en pipeline) -> se ejecuta en padre
 * - en cualquier otro caso -> fork_and_exec (child hará builtin o exec)
 *
 * in_fd/out_fd se pasan sólo al child (cuando se haga fork).
 * Devuelve pid del child si creó uno (>0), -2 si no creó pid (builtin en padre),
 * -1 en error.
 */
static pid_t	execute_command(t_shell *shell, char **cmd, char **tokens, int has_next, int in_fd, int out_fd)
{
	int		saved_stdin;
	int		saved_stdout;
	pid_t	pid;

	if (!tokens || !tokens[0])
		return (-2);
	/* Si es builtin y NO forma parte de un pipeline -> ejecútalo en el padre */
	if (is_builtin(tokens) && !has_next)
	{
		/* marcar que ejecutamos builtin en padre (mantener compatibilidad) */
		shell->builtin = 1;

		/* guardo/dup de stdio para poder aplicar redirecciones temporalmente */
		saved_stdin = dup(STDIN_FILENO);
		saved_stdout = dup(STDOUT_FILENO);
		if (saved_stdin == -1 || saved_stdout == -1)
		{
			perror("dup");
			if (saved_stdin != -1) close(saved_stdin);
			if (saved_stdout != -1) close(saved_stdout);
			return (-1);
		}
		handle_redirections(cmd, shell);
		ft_execute_builtin(tokens, shell);
		/* restaurar los fds originales en el padre */
		dup2(saved_stdin, STDIN_FILENO);
		dup2(saved_stdout, STDOUT_FILENO);
		close(saved_stdin);
		close(saved_stdout);
		/* no se creó pid */
		return (-2);
	}
	/* caso normal: crear child (en child se ejecutará builtin si corresponde) */
	pid = fork_and_exec(tokens, cmd, shell, in_fd, out_fd);
	return (pid);
}

/*
 * ft_execute_pipes: flujo robusto de pipeline:
 * - usa in_fd local para pasar read-end al siguiente comando
 * - crea pipe solo si hay siguiente comando
 * - controla cierres de fds en padre y en hijos
 * - restaura los saves del shell al final para prevenir colgados
 */
void	ft_execute_pipes(t_shell *shell)
{
	int		pipefd[2];
	char	**tokens;
	int		i;
	int		n;
	int		has_next;
	int		status = 0;
	pid_t	pids[256] = {0};
	int		in_fd = -1; /* fd que alimenta la entrada del siguiente comando */

	/* Nota: recordar los valores originales por si otras partes los usan */
	/* shell->stdin_save y stdout_save podrían usarse en otras partes del código,
	   por eso los restauramos al final. */
	n = 0;
	i = 0;
	while (shell->cmds[i])
	{
		has_next = (shell->cmds[i + 1] != NULL);

		/* si hay siguiente comando: crear pipe */
		if (has_next)
		{
			if (pipe(pipefd) == -1)
				handle_error(PIPES, shell);
		}

		/* preparar heredoc / comprobaciones; si falla, limpiar y salir */
		if (check_heredoc(shell->cmds[i], shell) == -1)
		{
			if (in_fd != -1)
				close(in_fd);
			/* cerrar cualquier pipe creado parcialmente */
			if (has_next)
			{
				close(pipefd[0]);
				close(pipefd[1]);
			}
			return ;
		}

		/* tokenizar/filtrar argumentos */
		filter_args(shell->cmds[i], &tokens, shell);

		/* determinar out_fd para el child: si hay next -> pipefd[1], sino STDOUT (-1 = no change) */
		int out_fd = (has_next ? pipefd[1] : -1);

		/* ejecutar comando: si se crea child, devuelve pid>0 */
		pid_t pid = execute_command(shell, shell->cmds[i], tokens, has_next, in_fd, out_fd);
		if (pid > 0)
			pids[n++] = pid;

		/* liberamos tokens (padre) */
		free(tokens);

		/* en el padre: cerrar extremos que no necesitemos y preparar in_fd para siguiente */
		if (has_next)
		{
			/* cerramos el extremo de escritura en el padre inmediatamente */
			close(pipefd[1]);
			/* cerramos el anterior in_fd (si distinto de stdin) */
			if (in_fd != -1)
				close(in_fd);
			/* la lectura del pipe será la entrada para el siguiente comando */
			in_fd = pipefd[0];
		}
		else
		{
			/* no hay siguiente: cerramos in_fd anterior si existe (ya no se necesita) */
			if (in_fd != -1)
			{
				close(in_fd);
				in_fd = -1;
			}
		}
		i++;
	}

	/* esperar a todos los hijos creados */
	for (int j = 0; j < n; j++)
	{
		if (waitpid(pids[j], &status, 0) == -1)
			perror("waitpid");
	}

	/* restaurar saves del shell para evitar que el shell principal quede con fds alterados.
	   Algunas partes del programa (o el main loop) pueden confiar en shell->stdin_save / stdout_save
	   apuntando a los fd originales, por lo que los devolvemos a STDIN/STDOUT. */
	if (shell->stdin_save != STDIN_FILENO)
	{
		dup2(STDIN_FILENO, shell->stdin_save); /* no fatal si falla */
		close(shell->stdin_save);
		shell->stdin_save = STDIN_FILENO;
	}
	if (shell->stdout_save != STDOUT_FILENO)
	{
		dup2(STDOUT_FILENO, shell->stdout_save);
		close(shell->stdout_save);
		shell->stdout_save = STDOUT_FILENO;
	}

	/* si no ejecutamos un builtin en el padre como último comando, actualizamos last_status
	   tomando el status del último child (si hay) */
	if (!shell->builtin)
	{
		if (WIFEXITED(status))
			shell->last_status = WEXITSTATUS(status);
		else if (WIFSIGNALED(status))
			shell->last_status = 128 + WTERMSIG(status);
	}
	/* reset del flag builtin (si se uso) */
	shell->builtin = 0;
}

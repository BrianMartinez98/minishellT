/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jarregui <jarregui@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/02 19:47:59 by jarregui          #+#    #+#             */
/*   Updated: 2025/08/04 20:10:16 by jarregui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	ft_execute(char **array, t_shell *shell)
{
    pid_t	pid;
    int		stdin_save, stdout_save;
    int		is_builtin = 0;
	char 	**clean_args;

    clean_args = filter_args(array);
    if (ft_strcmp(array[0], "exit") == 0 || ft_strcmp(array[0], "pwd") == 0 ||
        ft_strcmp(array[0], "echo") == 0 || ft_strcmp(array[0], "cd") == 0 ||
        ft_strcmp(array[0], "export") == 0 || ft_strcmp(array[0], "unset") == 0 ||
        ft_strcmp(array[0], "history") == 0 || ft_strcmp(array[0], "env") == 0)
        is_builtin = 1;

    if (is_builtin)
    {
        stdin_save = dup(STDIN_FILENO);
        stdout_save = dup(STDOUT_FILENO);

        handle_redirections(array);

        // Ejecuta builtin
        if (ft_strcmp(clean_args[0], "exit") == 0)
            shell->exit = 1;	
        else if (ft_strcmp(clean_args[0], "pwd") == 0)
            printf("%s\n", shell->cwd);
        else if (ft_strcmp(clean_args[0], "echo") == 0)
            ft_echo(shell, clean_args); 
        else if (ft_strcmp(clean_args[0], "cd") == 0)
            change_path(shell, clean_args);
        else if (ft_strcmp(clean_args[0], "export") == 0)
            ft_export(shell, clean_args);
        else if (ft_strcmp(clean_args[0], "unset") == 0)
            ft_unset(shell, clean_args);
        else if (ft_strcmp(clean_args[0], "history") == 0)
            ft_print_history(shell);
        else if (ft_strcmp(clean_args[0], "env") == 0)
            ft_print_env(shell);

        // Restaura stdin y stdout
        dup2(stdin_save, STDIN_FILENO);
        dup2(stdout_save, STDOUT_FILENO);
        close(stdin_save);
        close(stdout_save);
    }
    else
    {
        pid = fork();
        if (pid == -1)
        {
            perror("Error at fork");
            shell->last_status = 1;
            free(clean_args);
            return (shell->last_status);
        }
        if (pid == 0)
        {
            ft_setup_signals_child();
            handle_redirections(array);
            char **child_args = filter_args(array);
            execvp(child_args[0], child_args);
            if (errno == ENOENT)
                printf("command not found: %s\n", child_args[0]);
            else
                perror(child_args[0]);
            free(child_args);
            exit(127);
        }
        else
        {
            int	status;
            ft_setup_signals_prompt();
            waitpid(pid, &status, 0);
            if (WIFEXITED(status))
                shell->last_status = WEXITSTATUS(status);
            else if (WIFSIGNALED(status))
                shell->last_status = 128 + WTERMSIG(status);
        }
    }
    free(clean_args);
    return (shell->last_status);
}
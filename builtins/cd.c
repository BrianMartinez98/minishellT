#include "../minishell.h"
#include <unistd.h>     // chdir, getcwd
#include <stdlib.h>     // getenv, malloc, free
#include <stdio.h>      // printf, perror

void    change_path(t_shell *shell, char **tokens)
{
    const char  *path;

    if (tokens[1] == NULL)
    {
        path = getenv("HOME");
        if (!path)
            perror("cd: HOME not set");
    }
    else
        path = tokens[1];
    if (chdir(path) != 0)
        perror("cd");
    if (shell->cwd)
        free(shell->cwd);
    shell->cwd = getcwd(NULL, 0);
    if (!shell->cwd)
        perror("getcwd failed");
    printf("Changed directory to %s\n", shell->cwd);
}

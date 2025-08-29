#include "../minishell.h"

int dupp(char *filename, int dest, int flags)
{
    int fd = open(filename, flags, 0644);
    if (fd == -1)
        return -1;
    if (dup2(fd, dest) == -1)
    {
        close(fd);
        return -1;
    }
    close(fd);
    return 0;
}

/*
void redirect(char **array)
{
    int i = 0;

    while (array[i])
    {
        if (strcmp(array[i], ">") == 0)
            dupp(array[i + 1], STDOUT_FILENO, O_WRONLY | O_CREAT | O_TRUNC);
        else if (strcmp(array[i], "<") == 0)
            dupp(array[i + 1], STDIN_FILENO, O_RDONLY);
        // >> y << irían aquí también
        i++;
    }
}


char **filter_args(char **array)
{
    int i;
    int j;
    char **clean;

    int count = 0;
    for (i = 0; array[i]; i++)
    {
        if (strcmp(array[i], ">") == 0 || strcmp(array[i], "<") == 0
            || strcmp(array[i], ">>") == 0 || strcmp(array[i], "<<") == 0)
        {
            i++;
            continue;
        }
        count++;
    }
    clean = malloc(sizeof(char *) * (count + 1));
    if (!clean)
        return NULL;
    i = 0;
    j = 0;
    while (array[i])
    {
        if (strcmp(array[i], ">") == 0 || strcmp(array[i], "<") == 0
            || strcmp(array[i], ">>") == 0 || strcmp(array[i], "<<") == 0)
            i++; // saltamos el fichero
        else
            clean[j++] = strdup(array[i]);
        i++;
    }
    clean[j] = NULL;
    return clean;
}
*/
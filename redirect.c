static int duplication(char *filename) {
    int fd = open(filename, O_RDONLY);
    if (fd == -1) return -1; // Error
    if (dup2(fd, STDIN_FILENO) == -1) {
        close(fd);
        return -1;
    }
    close(fd);
    return 0; // Éxito
}

void    redirect(char **array)
{
    int i;

    i = 0;
    while(array[i])
    {
        if (array[i] == '<')
            duplication(array[i - 1]);
        else if (array[i] == '>')
            duplication(array[i + 1]);
        else if (array[i] == '<<')
            duplication(array[i - 1]);
        else if (array[i] == '>>')
            duplication(array[i + 1]);

    }
}
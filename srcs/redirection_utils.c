#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

// Devuelve un nuevo array sin los tokens de redirección y sus archivos
char **filter_args(char **args)
{
    int count = 0, i = 0, j = 0;
    while (args[count])
        count++;
    char **res = malloc(sizeof(char *) * (count + 1));
    if (!res) return NULL;
    while (args[i]) {
        if ((strcmp(args[i], "<") == 0 || strcmp(args[i], ">") == 0 ||
             strcmp(args[i], ">>") == 0 || strcmp(args[i], "<<") == 0) && args[i+1]) {
            i += 2; // Saltar operador y archivo
        } else {
            res[j++] = args[i++];
        }
    }
    res[j] = NULL;
    return res;
}

// Maneja redirecciones < > >> <<
// Devuelve 0 si OK, -1 si error
int handle_redirections(char **args)
{
    int i = 0, fd;
    while (args[i])
    {
        if (strcmp(args[i], "<") == 0 && args[i+1]) {
            fd = open(args[i+1], O_RDONLY);
            if (fd < 0) { perror(args[i+1]); return -1; }
            dup2(fd, STDIN_FILENO);
            close(fd);
            i += 2;
        } else if (strcmp(args[i], ">") == 0 && args[i+1]) {
            fd = open(args[i+1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
            if (fd < 0) { perror(args[i+1]); return -1; }
            dup2(fd, STDOUT_FILENO);
            close(fd);
            i += 2;
        } else if (strcmp(args[i], ">>") == 0 && args[i+1]) {
            fd = open(args[i+1], O_WRONLY | O_CREAT | O_APPEND, 0644);
            if (fd < 0) { perror(args[i+1]); return -1; }
            dup2(fd, STDOUT_FILENO);
            close(fd);
            i += 2;
        } else if (strcmp(args[i], "<<") == 0 && args[i+1]) {
            // Heredoc simple: crea un pipe y escribe hasta encontrar el delimitador
            int pipefd[2];
            if (pipe(pipefd) == -1) { perror("pipe"); return -1; }
            char *line = NULL;
            size_t len = 0;
            ssize_t nread;
            while (1) {
                write(1, "> ", 2);
                nread = getline(&line, &len, stdin);
                if (nread == -1) break;
                line[nread-1] = '\0'; // quitar \n
                if (strcmp(line, args[i+1]) == 0) break;
                write(pipefd[1], line, strlen(line));
                write(pipefd[1], "\n", 1);
            }
            free(line);
            close(pipefd[1]);
            dup2(pipefd[0], STDIN_FILENO);
            close(pipefd[0]);
            i += 2;
        } else {
            i++;
        }
    }
    return 0;
}
#include <stdio.h>
#include <string.h>





// Esto es basura
// Esto es basura
// Esto es basura
// Esto es basura
// Esto es basura
// Esto es basura




// Devuelve el índice del comando principal en los tokens
int find_command(char **tokens)
{
    int i = 0;
    int cmd_index = -1;

    while (tokens[i])
    {
        // Si encontramos un operador de redirección, saltamos el siguiente token
        if (strcmp(tokens[i], "<") == 0 ||
            strcmp(tokens[i], ">") == 0 ||
            strcmp(tokens[i], ">>") == 0 ||
            strcmp(tokens[i], "<<") == 0)
        {
            i++;
            continue;
        }

        // Si no es operador, y no está "consumido" por una redirección anterior,
        // este token es el comando principal.
        cmd_index = i;
        break;
    }

    return cmd_index;
}

int main(void)
{
    char *tokens1[] = {"cat", "file.txt", NULL};
    char *tokens2[] = {"cmd", "<", "echo", ">", "count", NULL};
    char *tokens3[] = {"<", "input", "grep", "word", ">", "out", NULL};

    int i;

    i = find_command(tokens1);
    printf("Caso 1: comando = %s\n", (i >= 0) ? tokens1[i] : "(ninguno)");

    i = find_command(tokens2);
    printf("Caso 2: comando = %s\n", (i >= 0) ? tokens2[i] : "(ninguno)");

    i = find_command(tokens3);
    printf("Caso 3: comando = %s\n", (i >= 0) ? tokens3[i] : "(ninguno)");

    return 0;
}

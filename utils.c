#include "minishell.h"

char	*ft_getcwd(char *buf, size_t size)
{
	char	*result;

	result = getcwd(buf, size);
	if (!result)
		perror("Getcwd failed");
	return (result);
}

void    ft_add_history(t_hist **lst, char *new)
{
    t_hist *node = malloc(sizeof(t_hist));
    if (!node)
        return ;
    node->line = strdup(new);
    node->next = NULL;

    if (*lst == NULL)
    {
        node->prev = NULL;
        *lst = node;
    }
    else
    {
        t_hist *last = *lst;
        while (last->next)
            last = last->next;
        last->next = node;
        node->prev = last;
    }
}

void print_history(t_hist **lst)
{
    t_hist *current;
    int i = 1;

    if (!lst || !*lst)
        return ;

    current = *lst;
    while (current)
    {
        printf("%d  %s\n", i, current->line);
        current = current->next;
        i++;
    }
}

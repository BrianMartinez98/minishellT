#include "minishell.h"

char	*ft_strjoin(char const *s1, char const *s2)
{
	int		i;
	int		j;
	char	*new;

	i = 0;
	new = (char *)malloc(sizeof(char) * (ft_strlen(s1) + ft_strlen(s2) + 1));
	if (!new)
		return (NULL);
	while (s1[i] != '\0')
	{
		new[i] = s1[i];
		i++;
	}
	j = 0;
	while (s2[j] != '\0')
	{
		new[i] = s2[j];
		i++;
		j++;
	}
	new[i] = '\0';
	return (new);
}

/*
#include <stdio.h>
#include <stdlib.h>

int main() {
	char *s1 = "hello";
	char *s2 = ", world!";
	printf("el string final es: %s \n", ft_strjoin(s1, s2));
	return 0;
}
*/

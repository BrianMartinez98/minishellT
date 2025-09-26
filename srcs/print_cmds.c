#include <stdio.h>

void print_cmds(char ***cmds)
{
	int i = 0, j;
	if (!cmds)
	{
		printf("cmds is NULL\n");
		return;
	}
	while (cmds[i])
	{
		printf("Comando %d:\n", i);
		j = 0;
		while (cmds[i][j])
		{
			printf("  Arg %d: '%s'\n", j, cmds[i][j]);
			j++;
		}
		i++;
	}
}
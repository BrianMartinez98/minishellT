

#include "../minishell.h"

char	**paths_finder(char **env)
{
	int	i;

	i = 0;
	if (!env)
		return (NULL);
	while (env[i])
	{
		if (ft_strncmp(env[i], "PATH=", 5) == 0)
		{
			if (DEBUG)
			{
				printf("\033[0;35mDEBUG: PATH found:\033[0m\n");
				printf("\033[0;35m%s\033[0m\n\n", env[i]);
			}
			return (ft_split(env[i] + 5, ':'));
		}
		i++;
	}
	return (NULL);
}

char	*command_finder(char **command, char **paths)
{
	int		i;
	char	*pathname;
	char	*trim;
	char	*temp;

	if (!command || !command[0] || !paths)
		return (NULL);
	i = 0;
	while (paths[i])
	{
		trim = ft_strtrim(paths[i], "\n");
		temp = ft_strjoin(trim, "/");
		free(trim);
		pathname = ft_strjoin(temp, command[0]);
		free(temp);
		if (!pathname)
			return (NULL);
		if (access(pathname, X_OK) == 0)
			return (pathname);
		free(pathname);
		i++;
	}
	return (NULL);
}

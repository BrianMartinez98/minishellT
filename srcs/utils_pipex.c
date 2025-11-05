

#include "../minishell.h"

void	free_paths(char **paths)
{
	int	i;

	if (!paths)
		return ;
	i = 0;
	while (paths[i])
	{
		free(paths[i]);
		i++;
	}
	free(paths);
}

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
				printf("\033[0;35m%s\033[0m\n", env[i]);
			return (ft_split(env[i] + 5, ':'));
		}
		i++;
	}
	return (NULL);
}

static char	*get_pathname(char *path, char *command)
{
	char	*trimmed;
	char	*temp;
	char	*pathname;

	trimmed = ft_strtrim(path, "\n");
	if (!trimmed)
		return (NULL);
	temp = ft_strjoin(trimmed, "/");
	free(trimmed);
	if (!temp)
		return (NULL);
	pathname = ft_strjoin(temp, command);
	free(temp);
	return (pathname);
}

char	*command_finder(char **command, char **paths)
{
	int		i;
	char	*pathname;

	if (!paths)
		return (NULL);
	i = 0;
	while (paths[i] != NULL)
	{
		pathname = get_pathname(paths[i], command[0]);
		if (!pathname)
			return (NULL);
		if (access(pathname, X_OK) == 0)
			return (pathname);
		free(pathname);
		i++;
	}
	return (NULL);
}
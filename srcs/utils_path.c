#include "../minishell.h"

static void	check_path_errors(t_shell *shell, char *pathname)
{
	struct stat	st;

	if (stat(pathname, &st) == -1)
	{
		if (errno == ENOENT)
			error_custom(shell, 127, "no such file or directory:", pathname);
	}
	else if (S_ISDIR(st.st_mode))
		error_custom(shell, 126, "is a directory:", pathname);
	else if (access(pathname, X_OK) != 0)
		error_custom(shell, 126, "permission denied:", pathname);
}

char	*ft_execve_prep(char **tokens, t_shell *shell)
{
	char	**paths;
	char	*pathname;

	paths = paths_finder(shell->env);
	pathname = command_finder(tokens, paths);
	free_paths(paths);
	if (!pathname && !is_builtin(tokens))
		error_custom(shell, 127, "minishell: command not found:", tokens[0]);
	if (!tokens[0] || !*tokens[0])
		exit(0);
	if (is_path_like(pathname))
		check_path_errors(shell, pathname);
	return (pathname);
}

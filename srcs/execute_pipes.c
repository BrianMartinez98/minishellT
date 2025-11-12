#include "../minishell.h"

static void	cleanup_fds(int has_next, int pipefd[2], int in_fd)
{
	if (in_fd != -1)
		close(in_fd);
	if (has_next)
	{
		close(pipefd[0]);
		close(pipefd[1]);
	}
}

static void	update_fds(int has_next, int pipefd[2], int *in_fd_ptr)
{
	if (has_next)
	{
		if (pipefd[1] >= 0)
			close(pipefd[1]);
		if (*in_fd_ptr != -1)
			close(*in_fd_ptr);
		*in_fd_ptr = pipefd[0];
	}
	else if (*in_fd_ptr != -1)
	{
		close(*in_fd_ptr);
		*in_fd_ptr = -1;
	}
}

static int	execute_single_pipe(t_shell *shell, char **cmd, int index, int *in_fd_ptr,
							pid_t *pids, int n)
{
	int		pipefd[2];
	int		has_next = (shell->cmds[index + 1] != NULL);
	char	**tokens;
	int		out_fd;
	pid_t	pid;

	if (has_next && pipe(pipefd) == -1)
		handle_error(PIPES, shell);
	if (check_heredoc(cmd, shell) == -1)
	{
		cleanup_fds(has_next, pipefd, *in_fd_ptr);
		return (n);
	}
	filter_args(cmd, &tokens, shell);
	if (has_next)
		out_fd = pipefd[1];
	else
		out_fd = -1;
	pid = execute_command(shell, cmd, tokens, has_next, *in_fd_ptr, out_fd);
	if (pid > 0)
		pids[n++] = pid;
	ft_free_array(&tokens);
	update_fds(has_next, pipefd, in_fd_ptr);
	return (n);
}

void	ft_execute_pipes(t_shell *shell)
{
	int		in_fd = -1;
	pid_t	pids[256] = {0};
	int		n = 0;
	int		i = 0;

	while (shell->cmds[i])
	{
		n = execute_single_pipe(shell, shell->cmds[i], i, &in_fd, pids, n);
		i++;
	}
	if (in_fd != -1)
		close(in_fd);
	ft_wait_children(shell, pids, n);
	restore_stdio(shell);
}

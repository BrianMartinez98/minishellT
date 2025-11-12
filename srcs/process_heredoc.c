#include "../minishell.h"
/*
void	print_heredoc_warning(t_redirection *redir)
{
	char	buffer[256];

	ft_strlcpy(buffer,
		"minishell: warning: here-document delimited by end-of-file (wanted `",
		256);
	ft_strlcat(buffer, redir->file, 256);
	ft_strlcat(buffer, "')", 256);
	ft_putendl_fd(buffer, STDERR_FILENO);
}

int	process_heredoc_line(t_shell *shell, int pipe_write_fd)
{
	char	*line;
	char	*expanded_line;

	line = readline("> ");
	if (!line)
	{
		print_heredoc_warning(redir);                         //NO lo entiendo
		return (1);
	}
	if (ft_strcmp(line, redir->file) == 0)                   //NO lo entiendo
	{
		free(line);
		return (1);
	}
	if (redir->expand_heredoc_content)                       //NO lo entiendo
	{
		expanded_line = expand_heredoc_line(line, shell);
		free(line);
		line = expanded_line;
	}
	ft_putendl_fd(line, pipe_write_fd);
	free(line);
	return (0);
}

int	fill_heredoc_pipe(t_shell *shell, int write_fd)
{
	while (1)
	{
		if (process_heredoc_line(shell, write_fd))
			break ;
	}
	return (0);
}

int	heredoc_parent(char **cmd, t_shell *shell, int *pipe_fds,
		pid_t pid)
{
	int	status;

	close(pipe_fds[1]);
	waitpid(pid, &status, 0);
	if (WIFSIGNALED(status) && WTERMSIG(status) == SIGINT)
	{
		shell->last_status = 130;
		close(pipe_fds[0]);
		return (1);
	}
	else if (WIFEXITED(status) && WEXITSTATUS(status) != 0)
	{
		close(pipe_fds[0]);
		return (1);
	}
	if (cmd->heredoc_fd != -1)                                //NO lo entiendo
		close(cmd->heredoc_fd);
	cmd->heredoc_fd = pipe_fds[0];
	return (0);
}

void	heredoc_child(t_shell *shell, int *pipe_fds)
{
	int	exit_code;

	close(pipe_fds[0]);
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_IGN);
	exit_code = fill_heredoc_pipe(shell, pipe_fds[1]);
	close(pipe_fds[1]);
	exit(exit_code);
}

int	handle_single_heredocs(char **cmd, t_shell *shell)
{
	int		pipe_fds[2];
	pid_t	pid;

	if (pipe(pipe_fds) == -1)
		return (perror("minishell: pipe"), 1);
	pid = fork();
	if (pid == -1)
	{
		close(pipe_fds[0]);
		close(pipe_fds[1]);
		return (perror("minishell: fork"), 1);
	}
	if (pid == 0)
		heredoc_child(shell, pipe_fds);
	else
		return (heredoc_parent(cmd, shell, pipe_fds, pid));
	return (0);
}

int	process_heredocs(t_command *commands, t_struct *mini)
{
	t_redirection	*redir;
	t_command		*cmd;

	cmd = commands;
	while (cmd)
	{
		redir = cmd->redirections;
		while (redir)
		{
			if (redir->type == REDIR_HEREDOC)
			{
				if (handle_single_heredocs(cmd, redir, mini) != 0)
					return (1);
			}
			redir = redir->next;
		}
		cmd = cmd->next;
	}
	return (0);
}*/

#include "../minishell.h"

void	ft_exit(t_shell *shell, char **cmd)
{
	shell->exit = 1;
	ft_putstr_fd("exit ", STDERR);
	if (cmd[1] && cmd[2])
	{
		shell->ret = 1;
		ft_putendl_fd("minishell: exit: too many arguments", STDERR);
	}
	else if (cmd[1] && ft_strisnum(cmd[1]) == 0)
	{
		shell->ret = 255;
		ft_putstr_fd("minishell: exit: ", STDERR);
		ft_putstr_fd(cmd[1], STDERR);
		ft_putendl_fd(": numeric argument required", STDERR);
	}
	else if (cmd[1])
		shell->ret = ft_atoi(cmd[1]);
	else
		shell->ret = 0;
}
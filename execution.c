/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: auspensk <auspensk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/20 15:38:09 by auspensk          #+#    #+#             */
/*   Updated: 2024/09/11 15:59:22 by auspensk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	path_not_found(t_cmd *cmd, t_data *data)
{
	if (data->st_code == 1)
	{
		write(2, "Command \'", ft_strlen("Command \'"));
		write(2, cmd->cmd, ft_strlen(cmd->cmd));
		write(2, "\' not found\n", strlen("\' not found\n"));
		data->st_code = 127;
	}
	if (data->st_code == 2)
	{
		write(2, cmd->cmd, ft_strlen(cmd->cmd));
		write(2, ": Is a directory\n", ft_strlen(": Is a directory\n"));
		data->st_code = 126;
	}
	if (data->st_code == 3)
	{
		write(2, cmd->cmd, ft_strlen(cmd->cmd));
		write(2, ": No such file or directory\n",
			ft_strlen(": No such file or directory\n"));
		data->st_code = 127;
	}
	if (data->st_code == 4)
		data->st_code = 0;
	close(STDIN_FILENO);
	close(STDOUT_FILENO);
	return (clean_exit(NULL, data->st_code, data));
}

int	child_process(t_cmd *cmd, t_data *data)
{
	if (cmd->next)
	{
		close ((data->fd)[0]);
		dup2((data->fd)[1], STDOUT_FILENO);
		close (data->fd[1]);
	}
	if (redirect(cmd))
		return (clean_exit(NULL, 1, data));
	data->st_code = check_command(cmd, data);
	if (data->st_code)
		return (path_not_found(cmd, data));
	execve(cmd->cmd, cmd->args, data->envp);
	close(STDIN_FILENO);
	close(STDOUT_FILENO);
	return (clean_exit(NULL, errno, data));
}

int	fork_function(t_cmd *cmd, t_data *data)
{
	int		pid;
	int		tty_fd;

	pid = fork();
	if (pid == -1)
		return (clean_exit("failed to create child process\n", 1, data));
	if (pid == 0)
		return (child_process(cmd, data));
	if (cmd->next)
	{
		close((data->fd)[1]);
		dup2((data->fd)[0], STDIN_FILENO);
		close((data->fd)[0]);
	}
	else
	{
		tty_fd = open(data->tty_in, O_RDWR, O_APPEND);
		dup2(tty_fd, STDIN_FILENO);
		close(tty_fd);
	}
	if (new_pid(pid, data) != 0)
		return (clean_exit("failed to malloc for pids\n", 1, data));
	return (0);
}

void	wait_loop(t_data *data)
{
	int		wstatus;
	t_pids	*cur_pid;

	cur_pid = data->pids;
	while (cur_pid)
	{
		waitpid(cur_pid->pid, &wstatus, 0);
		cur_pid = cur_pid->next;
	}
	if (WIFEXITED(wstatus))
	{
		data->st_code = WEXITSTATUS(wstatus);
	}
}

int	execute_loop(t_data *data)
{
	t_cmd	*cmd;
	int		tty_fd;

	cmd = data->cmd;
	while (cmd)
	{
		if (cmd->next)
		{
			if (pipe(data->fd) < 0)
				return (clean_exit("failed to create pipe\n", 1, data));
		}
		else if (check_builtin(cmd, data))
			return (clean_exit(NULL, 0, data));
		if (fork_function(cmd, data))
			return (1);
		cmd = cmd->next;
	}
	wait_loop(data);
	tty_fd = open(data->tty_in, O_RDWR, O_APPEND);
	dup2(tty_fd, STDIN_FILENO);
	close(tty_fd);
	return (0);
}



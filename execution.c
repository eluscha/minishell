/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eusatiko <eusatiko@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/20 15:38:09 by auspensk          #+#    #+#             */
/*   Updated: 2024/09/18 13:53:19 by eusatiko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	path_not_found(t_cmd *cmd, t_data *data)
{
	if (cmd->cmd_check == CMDNF)
	{
		write(2, "Command \'", ft_strlen("Command \'"));
		write(2, cmd->cmd, ft_strlen(cmd->cmd));
		write(2, "\' not found\n", strlen("\' not found\n"));
		data->st_code = 127;
	}
	if (cmd->cmd_check == ISDIR)
	{
		write(2, cmd->cmd, ft_strlen(cmd->cmd));
		write(2, ": Is a directory\n", ft_strlen(": Is a directory\n"));
		data->st_code = 126;
	}
	if (cmd->cmd_check == NSCHFL)
	{
		write(2, cmd->cmd, ft_strlen(cmd->cmd));
		write(2, ": No such file or directory\n",
			ft_strlen(": No such file or directory\n"));
		data->st_code = 127;
	}
	close(STDIN_FILENO);
	close(STDOUT_FILENO);
	clean_exit(NULL, data->st_code, data);
	exit(data->st_code);
}

int	child_process(t_cmd *cmd, t_data *data, struct sigaction *sa_ex)
{
	sigaction(SIGINT, sa_ex, NULL);
	if (cmd->next)
	{
		close ((data->fd)[0]);
		dup2((data->fd)[1], STDOUT_FILENO);
		close (data->fd[1]);
	}
	check_builtin(cmd, data);
	if (cmd->cmd_check != BLTN)
	{
		if (redirect(cmd, data))
			return (clean_exit(NULL, 1, data));
		check_command(cmd, data);
		if (cmd->cmd_check != BIN)
			return (path_not_found(cmd, data));
		execve(cmd->cmd, cmd->args, data->envp);
		data->st_code = errno;
	}
	close(STDIN_FILENO);
	close(STDOUT_FILENO);
	clean_exit(NULL, data->st_code, data);
	exit (data->st_code);
}

int	fork_function(t_cmd *cmd, t_data *data, struct sigaction *sa_ex)
{
	int		pid;
	int		tty_fd;

	pid = fork();
	if (pid == -1)
		return (clean_exit("failed to create child process\n", 1, data));
	if (pid == 0)
		return (child_process(cmd, data, sa_ex));
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

int	execute_loop(t_data *data, struct sigaction *sa_ex)
{
	int		tty_fd;
	t_cmd	*cmd;

	cmd = data->cmd;
	if (cmd && !cmd->next && check_builtin (cmd, data))
		return (data->st_code);
	while (cmd)
	{
		if (cmd->next)
		{
			if (pipe(data->fd) < 0)
				return (clean_exit("failed to create pipe\n", 1, data));
		}
		if (fork_function(cmd, data, sa_ex))
			return (1);
		cmd = cmd->next;
	}
	wait_loop(data);
	tty_fd = open(data->tty_in, O_RDWR, O_APPEND);
	dup2(tty_fd, STDIN_FILENO);
	close(tty_fd);
	return (0);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eusatiko <eusatiko@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/20 15:38:09 by auspensk          #+#    #+#             */
/*   Updated: 2024/10/11 11:38:19 by eusatiko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	exec_child(t_cmd *cmd, t_data *data)
{
	if (!redirect(cmd, data))
	{
		check_command(cmd, data);
		if (cmd->cmd_check != BIN)
			path_not_found(cmd, data);
		close(data->std_in);
		execve(cmd->cmd, cmd->args, data->envp);
		perror(cmd->cmd);
		data->st_code = errno;
		if (data->st_code == 13 || data->st_code == 22)
			data->st_code = 126;
	}
}

int	child_process(t_cmd *cmd, t_data *data)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	data->child = 1;
	if (cmd->next)
	{
		close ((data->fd)[0]);
		dup2((data->fd)[1], STDOUT_FILENO);
		close (data->fd[1]);
	}
	check_builtin(cmd, data);
	if (cmd->cmd_check != BLTN)
		exec_child(cmd, data);
	close(STDIN_FILENO);
	close(STDOUT_FILENO);
	close(data->std_in);
	exit(clean_exit(NULL, data->st_code, data));
}

int	fork_function(t_cmd *cmd, t_data *data)
{
	int		pid;

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
		dup2(data->std_in, STDIN_FILENO);
		close(data->std_in);
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
	wstatus = 0;
	while (cur_pid)
	{
		waitpid(cur_pid->pid, &wstatus, 0);
		cur_pid = cur_pid->next;
	}
	if (WIFEXITED(wstatus))
        data->st_code = WEXITSTATUS(wstatus);
    else if (WIFSIGNALED(wstatus))
	{
        lastsignal = WTERMSIG(wstatus);
		data->st_code = lastsignal + 128;
	}
}

int	execute_loop(t_data *data)
{
	int		tty_fd;
	t_cmd	*cmd;

	lastsignal = 0;
	cmd = data->cmd;
	if (cmd && !cmd->next && check_builtin(cmd, data))
		return (data->st_code);
	while (cmd)
	{
		if (cmd->next)
		{
			if (pipe(data->fd) < 0)
				return (clean_exit(strerror(errno), 1, data));
		}
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

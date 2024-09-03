/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: auspensk <auspensk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/03 14:52:52 by auspensk          #+#    #+#             */
/*   Updated: 2024/09/03 17:25:02 by auspensk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	print_array(char **array)
{
	int	i;

	i = 0;
	while (array[i])
	{
		write(1, array[i], ft_strlen(array[i]));
		write(1, "\n", 1);
		i++;
	}
	return (1);
}

char	*check_argument(char *arg)
{
	int		i;
	char	*key;

	i = 0;
	while (ft_isalnum(arg[i]))
		i++;
	if (arg[i] == '\0')
		return (arg);
	if (arg[i] == '=')
		key = ft_substr(arg, 0, i + 1);
	else
	{
		write(1, "export: ", ft_strlen("export "));
		write(1, arg, ft_strlen(arg));
		write(1, ": not a valid identifier\n", 25);
		key = NULL;
	}
	return (key);
}

int	find_key(char *key, char *arg, char **envp)
{
	int	i;

	i = 0;
	while (envp[i])
	{
		if (!ft_strncmp(key, envp[i], ft_strlen(key)))
		{
			if (ft_strlen(key) <= ft_strlen(arg))
				return (1);
			free(envp[i]);
			envp[i] = ft_strdup(arg);
			return (1);
		}
		i++;
	}
	return (0);
}

void	add_entry(char *entry, char **envp)
{
	int	i;

	i = 0;
	while (envp[i])
		i++;
	envp[i] = ft_strdup(entry);
	envp[i + 1] = NULL;
}

int	ft_export(t_cmd *cmd, t_data *data)
{
	int		i;
	char	*key;

	i = 0;
	if (!cmd->args[1])
		return (print_array(data->envp));
	else
	{
		key = check_argument(cmd->args[1]);
		if (!find_key(key, cmd->args[1], data->envp))
			add_entry(cmd->args[1], data->envp);
		free(key);
	}
	return (1);
}

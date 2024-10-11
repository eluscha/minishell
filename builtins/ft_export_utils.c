/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: auspensk <auspensk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/10 10:47:44 by auspensk          #+#    #+#             */
/*   Updated: 2024/10/11 12:42:31 by auspensk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	print_array(char **array)
{
	int	i;

	i = 0;
	if (!array)
		return (1);
	while (array[i])
	{
		write(STDOUT_FILENO, array[i], ft_strlen(array[i]));
		write(STDOUT_FILENO, "\n", 1);
		i++;
	}
	return (0);
}

int	add_entry(char *entry, char **envp, t_data *data)
{
	int	i;

	i = 0;
	while (envp[i])
		i++;
	envp[i] = ft_strdup(entry);
	if (!envp[i])
		data->st_code = 1;
	envp[i + 1] = NULL;
	return (0);
}

int	unset_variable(int i, char **envp, t_data *data)
{
	while (envp[i])
	{
		free (envp[i]);
		envp[i] = NULL;
		if (envp[i + 1])
		{
			envp[i] = ft_strdup(envp[i + 1]);
			if (!envp[i])
			{
				data->st_code = 1;
				return (1);
			}
		}
		i++;
	}
	return (0);
}

int	do_unset_ex(t_export *ex, char **envp, t_data *data, int i)
{
	if (ex->type == UNSET && ft_strlen(ex->key) == ft_strlen(ex->arg))
		return (unset_variable(i, envp, data));
	else if (ft_strlen(ex->key) == ft_strlen(ex->arg))
		return (0);
	else if (ex->type == EXPORT)
	{
		free(envp[i]);
		envp[i] = ft_strdup(ex->arg);
		if (!envp[i])
			data->st_code = 1;
		return (0);
	}
	return (0);
}

int	find_key(t_export *ex, char **envp, t_data *data)
{
	int	i;

	i = 0;
	while (envp[i])
	{
		if (!ft_strncmp(ex->key, envp[i], ft_strlen(ex->key)))
		{
			i = do_unset_ex(ex, envp, data, i);
			break ;
		}
		i++;
	}
	if (ex->key)
		free(ex->key);
	ex->key = NULL;
	return (i);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: auspensk <auspensk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/10 10:47:44 by auspensk          #+#    #+#             */
/*   Updated: 2024/09/16 15:18:39 by auspensk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	print_array(char **array)
{
	int	i;

	i = 0;
	if (!array)
		return (0);
	while (array[i])
	{
		write(1, array[i], ft_strlen(array[i]));
		write(1, "\n", 1);
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

int	find_key(t_export *export, char **envp, t_data *data)
{
	int	i;

	i = 0;
	while (envp[i])
	{
		if (!ft_strncmp(export->key, envp[i], ft_strlen(export->key)))
		{
			if (export->type == UNSET
				&& ft_strlen(export->key) == ft_strlen(export->arg))
				return (unset_variable(i, envp, data));
			else if (ft_strlen(export->key) == ft_strlen(export->arg))
				return (0);
			else if (export->type == EXPORT)
			{
				free(envp[i]);
				envp[i] = ft_strdup(export->arg);
				if (!envp[i])
					data->st_code = 1;
				return (0);
			}
		}
		i++;
	}
	return (1);
}

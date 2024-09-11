/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: auspensk <auspensk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/10 10:47:44 by auspensk          #+#    #+#             */
/*   Updated: 2024/09/11 10:41:38 by auspensk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

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

void	add_entry(char *entry, char **envp)
{
	int	i;

	i = 0;
	while (envp[i])
		i++;
	envp[i] = ft_strdup(entry);
	envp[i + 1] = NULL;
}

int	find_key(t_export *export, char **envp)
{
	int	i;

	i = 0;
	while (envp[i])
	{
		if (!ft_strncmp(export->key, envp[i], ft_strlen(export->key)))
		{
			if (ft_strlen(export->key) == ft_strlen(export->arg))
			{
				if (export->type == EXPORT)
					return (1);
				else
					return (unset_variable(i, envp));
			}
			free(envp[i]);
			envp[i] = ft_strdup(export->arg);
			return (1);
		}
		i++;
	}
	return (0);
}

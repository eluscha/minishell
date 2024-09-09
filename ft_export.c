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

char	*check_argument(t_export *export)
{
	int		i;
	char	*key;

	i = 0;
	if (export->arg[0] == '\0')
		return (NULL);
	while (ft_isalnum(export->arg[i]))
		i++;
	if (export->arg[i] == '\0')
		return (strdup(export->arg));
	if (export->arg[i] == '=' && export->type == EXPORT)
		key = ft_substr(export->arg, 0, i);
	else
	{
		if (export->type == EXPORT)
			write(1, "export: ", ft_strlen("export:"));
		if (export->type == UNSET)
			write(1, "unset: ", ft_strlen("unset:"));
		write(1, export->arg, ft_strlen(export->arg));
		write(1, ": not a valid identifier\n", 25);
		key = NULL;
	}
	return (key);
}

int	unset_variable(t_export *export, int i, char **envp)
{
	while (envp[i])
	{
		free (envp[i]);
		envp[i] = envp[i + 1];
		i++;
	}
	return (1);
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
				if (export->type == EXPORT)
					return (1);
				else
					return (unset_variable(export, i, envp));
			free(envp[i]);
			envp[i] = ft_strdup(export->arg);
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
	{
		printf("Entry N%d: %s\n", i, envp[i]);
		i++;
	}
	envp[i] = ft_strdup(entry);
	printf("Entry added at %d: %s", i, envp[i]);
	envp[i + 1] = NULL;
}

int	ft_export(t_cmd *cmd, t_data *data)
{
	t_export	export;

	if (!cmd->args[1])
		return (print_array(data->envp));
	else
	{
		export.arg = cmd->args[1];
		export.type = EXPORT;
		export.key = check_argument(export.arg);
		if (!export.key)
			return (1);
		if (!find_key(&export, data->envp))
			add_entry(&export, data->envp);
		free(export.key);
	}
	return (1);
}

int	ft_unset(t_cmd *cmd, t_data *data)
{
	t_export	export;

	if (!cmd->args[1])
		return (1);
	else
	{
		export.arg = cmd->args[1];
		export.type = UNSET;
		export.key = check_argument(&export);
		if (!export.key)
			return (1);
		return (find_key(&export, data));
		write(1, "\n\nAfter unset:\n", strlen("\n\nAfter unset:\n"));
		print_array(data->envp);
	}
}

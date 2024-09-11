/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: auspensk <auspensk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/03 14:52:52 by auspensk          #+#    #+#             */
/*   Updated: 2024/09/11 10:41:45 by auspensk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

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

int	unset_variable(int i, char **envp)
{
	while (envp[i])
	{
		free (envp[i]);
		if (envp[i + 1])
			envp[i] = ft_strdup(envp[i + 1]);
		else
			envp[i] = NULL;
		i++;
	}
	return (1);
}


int	ft_export(t_cmd *cmd, t_data *data)
{
	t_export	export;

	if (redirect(cmd))
		return (clean_exit(NULL, 1, data));
	if (!cmd->args[1])
		return (print_array(data->envp));
	else
	{
		export.arg = cmd->args[1];
		export.type = EXPORT;
		export.key = check_argument(&export);
		if (!export.key)
			return (1);
		if (!find_key(&export, data->envp))
			add_entry(export.arg, data->envp);
		free(export.key);
	}
	return (1);
}

int	ft_unset(t_cmd *cmd, t_data *data)
{
	t_export	export;

	if (redirect(cmd))
		return (clean_exit(NULL, 1, data));
	if (!cmd->args[1])
		return (1);
	else
	{
		export.arg = cmd->args[1];
		export.type = UNSET;
		export.key = check_argument(&export);
		if (!export.key)
			return (1);
		return (find_key(&export, data->envp));
	}
}

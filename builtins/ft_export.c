/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: auspensk <auspensk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/03 14:52:52 by auspensk          #+#    #+#             */
/*   Updated: 2024/09/27 17:07:55 by auspensk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*export_err(t_export *export, t_data *data)
{
	if (export->type == EXPORT)
		write(2, "export: '", ft_strlen("export: '"));
	if (export->type == UNSET)
		write(2, "unset: '", ft_strlen("unset: '"));
	write(2, export->arg, ft_strlen(export->arg));
	write(2, "': not a valid identifier\n", 26);
	data->st_code = 1;
	return (NULL);
}

char	*check_argument(t_export *export, t_data *data)
{
	int		i;
	char	*res;

	i = 0;
	res = NULL;
	if (!ft_isalpha(export->arg[i]) && export->arg[i] != '_')
		return (export_err(export, data));
	while (ft_isalnum(export->arg[i]) || export->arg[i] == '_')
		i++;
	if (i && export->arg[i] == '\0')
		res = strdup(export->arg);
	else if (i && export->arg[i] == '=' && export->type == EXPORT)
		res = ft_substr(export->arg, 0, i);
	else
		return (export_err(export, data));
	if (!res)
		data->st_code = 1;
	return (res);
}

int	unset_variable(int i, char **envp, t_data *data)
{
	while (envp[i])
	{
		free (envp[i]);
		if (envp[i + 1])
			envp[i] = ft_strdup(envp[i + 1]);
		if (!envp[i])
			data->st_code = 1;
		else
			envp[i] = NULL;
		i++;
	}
	return (0);
}

int	ft_export(char *arg, t_cmd *cmd, t_data *data)
{
	t_export	export;
	int			i;

	i = 0;
	data->st_code = 0;
	if (cmd)
	{
		cmd->cmd_check = BLTN;
		if (redirect(cmd, data))
			return (1);
		if (!cmd->args[1])
			return (print_array(data->envp));
	}
	export.arg = arg;
	export.type = EXPORT;
	export.key = check_argument(&export, data);
	if (!export.key)
		return (1);
	if (find_key(&export, data->envp, data))
		i = add_entry(export.arg, data->envp, data);
	free(export.key);
	return (i);
}

int	ft_unset(t_cmd *cmd, t_data *data)
{
	t_export	export;

	data->st_code = 0;
	cmd->cmd_check = BLTN;
	if (redirect(cmd, data))
		return (1);
	if (!cmd->args[1])
		return (0);
	export.arg = cmd->args[1];
	export.type = UNSET;
	export.key = check_argument(&export, data);
	if (!export.key)
		return (1);
	return (find_key(&export, data->envp, data));
}

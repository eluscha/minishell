/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: auspensk <auspensk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/03 14:52:52 by auspensk          #+#    #+#             */
/*   Updated: 2024/10/02 10:36:10 by auspensk         ###   ########.fr       */
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
		res = ft_strdup(export->arg);
	else if (i && export->arg[i] == '=' && export->type == EXPORT)
		res = ft_substr(export->arg, 0, i);
	else
		return (export_err(export, data));
	if (!res)
		data->st_code = 1;
	return (res);
}

int	export_loop(t_cmd *cmd, t_data *data, t_export *export)
{
	int	i;
	int	ret;

	i = 1;
	ret = 0;
	while ((cmd && cmd->args[i]) || !cmd)
	{
		if (cmd)
			export->arg = cmd->args[i];
		export->type = EXPORT;
		export->key = check_argument(export, data);
		if (!export->key)
			return (1);
		if (find_key(export, data->envp, data))
			ret = add_entry(export->arg, data->envp, data);
		if (export->key)
			free(export->key);
		export->key = NULL;
		if (!cmd)
			break ;
		i++;
	}
	return (ret);
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
	if (!cmd)
		export.arg = arg;
	i = export_loop(cmd, data, &export);
	return (i);
}

int	ft_unset(t_cmd *cmd, t_data *data)
{
	t_export	export;
	int			i;

	data->st_code = 0;
	cmd->cmd_check = BLTN;
	if (redirect(cmd, data))
		return (1);
	i = 1;
	if (!cmd->args[i])
		return (0);
	while (cmd->args[i])
	{
		export.arg = cmd->args[i];
		export.type = UNSET;
		export.key = check_argument(&export, data);
		if (!export.key)
			return (1);
		if (find_key(&export, data->envp, data))
			return (0);
		i++;
	}
	return (0);
}

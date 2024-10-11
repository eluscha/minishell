/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   iterate_shlvl.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: auspensk <auspensk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/11 10:28:57 by auspensk          #+#    #+#             */
/*   Updated: 2024/10/11 10:57:16 by auspensk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	find_envp(char **envp, char *key)
{
	int	i;

	i = 0;
	while (envp[i])
	{
		if (!ft_strncmp(envp[i], key, ft_strlen(key)))
			return (i);
		i++;
	}
	return (-1);
}

void	iterate_shlvl(t_data *data)
{
	int		i;
	int		lvl;
	char	*shlvl;

	i = find_envp(data->envp, "SHLVL=");
	if (i < 0)
		ft_export("SHLVL=1", NULL, data);
	else
	{
		lvl = ft_atoi(&(data->envp[i][6]));
		lvl ++;
		shlvl = ft_strjoin("SHLVL=", ft_itoa(lvl));
		ft_export(shlvl, NULL, data);
	}
}

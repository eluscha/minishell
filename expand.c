#include "minishell.h"

int	handle_expand(char *start, t_tok *tail, t_data *data, int *err)
{
	char	*varvalue;
	char	*varname;
	int		i;

	i = 0;
	if (start[i] == '?' && ++i)
		varvalue = ft_itoa(data->st_code);
	else
	{
		while (start[i] && ft_isalnum(start[i]))
			i++;
		if (!i)
		{
			tail->word[tail->idx++] = '$';
			return (0);
		}
		varname = find_var(data->envp, start, i);
		if (!varname)
			varvalue = (ft_strdup(""));
		else
			varvalue = ft_strdup(varname + i + 1);
	}
	*err = change_word(tail, varvalue, start + i);
	return (i);
}

char *find_var(char **list, char *start, int i)
{
	while (*list)
	{
		if (ft_strncmp(*list, start, i) == 0 && (*list)[i] == '=')
			break ;
		list++;
	}
	return (*list);
}

int	change_word(t_tok *token, char *var, char *start)
{
	int		lenword;
	int		lenvar;
	int		sum;
	char	*newword;

	if (!var)
		return (-1);
	lenword = ft_strlen(token->word);
	lenvar = ft_strlen(var);
	sum = lenword + lenvar + ft_strlen(start);
	newword = ft_calloc(sum + 1, sizeof(char));
	if (!newword)
	{
		free(var);
		return (-1);
	}
	ft_strlcpy(newword, token->word, lenword + 1);
	ft_strlcat(newword, var, sum + 1);
	free(token->word);
	free(var);
	token->word = newword;
	token->idx += lenvar;
	return (0);
}

#include "minishell.h"

int	handle_expand(t_lex *lex, int dist)
{
	char	*varvalue;
	int		i;
	char	*start;

	i = 0;
	start = lex->input + dist;
	if (start[i] == '?' && ++i)
		varvalue = ft_itoa(lex->st_code);
	else
	{
		while (start[i] && ft_isalnum(start[i]))
			i++;
		if (!i)
		{
			lex->tail->word[lex->tail->idx++] = '$';
			return (0);
		}
		varvalue = find_var(lex->envp, start, i);
	}
	if (!varvalue)
		lex->err = -1;
	else
		change_word(lex->tail, varvalue, start + i);
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
	if (!*list)
		return (ft_strdup(""));
	else
		return (ft_strdup(*list + i + 1));
}

int	change_word(t_tok *token, char *var, char *start)
{
	int		lenword;
	int		lenvar;
	int		sum;
	char	*newword;

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
	if (!tail->idx)
		*lex->state = EXPAND;
	return (0);
}

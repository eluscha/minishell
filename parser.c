/*
#include <readline/readline.h>
#include <readline/history.h>
#include <stdlib.h>
#include "libft.h"

#include <stdio.h>
*/


typedef enum lex_state
{
    WORD,
    DELIM,
    INSQTS,
    INDQTS
} lex_state;

typedef enum e_toktype
{
    UNDETERM,
    END,
    SQERR,
    DQERR,
    NWLINE,
    PIPE,
    CMD,
    ARGS,
    DISCARD,
    HEREDOC,
    INPUT,
    OUTPUT,
    APPEND
}   t_toktype;

typedef struct s_tok
{
    char *word;
    int idx;
    struct s_tok *next;
    t_toktype type;
}   t_tok;


/*  !!!! NOTE :
typedef struct redirect
{
    t_toktype        type;   // changed YOUR struct here !!
    char            *value;
    struct redirect *next;
}   t_redirect;
*/

t_cmd	*parser(char *input, char **envp);
t_tok *lexer(char *input, lex_state state, t_tok *tail, char **envp);
t_tok *gen_token(t_toktype type, int len);
char    *expand(char *start, int *lenvar, char **envp);
void    change_word(t_tok *token, char *var, int len);
int process_tokens(t_tok *head);
int    io_type(t_tok *token, t_toktype type);
void insert_token(t_tok *token);
int check_syntax(t_tok *head);
t_cmd *generate_structs(t_tok *head, int numargs);

//void print_struct(t_cmd *cmd);
//void print_toktype(t_tok *token)


t_tok *gen_token(t_toktype type, int len)
{
    t_tok *token = malloc(sizeof(t_tok));
    token->next = NULL;
    token->type = type;
    token->word = ft_calloc(len + 1, sizeof(char));
    token->idx = 0;
    if (!token->word)
    {
        free(token);
        return (NULL);
    }
    if (type == END)
        ft_strlcpy(token->word, "newline", 8);
    return token; //need to handle if it is NULL  
}

char    *expand(char *start, int *lenvar, char **envp)
{
    int i = 0;
    while(start[i] && ft_isalnum(start[i]))
        i++;
    *lenvar = i;
    char *var = ft_strdup(start);
    var[i] = '\0';
    int len = ft_strlen(var);
    while (*envp)
	{
		if (ft_strncmp((const char *) *envp, var, len) == 0 || *envp[len] == '=')
			break ;
		envp++;
	}
    free(var);
	if (!*envp)
		return (ft_strdup(""));
    return (ft_strdup(*envp + len + 1)); //need to free though later
}

void    change_word(t_tok *token, char *var, int len)
{
    int lenvar = ft_strlen(var);
    int lenword = ft_strlen(token->word);
    char *newword = ft_calloc(lenvar + len + 1, sizeof(char));
    ft_strlcpy(newword, token->word, lenword + 1);
    ft_strlcat(newword, var, lenvar + len + 1);
    free(token->word);
    free(var);
    token->word = newword;
    token->idx += lenvar;
    //printf("idx is %d\n", token->idx);
}

t_tok *lexer(char *input, lex_state state, t_tok *tail, char **envp)
{
    char c;
    t_tok *head;
    int len = ft_strlen(input);
    int i = -1;

    int lenvar;
    char *varvalue;

    if (!tail)
    {
        tail = gen_token(UNDETERM, len); // need to protect malloc
        head = tail;
    }
    else
    {
        head = tail->next;
        tail->next = gen_token(NWLINE, 1);
        tail = tail->next;
        tail->word[tail->idx++] = '\n';
        tail->next = gen_token(UNDETERM, len);
        tail = tail->next;
    }
    while(input[++i])
    {
        c = input[i];
        if (state == DELIM)
        {
            if (c == ' ' || c == '\t')
                continue;
            state = WORD;
        }
        if (c == '\'')
        {
            if (state == INSQTS)
                state = WORD;
            else if (state == WORD)
                state = INSQTS;
            else
                tail->word[tail->idx++] = c;
        }
        else if (c == '\"')
        {
            if (state == INDQTS)
                state = WORD;
            else if (state == WORD)
                state = INDQTS;
            else
                tail->word[tail->idx++] = c;
        }
        else if (state == WORD && (c == ' ' || c == '\t'))
        {
            state = DELIM;
            tail->next = gen_token(UNDETERM, len);
            tail = tail->next;
        }
        else if (c == '$' && (state == WORD || state == INDQTS))
        {
            varvalue = expand(input + i + 1, &lenvar, envp);
            change_word(tail, varvalue, len);
            i += lenvar;
        }
        else
            tail->word[tail->idx++] = c;
    }
    if (state == INSQTS)
        tail->type = SQERR;
    else if (state == INDQTS)
        tail->type = DQERR;
    else if (state == DELIM)
        tail->type = END;
    else
    {
        tail->next = gen_token(END, 7);
        tail = tail->next;
    }
    tail->next = head;
    return (tail);
}

t_cmd	*parser(char *input, char **envp)
{
    int numargs;
    t_tok *tail;
    t_tok *head;
    t_tok *ptr;
    t_cmd *cmds = NULL;
    /*  THIS is preliminary readline for multiple line input..
    while (1)
    {
        if (input)
            free(input);
        input = readline("prompt> ");
        if (input == NULL) //will happen with ctrl+D
            break;
        if (*input)
            add_history(input);
        tail = lexer(input, DELIM, NULL, envp);
        while (tail->type == SQERR)
        {
            input = readline("> ");
            if (input == NULL) //will happen with ctrl+D
                break;
            tail = lexer(input, INSQTS, NULL, envp);
        }
        while (tail->type == DQERR)
        {
            input = readline("> ");
            if (input == NULL) //will happen with ctrl+D
                break;
            tail = lexer(input, INDQTS, tail, envp);
        }
        head = tail->next;
        */

    tail = lexer(input, DELIM, NULL, envp);
    head = tail->next;
    numargs = process_tokens(head);
    t_cmd *cmds = NULL;
    if (check_syntax(head) == 0)
        cmds = generate_structs(head, numargs);
        
        /* THIS IS for printing structs
        t_cmd *ptrs = cmds;
        while (ptrs)
        {
            print_struct(ptrs);
            ptrs = ptrs->next;
        }
        */

    while (head->type != END)
    {
        if (head->word)
            free(head->word);
        ptr = head->next;
        free(head);
        head = ptr;
    }
    free(head->word);
    free(head);

    return (cmds); 
}

int    io_type(t_tok *token, t_toktype type)
{
    token->type = type;
    char *io_arg;
    int idx;
    if (type == HEREDOC || type == APPEND)
        idx = 2;
    else
        idx = 1;
    if (!token->word[idx])
    {
        token->type = DISCARD;
        if (token->next->type != END)
            token->next->type = type;
    }
    else
    {
        token->type = type;
        io_arg = ft_strdup(token->word + idx);
        if (!io_arg)
            return (1);
        free(token->word);
        token->word = io_arg; 
    }
    return (0);
}

void insert_token(t_tok *token)
{
    char *word = ft_strdup(token->word + 1);
    t_tok *next = token->next;
    token->next = gen_token(UNDETERM, ft_strlen(word));
    token->next->word = word;
    token->next->next = next;
    token->word[1] = '\0';
}

int process_tokens(t_tok *token) //maybe return -1 always on malloc err ?
{
    int cmd = 0;
    int args = 0;
    while (token->type != END)
    {
        if (token->word[0] == '|')
        {
            token->type = PIPE;
            cmd = 0;
            if (token->word[1])
                insert_token(token);
        }
        else if (token->type >= HEREDOC)
        {   
            token = token->next;
            continue ;
        }
        else if (token->word[0] == '<')
        {
            if (token->word[1] == '<')
                io_type(token, HEREDOC);
            else
                io_type(token, INPUT);
        }    
        else if (token->word[0] == '>')
        {
            if (token->word[1] == '>')
                io_type(token, APPEND);
            else
                io_type(token, OUTPUT);
        }
        else if (!cmd)
        {
            token->type = CMD;
            cmd = 1;
        }
        else
        {
            token->type = ARGS;
            args++;
        }
        token = token->next;
    }
    return args;
}

int check_syntax(t_tok *head)
{
    t_toktype ntype;
    int err = 0;
    while(head->type != END)
    {
        ntype = head->next->type;
        if (head->type == PIPE && ntype != CMD && ntype < DISCARD)
            err = 1;
        else if (head->type == DISCARD && ntype < HEREDOC)
            err = 1;
        if (err)
        {
            printf("syntax error near unexpected token `");
            printf("%s\'\n", head->next->word);
            return (1);
        }
        head = head->next;
    }
    return (0);
}

t_cmd *generate_structs(t_tok *head, int numargs)
{
    t_cmd *cmd = ft_calloc(1, sizeof(t_cmd));
    cmd->args = ft_calloc(numargs + 2, sizeof(char *));
    t_redirect *ptr;
    t_redirect **ptradr;
    int idx = 1;
    while (head->type != END)
    {
        if (head->type == PIPE)
        {
            cmd->next = generate_structs(head->next, numargs);
            break ;
        }
        else if (head->type == CMD)
            cmd->cmd = ft_strdup(head->word); // need to protect
        else if (head->type == ARGS)
            cmd->args[idx++] = head->word;
        else if (head->type >= HEREDOC)
        {
            if (head->type >= OUTPUT)
            {
                ptr = cmd->out_redirect;
                ptradr = &cmd->out_redirect;
            }
            else
            {
                ptr = cmd->in_redirect;
                ptradr = &cmd->in_redirect;
            }
            while (ptr)
            {
                ptr = ptr->next;
                ptradr = &ptr->next;
            }
            ptr = ft_calloc(1, sizeof(t_redirect));
            ptr->value = ft_strdup(head->word); //need to protect
            ptr->type = head->type;
            *ptradr = ptr; 
        }
        head = head->next;
    }
    return (cmd);
}

/*
void print_struct(t_cmd *cmd)
{
    if (!cmd)
        return ;
    printf("CMD: %s ARGS: ", cmd->cmd);
    int i = -1;
    while (cmd->args[++i])
        printf("%s ", cmd->args[i]);
    printf("%s\n", cmd->args[i]);
    t_redirect *ptr = cmd->in_redirect;
    while (ptr)
    {
        printf("in_redirect type is %d value is %s\n", ptr->type, ptr->value);
        ptr = ptr->next;
    }
    ptr = cmd->out_redirect;
    while (ptr)
    {
        printf("out_redirect type is %d value is %s\n", ptr->type, ptr->value);
        ptr = ptr->next;
    }
} 

void print_toktype(t_tok *token)
{
    if (token->type == UNDETERM)
        printf("UNDETERM ");
    else if (token->type == END)
        printf("END\n");
    else if (token->type == SQERR)
        printf("SQERR ");
    else if (token->type == DQERR)
        printf("DQERR ");
    else if (token->type == NWLINE)
        printf("NWLINE ");
    else if (token->type == PIPE)
        printf("PIPE ");
    else if (token->type == CMD)
        printf("CMD ");
    else if (token->type == ARGS)
        printf("ARGS ");
    else if (token->type == HEREDOC)
        printf("HEREDOC ");
    else if (token->type == INPUT)
        printf("INPUT ");
    else if (token->type == OUTPUT)
        printf("OUTPUT ");
    else if (token->type == APPEND)
        printf("APPEND ");
    else if (token->type == DISCARD)
        printf("DISCARD ");
}
*/
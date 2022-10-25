#include "../../includes/lexer.h"
#include "../../includes/minishell.h"

//#include "../../libft/libft.h"

#include "lexer.h"

t_token_list	*init_token(t_token_list *prev, t_token_type type)
{
	t_token_list	*ret_token;

	ret_token = malloc(sizeof(t_token_list));
	if (!ret_token)
	{
		//未処理
		printf("malloc error\n");
	}
	//未処理　確保する領域の範囲を修正
	ret_token->comp = malloc(sizeof(char) * 100);
	if (!ret_token->comp)
	{
		//未処理
		printf("malloc error\n");
	}
	ret_token->next = NULL;
	ret_token->prev = prev;
	ret_token->type = type;
	ret_token->comp[0] = '\0';
	return (ret_token);
}

void	init_token_info(t_token_info *token_info, char *av)
{
	token_info->str_i = 0;
	token_info->each_i = 0;
	token_info->len = ft_strlen(av);
	token_info->token = init_token(NULL, CHAR_OTHER);
	token_info->first_token = token_info->token;
	token_info->quote_flag = false;
}

t_token_type	get_token_type(char c)
{
	if (c == '\\')
		return (CHAR_BACKSLASH);
	else if (c == ';')
		return (CHAR_SEMICOLON);
	else if (c == '\'')
		return (CHAR_QUOTE);
	else if (c == '\"')
		return (CHAR_D_QUOTE);
	else if (c == '|')
		return (CHAR_PIPE);
	else if (c == '<')
		return (CHAR_LESS);
	else if (c == '>')
		return (CHAR_GREATER);
	else if (c == ' ')
		return (CHAR_SPACE);
	else if (c == '\t')
		return (CHAR_TAB);
	else
		return (CHAR_OTHER);
}

void	add_new_token_list(t_token_info *token_info, t_token_type type)
{
	t_token_list	*new_token;

	token_info->token->comp[token_info->each_i] = '\0';
	new_token = init_token(token_info->token, type);
	token_info->token->next = new_token;
	//トークンのアドレスを変更することで現在のトークンは実質空みたいに
	token_info->token = new_token;
	token_info->each_i = 0;
	token_info->quote_flag = false;
	token_info->status = NOT_QUOTED;
}

void	type_char_other_process(t_token_info *token_info, t_token_type type, char *str)
{
	if (token_info->each_i != 0 && (type == CHAR_SPACE || type == CHAR_TAB))
		add_new_token_list(token_info, type);
	//特殊な文字の時の処理
	else if (type != CHAR_SPACE && type != CHAR_TAB && type != CHAR_OTHER)
	{
		//必要以上に新しいトークンを作らないような処理
		if (token_info->each_i != 0)
			add_new_token_list(token_info, type);
		token_info->token->type = type;
		if (type == CHAR_QUOTE || type == CHAR_D_QUOTE)
		{
			if (type == CHAR_QUOTE)
				token_info->status = QUOTED;
			else
				token_info->status = D_QUOTED;
			token_info->quote_flag = true;
			token_info->token->comp[token_info->each_i++] = str[token_info->str_i];
		}
		else
		{
			token_info->token->comp[token_info->each_i++] = str[token_info->str_i];
			if (get_token_type(str[token_info->str_i + 1]) == CHAR_OTHER)
				add_new_token_list(token_info, CHAR_OTHER);
		}
	}
}

void	status_quoted_process(t_token_info *token_info, t_token_type type, char *str)
{
	if (token_info->status == QUOTED && type == CHAR_QUOTE)
	{
		token_info->token->comp[token_info->each_i++] = str[token_info->str_i];
		token_info->quote_flag = false;
	}
	else if (token_info->status == D_QUOTED && type == CHAR_D_QUOTE)
	{
		token_info->token->comp[token_info->each_i++] = str[token_info->str_i];
		token_info->quote_flag = false;
	}
		//quote内の文字をどんどんいれる
	else
		token_info->token->comp[token_info->each_i++] = str[token_info->str_i];
}

t_token_list	*split_token(char *av)
{
	t_token_info	token_info;
	t_token_type	type;

	init_token_info(&token_info, av);
	//全ての文字は一つずつこのwhile文でみていくのがいいかも
	while (av[token_info.str_i] != '\0')
	{
		type = get_token_type(av[token_info.str_i]);
		if (token_info.quote_flag == false)
		{
			//特殊な文字じゃないときの普通の処理
			if (type == CHAR_OTHER)
			{
				if (token_info.each_i == 0)
					token_info.token->type = type;
				token_info.token->comp[token_info.each_i++] = av[token_info.str_i];
			}
			//新しいトークンが必要な処理->CHAR_OTHERじゃない処理
			else
				type_char_other_process(&token_info, type, av);
		}
		else
			status_quoted_process(&token_info, type, av);
		token_info.str_i++;
	}
	token_info.token->comp[token_info.each_i] = '\0';
	return (token_info.first_token);
}

void	print_type(t_token_type type)
{
	char	*str;
	if (type == CHAR_BACKSLASH)
		str = "\\";
	else if (type == CHAR_SEMICOLON)
		str = ";";
	else if (type == CHAR_QUOTE)
		str = "\'";
	else if (type == CHAR_D_QUOTE)
		str = "\"";
	else if (type == CHAR_PIPE)
		str = "|";
	else if (type == CHAR_LESS)
		str = "<";
	else if (type == CHAR_GREATER)
		str = ">";
	else if (type == CHAR_SPACE)
		str = "SPACE";
	else if (type == CHAR_TAB)
		str = "TAB";
	else if (type == CHAR_OTHER)
		str = "OTHER";
	printf("type->[%s]\n", str);
}

int main(int ac, char **av)
{
	t_token_list	*token;
	if (ac != 2)
		return (1);
	printf("av : %s\n", av[1]);
	token = split_token(av[1]);
	int i = 0;
	while (token)
	{
		printf("%d [%s]\t", i, token->comp);
		print_type(token->type);
		token = token->next;
		i++;
	}
	return (0);
}

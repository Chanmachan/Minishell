#include "../../includes/minishell.h"

//#include "../../includes/minishell.h"
//#include "../../libft/libft.h"

t_node	*create_and_init_node()
{
	t_node	*new_node;

	new_node = (t_node *) malloc(sizeof(t_node));
	if (!new_node)
		util_put_cmd_err_and_exit("malloc");
	new_node->command = (t_command *) malloc(sizeof(t_command));
	if (!new_node->command)
		util_put_cmd_err_and_exit("malloc");
	new_node->type = NODE_COMMAND;
	new_node->left = NULL;
	new_node->right = NULL;
	new_node->command->args = NULL;
	new_node->command->redirects = NULL;
	new_node->command->next = NULL;
	return (new_node);
}
/*
typedef struct			s_redirect
{
	int					fd_io;
	int					fd_file;
	int					fd_backup;
	t_redirect_type		type;
	t_token_list		*filename;
	struct s_redirect	*next;
	struct s_redirect	*prev;
}						t_redirect;
*/

t_redirect	*create_and_init_redirect(void)
{
	t_redirect	*ret;

	ret = (t_redirect *) malloc(sizeof(t_redirect));
	if (!ret)
		util_put_cmd_err_and_exit("malloc");
	ret->fd_io = NO_PID;
	ret->fd_file = NO_PID;
	ret->fd_backup = NO_PID;
	//ret->typeの初期化は不要？
	ret->filename = NULL;
	ret->next = NULL;
	ret->prev = NULL;
	return (ret);
}

bool	parse_redirect_process(t_node *node, t_token_list **token)
{
	t_redirect	*rediret;

	rediret = create_and_init_redirect();
}

t_token_list	*duplicate_token(t_token_list *token)
{
	t_token_list	*dup_token;
	size_t			comp_len;

	comp_len = ft_strlen(token->comp);
	dup_token = init_token(NULL, comp_len);
	ft_strlcpy(dup_token->comp, token->comp, comp_len + 1);
	dup_token->type = token->type;
	return (dup_token);
}

void	add_token_into_cmd_args(t_token_list **cmd_args, t_token_list **token)
{
	//cmd_argsにtokenの一つ目をまるまるコピーしたい
	//->するとtokenの一つ目のコピーを作ってあげて代入する必要がある
	t_token_list	*dup_token;
	//元のcmd_argsのアドレスを動かしたくないから別で最後尾のアドレスを持つ変数を宣言
	t_token_list	*cmd_args_tail;

	dup_token = duplicate_token(*token);
	if (!*cmd_args)
		*cmd_args = dup_token;
	//先頭じゃなかった場合は
	else
	{
		cmd_args_tail = *cmd_args;
		while (cmd_args_tail->next)
		{
			cmd_args_tail = cmd_args_tail->next;
		}
		cmd_args_tail->next = dup_token;
	}
}

void	input_cmd_args(t_command *command, t_token_list **token)
{
	while (*token && (*token)->type == TOKEN)
	{
		add_token_into_cmd_args(&command->args, token);
		*token = (*token)->next;
	}
}

void	print_this(t_token_list *token)
{
	t_token_list	*tmp_token = token;

	while (tmp_token)
	{
		printf("comp: [%s]\n", tmp_token->comp);
		tmp_token = tmp_token->next;
	}
}

void	print_parser(t_command *cmd)
{
	t_command	*tmp_cmd;

	tmp_cmd = cmd;
	while (tmp_cmd)
	{
		print_this(tmp_cmd->args);
		tmp_cmd = tmp_cmd->next;
	}
}

//cmdを入れてく関数
bool	parse_command(t_command *last_cmd, t_node **node, t_token_list **token)
{
	if (!*node)
		return (false);
	*node = create_and_init_node();
	if (last_cmd)
		last_cmd->next = (*node)->command;
	else // last_cmd == NULL
		last_cmd = (*node)->command;
	//前回のコマンドを記録
	while (*token)
	{
		if ((*token)->type == TOKEN)
		{
			//nodeのargsにコマンドを入れてく処理
			input_cmd_args((*node)->command, token);
		}
		else if ((*token)->type == CHAR_LESS || (*token)->type == CHAR_GREATER \
				|| (*token)->type == D_GREATER || (*token)->type == IO_NUMBER)
		{
			//redirectの処理
			parse_redirect_process();
		}
		else
			break ;
	}
	/*この処理の起こるときがあまり理解できてない
	if (!(*node)->command->args && !(*node)->command->redirects)
	{
		del_node_list(node);
		return (FALSE);
	}
	*/
	print_parser((*node)->command);
	return (true);
}

t_node	*add_parent_node(t_node *left, t_node *right)
{
	t_node	*new_parent_node;

	new_parent_node = (t_node *) malloc(sizeof(t_node));
	if (!new_parent_node)
		util_put_cmd_err_and_exit("malloc");
	new_parent_node->type = NODE_PIPE;
	new_parent_node->command = NULL;
	new_parent_node->right = right;
	new_parent_node->left = left;
	return (new_parent_node);
}

//前回のコマンドを保持する構造体に何をいれるのかわからない状況
bool	parser(t_node **parent_node, t_token_list **token)
{
	t_node		*child;
	t_command	last_cmd;

	if (*token)
	{
		//親ノード(左側)に入れてく
		if (parse_command(&last_cmd, parent_node, token) == false)
			return (false);
	}
	while (*token)
	{
		if ((*token)->type == CHAR_PIPE)
		{
			(*token) = (*token)->next;
			if (!*token)
				return (false);
			//右側に入れてく
			if (parse_command(&last_cmd, &child, token) == false)
				return (false);
			//親ノードに移動する
			*parent_node = add_parent_node(*parent_node, child);
		}
		else
			break ;
	}
	return (true);
}

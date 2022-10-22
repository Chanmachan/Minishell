/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nfukuma <nfukuma@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/17 01:19:07 by nfukuma           #+#    #+#             */
/*   Updated: 2022/10/23 01:10:03 by nfukuma          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "libft.h"
# include <readline/history.h>  // for add_history()
# include <readline/readline.h> // for readline()
# include <stdbool.h>           // for type bool
# include <stdio.h>             // for printf() perror() strerror()
# include <stdlib.h>            // for free()
# include <unistd.h>            // for write(), access(), execve()
# include "lexer.h"
# include <errno.h> // for errno

# define RED_COLOR "\e[31m"
# define WHITE_COLOR "\e[m"

/* environ */
typedef struct s_env
{
	char			*name;
	char			*value;
	bool			is_env;
	struct s_env	*next;
}					t_env;

/* global */
typedef struct s_shell
{
	t_env			*envs;
	int				status;
	// char	*pwd;
	bool			interactive;
	bool			interrupted;
	bool			exited;
}					t_shell;

void				test_util(void);

// in init/init_shell.c
void				init_minishell(void);

// in utils/util.c
bool				util_is_builtin(const char *arg);

// in utils/util_put_err_exit.c
void				util_put_err_exit(char *err_msg);

// in utils/util_list.c
t_env				*util_list_new_envnode(char *env_str);
t_env				*util_list_get_last_envnode(t_env *node_ptr);
void				util_list_add_last_new_envnode(t_env **envs,
						t_env *new_env);

// in exec_builtin.c
int					exec_builtin(char **args);

// in util/util_env_get.c
t_env	*util_env_get(const	char *name);

// in exec_exit.c
int					exec_exit(char **args);

#endif

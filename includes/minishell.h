/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nfukuma <nfukuma@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/17 01:19:07 by nfukuma           #+#    #+#             */
/*   Updated: 2022/10/23 15:04:36 by nfukuma          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>             // for printf() perror() strerror()
# include <errno.h>             // for errno
# include <readline/history.h>  // for add_history()
# include <readline/readline.h> // for readline()
# include <stdbool.h>           // for type bool
# include <stdlib.h>            // for free()
# include <unistd.h>            // for write(), access(), execve()
# include "libft.h"
# include "utils.h"
# include "init.h"
# include "lexer.h"
# include "builtin.h"

# define RED_COLOR "\e[31m"
# define WHITE_COLOR "\e[m"

/*
in utils.h define
environ node struct
typedef struct s_env
{
	char			*name;
	char			*value;
	bool			is_env;
	struct s_env	*next;
}					t_env;
*/

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

#endif

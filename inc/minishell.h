/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odana <odana@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/27 21:31:05 by yitani            #+#    #+#             */
/*   Updated: 2025/07/13 18:15:09 by odana            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

// libraries
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <fcntl.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <dirent.h>
# include <sys/wait.h>
# include <signal.h>
# include <string.h>
# include <errno.h>
# include <termios.h>
# include <termcap.h>
# include <sys/ioctl.h>
# include <readline/readline.h>
# include <readline/history.h>
# include "../minishell_libft/libft.h"

// global variable
extern volatile sig_atomic_t	g_signal_received;

// macros
# define EXIT_SUCCESS		0
# define EXIT_GENERAL_ERROR	1
# define EXIT_MISUSE		2
# define EXIT_CANNOT_EXEC	126
# define EXIT_NOT_FOUND		127
# define EXIT_CTRL_C		130
# define EXIT_CTRL_BACK		131
# define ERROR				-1
# define NO_CHANGE			0
# define SPLIT_SUCCESS		1
# define TOKEN_REMOVED

// forward strcut declarations
typedef struct s_token		t_token;
typedef struct s_node		t_node;
typedef struct s_cmd		t_cmd;
typedef struct s_arg		t_arg;
typedef struct s_redir		t_redir;
typedef struct s_env		t_env;
typedef struct s_exec		t_exec;
typedef struct s_shell		t_shell;

// enums

typedef enum e_token_type
{
	TOKEN_WORD,
	TOKEN_PIPE,
	TOKEN_REDIR_IN,
	TOKEN_REDIR_OUT,
	TOKEN_REDIR_OUT_APPEND,
	TOKEN_HERDOC,
	TOKEN_OR,
	TOKEN_AND,
	TOKEN_WILDCARD,
	TOKEN_LPAREN,
	TOKEN_RPAREN,
}	t_token_type;

typedef enum e_node_type
{
	NODE_CMD,
	NODE_PIPE,
	NODE_AND,
	NODE_OR,
}	t_node_type;

typedef enum e_redir_type
{
	REDIR_IN,
	REDIR_OUT,
	REDIR_OUT_APPEND,
	HERE_DOC
}	t_redir_type;

typedef enum e_builtin
{
	BUILTIN_NONE,
	BUILTIN_CD,
	BUILTIN_ECHO,
	BUILTIN_ENV,
	BUILTIN_EXIT,
	BUILTIN_EXPORT,
	BUILTIN_PWD,
	BUILTIN_UNSET
}	t_builtin;

typedef enum e_phase
{
	PHASE_NONE,
	PHASE_TOKEN,
	PHASE_EXPAND,
	PHASE_PARSE,
	PHASE_EXECUTE,
}	t_phase;

// minishell modules
# include "tokenizer.h"
# include "parser.h"
# include "expansion.h"
# include "env.h"
# include "execution.h"
# include "shell.h"

#endif
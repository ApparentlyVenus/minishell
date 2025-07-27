/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   structs.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yitani <yitani@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/24 21:55:16 by odana             #+#    #+#             */
/*   Updated: 2025/07/28 00:26:16 by yitani           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STRUCTS_H
# define STRUCTS_H

# include <sys/types.h>

// enums
typedef enum e_token_type
{
	TOKEN_WORD,
	TOKEN_PIPE,
	TOKEN_REDIR_IN,
	TOKEN_REDIR_OUT,
	TOKEN_REDIR_OUT_APPEND,
	TOKEN_HEREDOC,
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
	PHASE_PARSE,
	PHASE_EXECUTE,
}	t_phase;

typedef struct s_token	t_token;
typedef struct s_node	t_node;
typedef struct s_cmd	t_cmd;
typedef struct s_arg	t_arg;
typedef struct s_redir	t_redir;
typedef struct s_env	t_env;
typedef struct s_exec	t_exec;
typedef struct s_shell	t_shell;

// struct definitions

typedef struct s_heredoc_data
{
	char	*delimiter;
	t_env	*env;
	int		s_quotes;
	int		d_quotes;
	int		*pipe_fd;
}	t_heredoc_data;

typedef struct s_env
{
	char			*key;
	char			*value;
	int				equal;
	struct s_env	*next;
}	t_env;

typedef struct s_token
{
	t_token_type	type;
	char			*value;
	int				single_quotes;
	int				double_quotes;
	int				priority;
	int				has_wildcard;
	struct s_token	*next;
}	t_token;

typedef struct s_redir
{
	t_redir_type	type;
	char			*filename;
	int				expand_heredoc;
	struct s_redir	*next;
}	t_redir;

typedef struct s_arg
{
	char			*value;
	int				single_quotes;
	int				double_quotes;
	struct s_arg	*next;
}	t_arg;

typedef struct s_cmd
{
	t_arg	**args;
	t_redir	*redirs;
}	t_cmd;

typedef struct s_node
{
	t_node_type		type;
	t_cmd			*cmd;
	struct s_node	*left;
	struct s_node	*right;
}	t_node;

typedef struct s_op_info
{
	t_token_type	type;
	int				priority;
}	t_op_info;

typedef struct s_exec
{
	int		cmd_count;
	int		**pipes;
	pid_t	*pids;
	t_env	**env;
}	t_exec;

typedef struct s_shell
{
	t_env		*env;
	int			exit_code;
	int			interactive;
	t_token		*tokens;
	t_node		*ast;
	t_exec		*exec_ctx;
	char		*last_error;
	t_phase		current_phase;
}	t_shell;

#endif
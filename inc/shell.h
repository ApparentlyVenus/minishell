/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odana <odana@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/11 00:24:38 by odana             #+#    #+#             */
/*   Updated: 2025/07/11 08:58:11 by odana            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SHELL_H
# define SHELL_H

# include "minishell.h"

typedef enum e_phase
{
	PHASE_NONE,
	PHASE_TOKEN,
	PHASE_EXPAND,
	PHASE_PARSE,
	PHASE_EXECUTE,
}	t_phase;

typedef struct s_shell
{
    t_env  		*env;
	int    		exit_code;
    int    		interactive;
    t_token		*tokens;
    t_node 		*ast;
    t_exec		*exec_ctx;
    t_token		*expanded_tokens;
	char		*last_error;
    t_phase		current_phase;
}	t_shell;

// exit_codes

# define EXIT_SUCCESS		0
# define EXIT_GENERAL_ERROR	1
# define EXIT_MISUSE		2
# define EXIT_CANNOT_EXEC	126
# define EXIT_NOT_FOUND		127
# define EXIT_CTRL_C		130
# define EXIT_CTRL_BACK		131

// shell functions

t_shell     *shell_init(char **envp);
void        shell_cleanup(t_shell *shell);
void        shell_exit(t_shell *shell, int exit_code);
void        shell_reset_phase(t_shell *shell);

// cleanup functions

void        cleanup_tokens(t_shell *shell);
void        cleanup_expanded_tokens(t_shell *shell);
void        cleanup_ast(t_shell *shell);
void        cleanup_exec(t_shell *shell);

// error handeling

int			handle_error(t_shell *shell, char *msg, int exit_code);
int			handle_fatal_error(t_shell *shell, char *msg, int exit_code);

// debugging / utils

int			is_valid_shell(t_shell *shell);
void		print_shell_state(t_shell *shell);


#endif
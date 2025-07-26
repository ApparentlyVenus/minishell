/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odana <odana@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/27 21:31:05 by yitani            #+#    #+#             */
/*   Updated: 2025/07/26 12:47:33 by odana            ###   ########.fr       */
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
# include "../libft/libft.h"

// minishell modules

# include "structs.h"
# include "tokenizer.h"
# include "parser.h"
# include "expansion.h"
# include "env.h"
# include "execution.h"
# include "shell.h"

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
# define TOKEN_REMOVED		2

#endif
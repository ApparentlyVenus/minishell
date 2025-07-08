/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yitani <yitani@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/27 21:31:05 by yitani            #+#    #+#             */
/*   Updated: 2025/07/08 11:18:57 by yitani           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

/* C libraries */

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

/* minishell utils / helper functions */

# include "tokenizer.h"
# include "parser.h"
# include "expansion.h"
# include "env.h"
# include "execution.h"

# include "../minishell_libft/libft.h"	// libft functions

/* minishell modules */

typedef struct s_token	t_token;
typedef struct s_exec	t_exec;
typedef struct s_env	t_env;

#endif
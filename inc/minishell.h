/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yitani <yitani@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/27 21:31:05 by yitani            #+#    #+#             */
/*   Updated: 2025/06/27 22:11:53 by yitani           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

/* Includes */

// -------------------------
// 💬 Standard I/O & Errors
// -------------------------
# include <stdio.h>      // printf, perror
# include <stdlib.h>     // malloc, free, exit, getenv
# include <unistd.h>     // read, write, access, fork, execve, pipe, dup, dup2, close, chdir, getcwd, unlink, isatty, ttyname, ttyslot

// -------------------------
// 📁 Files & Directories
// -------------------------
# include <fcntl.h>      // open
# include <sys/types.h>  // pid_t, size_t, opendir/readdir
# include <sys/stat.h>   // stat, lstat, fstat
# include <dirent.h>     // opendir, readdir, closedir

// -------------------------
// 👶 Process & Signals
// -------------------------
# include <sys/wait.h>   // wait, waitpid, wait3, wait4
# include <signal.h>     // signal, sigaction, sigemptyset, sigaddset, kill

// -------------------------
// 🧠 Errors
// -------------------------
# include <string.h>     // strerror
# include <errno.h>      // errno (used with strerror/perror)

// -------------------------
// 🖥️ Terminal handling
// -------------------------
# include <termios.h>    // tcgetattr, tcsetattr (terminal attributes)
# include <termcap.h>    // tgetent, tgetflag, tgetnum, tgetstr, tgoto, tputs (terminal capabilities)
# include <sys/ioctl.h>  // ioctl (terminal size, etc.)

// -------------------------
// 📜 Readline (must link with -lreadline)
// -------------------------
# include <readline/readline.h>   // readline, rl_on_new_line, rl_replace_line, rl_redisplay
# include <readline/history.h>    // add_history, rl_clear_history

/* Parsing */
/* ... */
#endif
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odana <odana@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/29 00:52:44 by yitani            #+#    #+#             */
/*   Updated: 2025/07/27 13:27:23 by odana            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ENV_H
# define ENV_H

# include "minishell.h"

// Parsing environment

void	env_init(t_env **env_list, char **envp);
int		valid_entries_count(t_env *env);
char	**convert_env_to_array(t_env *env);
char	**cleanup_envp(char **envp, int count);

// Builtin functions

int		builtin_echo(char **args);
int		builtin_export(char **args, t_env **env);
int		builtin_pwd(void);
int		builtin_unset(char **args, t_env **env);
int		builtin_cd(char **args, t_env **env);
void	builtin_env(t_env **env);
int		builtin_exit(char **args, t_shell *shell);

// Helpers

void	handle_new_key(t_env **env, char *key, char *value);
void	set_env_value(t_env **env, char *key, char *value);
void	export_helper(t_env **envp, char *equal_sign, char *args, int *failure);
void	swap_env_nodes(t_env *a, t_env *b);
t_env	**do_something(t_env **envp);
char	*get_env_value(t_env *env, char *key);
int		is_valid_key(const char *key);
void	env_add_back(t_env **env_list, t_env *new_node);
int		size_of_arr(char **args);

#endif
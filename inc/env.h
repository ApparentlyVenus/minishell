/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yitani <yitani@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/29 00:52:44 by yitani            #+#    #+#             */
/*   Updated: 2025/08/02 20:11:40 by yitani           ###   ########.fr       */
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
int		builtin_pwd(t_env **env);
int		builtin_unset(char **args, t_env **env);
int		builtin_cd(char **args, t_env **env);
int		builtin_env(t_env **env, char **args);
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
void	handle_fail_old_pwd(char *old_pwd, char *pwd_env);
void	handle_fail_new_pwd(char *old_pwd, char *pwd_env);
int		change_directory(char **args, t_env **env, int *print);
void	update_pwd(t_env **env, char *old_pwd, char *new_pwd);
void	concat_args(t_cmd *cmd);

// Environment copying and manipulation

t_env	*env_copy_or_clean(char **args, t_env *original);
int		env_apply_changes(t_env **env, char **args, int start_idx, int cmd_idx);
int		env_execute_command(char **cmd_args, t_env *env);

#endif
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yitani <yitani@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/29 00:52:44 by yitani            #+#    #+#             */
/*   Updated: 2025/07/08 01:52:46 by yitani           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ENV_H

# define ENV_H

typedef struct s_env
{
	char			*key;
	char			*value;
	int				equal;
	struct s_env	*next;
}	t_env;

// Parsing environment

void	env_init(t_env **env_list, char **envp);
char	**convert_env_to_array(t_env *env);

// Builtin functions

int		builtin_echo(char **args, t_exec *shell);
void	builtin_env(t_exec *shell);
void	builtin_exit(char **args, t_exec *shell);
void	builtin_export(t_exec *shell, char **args);
void	builtin_pwd(t_exec *shell);
void	builtin_unset(t_exec *shell, char **arg);
void	builtin_cd(t_exec *shell, char **args);

// Utils 

char	*expand_token_value_final(char *value, t_exec *shell);
t_token	*expand_dollar(t_token *tokens, t_exec *shell); // for expansion $
// for expansion *
// for expansion all toguether;
void	set_env_value(t_env **env, char *key, char *value);
void	export_helper(t_env **envp, char *equal_sign, char *args);
int		is_valid_key(const char *key);
t_env	**do_something(t_env **envp);
char	*get_env_value(t_env *env, char *key);

#endif
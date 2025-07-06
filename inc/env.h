/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yitani <yitani@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/29 00:52:44 by yitani            #+#    #+#             */
/*   Updated: 2025/07/06 09:27:17 by yitani           ###   ########.fr       */
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

// typedef struct s_shell
// {
// 	t_env	**env;
// 	int		exit_code;
// }	t_shell;

// Parsing environment

void	env_init(t_env **env_list, char **envp);
char	**convert_env_to_array(t_env *env);

// Builtin functions

int		builtin_echo(t_token *tokens, t_exec *shell);
void	builtin_env(t_exec *shell);
void	builtin_exit(char **args, t_exec *shell);
void	builtin_export(t_exec *shell, t_token *args);
void	builtin_pwd(t_exec *shell);
void	builtin_unset(t_exec *shell, t_token *arg);
void	builtin_cd();

// Utils 

t_token	*echo_util(t_token *tokens, t_exec *shell);
void	set_env_value(t_env **env, char *key, char *value);
void	export_helper(t_env **envp, char *equal_sign, t_token *args);
int		is_valid_key(const char *key);
t_env	**do_something(t_env **envp);
char	*get_env_value(t_env *env, char *key);

#endif
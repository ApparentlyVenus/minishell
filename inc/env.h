/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yitani <yitani@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/29 00:52:44 by yitani            #+#    #+#             */
/*   Updated: 2025/07/05 17:21:02 by yitani           ###   ########.fr       */
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

typedef struct s_shell
{
	t_env	*env;
	int		exit_code;
}	t_shell;


void	env_init(t_env **env_list, char **envp);
char	*get_env_value(t_env *env, char *key);
void	handle_new_key(t_env **env, char *key, char *value);
void	set_env_value(t_env **env, char *key, char *value);
void	unset_env_value(t_env **env, char *key);
int		valid_entries_count(t_env *env);
char	**convert_env_to_array(t_env *env);

// expand $VAR into its value 👍🏼

char	*expand_token_value_1(char *value, t_env *env, int *i);
char	*expand_token_value_1_5(char *dest, const char *to_append);
char	*handle_dollar_expansion(char *result, char *value, t_env *env, int *i);
char	*expand_token_value_final(char *value, t_env *env);
t_token	*expand_variables_in_tokens(t_token *tokens, t_env *env);

#endif
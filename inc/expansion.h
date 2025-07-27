/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odana <odana@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/16 17:08:36 by yitani            #+#    #+#             */
/*   Updated: 2025/07/27 17:31:12 by odana            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odana <odana@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/16 17:08:36 by yitani            #+#    #+#             */
/*   Updated: 2025/07/18 13:21:58 by odana            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXPANSION_H
# define EXPANSION_H

# include "minishell.h"

// Main expansion entry point
void	expand_cmd(t_cmd *cmd, t_builtin type, t_shell *shell);

// Quote REmoval

char	*remove_all_quotes(char *str);
char	*remove_quotes(char *str);

// Assignment and argument expansion

char	*expand_assignment_value(char *arg, t_env *env);
char	*expand_cmd_arg(char *arg, t_shell *shell, t_builtin builtin_type,
			int index);
void	expand_cmd_args(t_cmd *cmd, t_shell *shell, t_builtin builtin_type);
void	expand_cmd_redirs(t_cmd *cmd, t_env *env);

// Exit code expansion

char	*expand_exit(char *str, t_env *env, int exit_code);
char	*replace_exit_code(char *str, char *exit_code_str);
char	*replace_substring(char *str, char *pos, char *old_str, char *new_str);

// Assginemtn Utils

int		is_assignment(char *arg);
char	*join_assignment(char *var, char *val);
char	*remove_quotes(char *filename);
char	*remove_adjacent_quotes(char *filename);

// Variable expansion in arguments

void	expand_variables_in_args(char **args, t_env *env);
void	word_split_args(char ***args);
int		replace_args_with_matches(char ***args, char **matches, int i);
void	expand_wildcards_in_args(char ***args);
void	expand_all_args(char ***args, t_env *env);

// Variable expansion

char	*expand_variables(char *str, t_env *env);
char	*expand_single_var(char *str, int *pos, t_env *env);
int		is_var_char(char c);
char	*char_to_string(char c);
char	*append_to_result(char *result, char *to_append);

// Wildcard expansion

int		wildcard_expand(char *pattern, char ***matches);
void	sort_matches(char **matches, int count);
int		match_star_pattern(const char *pattern, const char *filename);
int		count_star_matches(const char *pattern);

// Wildcard collection

void	cleanup_matches_on_error(char **matches, int count);
int		should_skip_entry(struct dirent *entry, const char *pattern);
int		add_match_to_array(char **matches, int count, char *filename);
int		fill_star_matches(const char *pattern,
			char **matches, int expected_count);
int		collect_star_matches(const char *pattern, char ***matches);

// Word splitting

int		contains_whitespace(char *str);
void	free_split(char **args);
char	**create_new_args_array(char ***args, char **split, int pos);
int		replace_args_with_split(char ***args, char **split, int i);
int		count_args(char **args);
int		copy_args_before(char **new_args, char **old_args, int pos);
int		copy_split_args(char **new_args, char **split, int j);
int		copy_args_after(char **new_args, char **old_args, int pos, int j);

#endif

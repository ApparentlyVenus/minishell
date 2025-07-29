/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odana <odana@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/16 17:08:36 by yitani            #+#    #+#             */
/*   Updated: 2025/07/29 03:50:42 by odana            ###   ########.fr       */
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

// Main expansion functions
void	expand_cmd_args(t_cmd *cmd, t_shell *shell, t_builtin builtin_type);
void	expand_cmd_redirs(t_cmd *cmd, t_env *env);

// Phase functions
void	expand_variables_phase(t_cmd *cmd,
			t_shell *shell, t_builtin builtin_type);
void	expand_splitting_phase(t_arg ***args);

// Command argument expansion helpers
char	*expand_assignment_value(char *arg, t_env *env);
char	*expand_cmd_arg(char *arg,
			t_shell *shell, t_builtin builtin_type, int index);

// Quote removal
char	*remove_all_quotes(char *str);
char	*remove_quotes(char *str);

// Exit code expansion
char	*expand_exit(char *str, t_env *env, int exit_code);
char	*replace_exit_code(char *str, char *exit_code_str);
char	*replace_substring(char *str, char *pos, char *old_str, char *new_str);

// Variable expansion
char	*expand_variables(char *str, t_env *env);
char	*expand_single_var(char *str, int *pos, t_env *env);
int		is_var_char(char c);
char	*char_to_string(char c);
char	*append_to_result(char *result, char *to_append);

// Word splitting and wildcard expansion wrapper
int		perform_word_split(t_arg ***args, int index);
int		perform_wildcard_expand(t_arg ***args, int index);

// Wildcard expansion
int		expand_wildcard(char *pattern, char ***matches);
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

// t_arg array manipulation helpers
int		count_targs(t_arg **args);
int		copy_targs_before(t_arg **new_args, t_arg **old_args, int pos);
int		copy_split_targs(t_arg **new_args, char **split, int j);
int		copy_targs_after(t_arg **new_args, t_arg **old_args, int pos, int j);
int		copy_match_targs(t_arg **new_args,
			char **matches, int start_index, int match_count);

// Utility functions
int		is_assignment(char *arg);
char	*join_assignment(char *var, char *val);
int		contains_whitespace(char *str);
int		has_wildcard(char *word);
void	free_split(char **args);
int		count_args(char **args);

#endif
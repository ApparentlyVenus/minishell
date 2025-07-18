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

// Expands a t_cmd in-place for JIT expansion during execution
void	expand_cmd(t_cmd *cmd, t_env *env, t_builtin builtin_type);

// Wildcard/globbing helpers
int		count_star_matches(char *pattern);
int		fill_star_matches(char *pattern, char **matches, int expected_count);
int		collect_star_matches(char *pattern, char ***matches);
void	sort_star_matches(char **matches, int count);
int		wildcard_expand(char *pattern, char ***matches);
void	sort_matches(char **matches, int count);
int		contains_whitespace(char *str);
void	free_split(char **args);

// Pattern Matching
int		match_star_pattern(char *pattern, char *filename);
int		handle_star_case(char **p, char *f, char **star_p, char **star_f);
int		try_backtrack(char **p, char *f);

// Variable expansion

int		is_assignment(char *arg);
char	*join_assignment(char *var, char *val);
char	*expand_assignment_value(char *arg, t_env *env);
char	*expand_cmd_arg(char *arg, t_env *env, t_builtin builtin_type,
			int index);
void	expand_cmd_args(t_cmd *cmd, t_env *env, t_builtin builtin_type);
void	expand_cmd(t_cmd *cmd, t_env *env, t_builtin builtin_type);

#endif
#ifndef NEW_EXPANSION_H
# define NEW_EXPANSION_H

/*
** new_expansion.h
** Purpose: Header for new, JIT expansion logic that operates on parsed AST structures (t_cmd, t_node, t_redir, etc.)
** Only include new expansion functions for use during execution, not token-based expansion.
*/

# include "minishell.h"

// Expands a t_cmd in-place for JIT expansion during execution
void expand_cmd(t_cmd *cmd, t_env *env, t_builtin builtin_type);

// Wildcard/globbing helpers
int count_star_matches(const char *pattern); // Counts files matching *
int fill_star_matches(const char *pattern, char **matches, int expected_count); // Fills array with matches
int collect_star_matches(const char *pattern, char ***matches); // Collects all matches
void sort_star_matches(char **matches, int count); // Sorts matches alphabetically
int match_star_pattern(const char *pattern, const char *filename); // Checks if filename matches pattern

// Expands a wildcard pattern to a sorted array of matches
int wildcard_expand(char *pattern, char ***matches);
// Sorts an array of strings alphabetically
void sort_matches(char **matches, int count);

// Word splitting helpers
int contains_whitespace(char *str); // Checks for whitespace in a string
void free_split(char **args); // Frees a NULL-terminated array of strings

#endif 
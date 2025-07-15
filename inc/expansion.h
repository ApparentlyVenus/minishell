#ifndef NEW_EXPANSION_H
# define NEW_EXPANSION_H

# include "minishell.h"

// Expands a t_cmd in-place for JIT expansion during execution
void expand_cmd(t_cmd *cmd, t_env *env, t_builtin builtin_type);

// Wildcard/globbing helpers
int count_star_matches(char *pattern);
int fill_star_matches(char *pattern, char **matches, int expected_count);
int collect_star_matches(char *pattern, char ***matches);
void sort_star_matches(char **matches, int count);
int wildcard_expand(char *pattern, char ***matches);
void sort_matches(char **matches, int count);
int contains_whitespace(char *str);
void free_split(char **args);

// Pattern Matching
int match_star_pattern(char *pattern, char *filename);
int handle_star_case(char **p, char *f, char **star_p, char **star_f);
int try_backtrack(char **p, char *f);

// 


#endif 
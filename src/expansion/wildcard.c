#include "../../inc/minishell.h"
#include <dirent.h>
#include <string.h>
#include <stdlib.h>

// Simple pattern matcher for *, ?, [abc] (no nested [] or escapes)
static int match_pattern(const char *pattern, const char *str) {
    while (*pattern && *str) {
        if (*pattern == '*') {
            pattern++;
            if (!*pattern)
                return 1;
            while (*str) {
                if (match_pattern(pattern, str))
                    return 1;
                str++;
            }
            return 0;
        } else if (*pattern == '?') {
            if (!*str)
                return 0;
            pattern++;
            str++;
        } else if (*pattern == '[') {
            pattern++;
            int match = 0;
            while (*pattern && *pattern != ']') {
                if (*pattern == *str)
                    match = 1;
                pattern++;
            }
            if (!match || *pattern != ']')
                return 0;
            pattern++; str++;
        } else {
            if (*pattern != *str)
                return 0;
            pattern++; str++;
        }
    }
    // Skip trailing * in pattern
    while (*pattern == '*') pattern++;
    return !*pattern && !*str;
}

t_token *expand_wildcard(t_token *current) {
    if (!current || current->single_quotes || current->double_quotes)
        return current;
    if (!has_wildcard(current->value))
        return current;
    DIR *dir = opendir(".");
    if (!dir)
        return current;
    struct dirent *entry;
    t_token *first = NULL, *last = NULL;
    int found = 0;
    while ((entry = readdir(dir))) {
        if (entry->d_name[0] == '.' && current->value[0] != '.')
            continue; // skip hidden unless pattern starts with .
        if (match_pattern(current->value, entry->d_name)) {
            t_token *new_token = malloc(sizeof(t_token));
            if (!new_token) continue;
            new_token->value = ft_strdup(entry->d_name);
            new_token->type = TOKEN_WORD;
            new_token->priority = 0;
            new_token->single_quotes = 0;
            new_token->double_quotes = 0;
            new_token->has_wildcard = 0;
            new_token->next = NULL;
            if (!first) first = new_token;
            if (last) last->next = new_token;
            last = new_token;
            found = 1;
        }
    }
    closedir(dir);
    if (found) {
        // Replace current token with first match, append others after
        free(current->value);
        current->value = ft_strdup(first->value);
        current->has_wildcard = 0;
        t_token *cur = first->next;
        t_token *insert_after = current;
        while (cur) {
            t_token *next = cur->next;
            insert_after->next = cur;
            insert_after = cur;
            cur = next;
        }
        insert_after->next = current->next;
        current->next = first->next;
    }
    return current;
} 
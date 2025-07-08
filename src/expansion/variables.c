#include "../../inc/minishell.h"

char *extract_var_name(char *value, int *i)
{
	int start = *i;
	int len;
	char *var_name;
	while (value[*i] && is_valid_var_char(value[*i]))
		(*i)++;
	len = *i - start;
	var_name = (char *)malloc(sizeof(char) * (len + 1));
	if (!var_name)
		return (NULL);
	ft_strncpy(var_name, value + start, len);
	var_name[len] = '\0';
	return (var_name);
}

char *append_str(char *str, char c)
{
	char *new_str;
	int len;
	int i;
	if (!str)
		return (NULL);
	len = ft_strlen(str);
	new_str = malloc(len + 2);
	if (!new_str)
	{
		free(str);
		return (NULL);
	}
	i = 0;
	while (i < len)
	{
		new_str[i] = str[i];
		i++;
	}
	new_str[i] = c;
	new_str[i + 1] = '\0';
	free(str);
	return (new_str);
}

static char *join_and_free_first(char *s1, char *s2)
{
	char *result;
	if (!s1 || !s2)
		return (s1);
	result = ft_strjoin(s1, s2);
	free(s1);
	return (result);
}

char *expand_variables(char *value, t_env *env)
{
	char *new_str;
	char *var_name;
	char *var_value;
	int i;
	if (!contains_dollar(value))
		return (ft_strdup(value));
	new_str = ft_strdup("");
	if (!new_str)
		return (NULL);
	i = 0;
	while (value[i])
	{
		if (value[i] == '$' && is_valid_var_start(value[i + 1]))
		{
			i++;
			var_name = extract_var_name(value, &i);
			if (!var_name)
				return (free(new_str), NULL);
			var_value = get_env_value(env, var_name);
			new_str = join_and_free_first(new_str, var_value ? var_value : "");
			free(var_name);
		}
		else
			new_str = append_char_to_str(new_str, value[i++]);
		if (!new_str)
			return (NULL);
	}
	return (new_str);
} 
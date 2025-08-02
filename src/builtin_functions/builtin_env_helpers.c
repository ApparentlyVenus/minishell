/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_env_helpers.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yitani <yitani@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/02 20:10:27 by yitani            #+#    #+#             */
/*   Updated: 2025/08/02 20:22:12 by yitani           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

t_env	*env_copy_or_clean(char **args, t_env *original)
{
	t_env	*new_env;
	t_env	*current;
	t_env	*new_node;

	if (args[1] && ft_strcmp(args[1], "-i") == 0)
		return (NULL);
	new_env = NULL;
	current = original;
	while (current)
	{
		new_node = malloc(sizeof(t_env));
		if (!new_node)
			return (free_env(new_env), NULL);
		new_node->key = ft_strdup(current->key);
		if (current->value)
			new_node->value = ft_strdup(current->value);
		else
			new_node->value = NULL;
		new_node->equal = current->equal;
		new_node->next = NULL;
		env_add_back(&new_env, new_node);
		current = current->next;
	}
	return (new_env);
}

static int	env_parse_assignment(char *arg, char **key, char **value)
{
	char	*eq_pos;

	eq_pos = ft_strchr(arg, '=');
	if (!eq_pos)
		return (0);
	*key = ft_substr(arg, 0, eq_pos - arg);
	*value = ft_strdup(eq_pos + 1);
	if (!*key || !*value)
	{
		free(*key);
		free(*value);
		return (0);
	}
	return (1);
}

static t_env	*env_create_first_node(char *key, char *value)
{
	t_env	*new_node;

	new_node = malloc(sizeof(t_env));
	if (!new_node)
		return (NULL);
	new_node->key = key;
	new_node->value = value;
	new_node->equal = 1;
	new_node->next = NULL;
	return (new_node);
}

static int	env_handle_assignment(t_env **env, char *key, char *value)
{
	if (!is_valid_key(key))
	{
		printf("env: '%s': invalid variable name\n", key);
		return (free(key), free(value), EXIT_GENERAL_ERROR);
	}
	if (!*env)
		*env = env_create_first_node(key, value);
	else
	{
		set_env_value(env, key, value);
		free(key);
		free(value);
	}
	return (EXIT_SUCCESS);
}

int	env_apply_changes(t_env **env, char **args, int start_idx, int cmd_idx)
{
	char	*key;
	char	*value;
	int		i;
	int		result;

	i = start_idx;
	while (i < cmd_idx)
	{
		if (env_parse_assignment(args[i], &key, &value))
		{
			result = env_handle_assignment(env, key, value);
			if (result != EXIT_SUCCESS)
				return (result);
		}
		else
		{
			printf("env: '%s': No such file or directory\n", args[i]);
			return (EXIT_GENERAL_ERROR);
		}
		i++;
	}
	return (EXIT_SUCCESS);
}

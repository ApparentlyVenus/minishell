/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yitani <yitani@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/06 04:22:23 by yitani            #+#    #+#             */
/*   Updated: 2025/07/08 20:55:23 by yitani           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

char	*expand_floaty(char *value, t_exec *shell)
{
	char	*result;
	char	*home;
	char	*suffix;
	
	if (value[0] == '~' && (value[1] == '/' || value[1] == '\0'))
	{
		home = get_env_value(*(shell->env), "HOME");
		suffix = ft_strdup(value + 1);
		result = ft_strjoin(home, suffix);
		free(suffix);
		return (result);
	}
	return (NULL);
}

void	change_directory(t_exec *shell, char **args, int *status)
{
	if (!args || !args[0])
		status[0] = chdir(get_env_value(*(shell->env), "HOME"));
	else if (args[0][0] == '-' && args[0][1] == '\0')
	{
		status[0] = chdir(get_env_value(*(shell->env), "OLDPWD"));
		status[1] = 1;
	}
	else
		status[0] = chdir(args[0]);
}

void	builtin_cd(t_exec *shell, char **args)
{
	int	status[2];
	char	*env_key[2];
	char	*new_value[2];

	status[0] = 0;
	status[1] = 0;
	new_value[0] = getcwd(NULL, 0);
	if (size_of_arr(args) > 1)
		return (printf("cd: too many arguments\n"), shell->exit_code = 1);
	change_directory(shell, args, status);
	if (status[0] == -1)
		return(perror("cd"), shell->exit_code = 1);
	new_value[1] = getcwd(NULL, 0);
	env_key[0] = ft_strjoin("PWD=", new_value[1]);
	env_key[1] = ft_strjoin("OLDPWD=", new_value[0]);
	builtin_export(shell, env_key);
	if (status[1] == 1)
		printf("%s", new_value[1]);
	free_split(env_key);
	shell->exit_code = 0;
}

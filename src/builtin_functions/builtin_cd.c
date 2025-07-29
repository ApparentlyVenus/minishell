/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yitani <yitani@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 01:19:43 by yitani            #+#    #+#             */
/*   Updated: 2025/07/30 01:20:29 by yitani           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

/*
for 	char	*old_new_pwd[2];
old_new_pwd[0] == old_Pwd;
old_new_pwd[1] == new_pwd
*/

static int	execute_cd(char **args, t_env **env, char *pwd_env)
{
	int		status;
	int		print;
	char	*old_new_pwd[2];

	old_new_pwd[0] = getcwd(NULL, 0);
	if (!old_new_pwd[0] && !pwd_env)
		old_new_pwd[0] = ft_strdup("");
	else if (!old_new_pwd[0])
		old_new_pwd[0] = ft_strdup(pwd_env);
	status = change_directory(args, env, &print);
	if (status == -1)
		return (free(old_new_pwd[0]), 1);
	old_new_pwd[1] = getcwd(NULL, 0);
	if (!old_new_pwd[1] && !pwd_env)
	{
		old_new_pwd[1] = ft_strdup(getenv("HOME"));
		if (!old_new_pwd[1])
			old_new_pwd[1] = ft_strdup("/");
	}
	else if (!old_new_pwd[1])
		old_new_pwd[1] = ft_strjoin(pwd_env, "/..");
	update_pwd(env, old_new_pwd[0], old_new_pwd[1]);
	if (print == 1)
		printf("%s\n", old_new_pwd[1]);
	return (free(old_new_pwd[0]), free(old_new_pwd[1]), 0);
}

int	builtin_cd(char **args, t_env **env)
{
	char	*pwd_env;

	if (size_of_arr(args) > 1)
		return (ft_putendl_fd("cd: too many arguments", STDERR_FILENO), 1);
	pwd_env = get_env_value(*env, "PWD");
	return (execute_cd(args, env, pwd_env));
}

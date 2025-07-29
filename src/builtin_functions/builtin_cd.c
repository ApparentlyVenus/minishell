/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odana <odana@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/06 04:22:23 by yitani            #+#    #+#             */
/*   Updated: 2025/07/28 21:27:00 by odana            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static char	*expand_floaty(char *value, t_env **env)
{
	char	*result;
	char	*home;
	char	*suffix;

	if (!value || value[0] != '~'
		|| (value[1] != '/' && value[1] != '\0'))
		return (ft_strdup(value));
	home = get_env_value(*env, "HOME");
	if (!home)
		return (ft_strdup(value));
	suffix = ft_strdup(value + 1);
	if (!suffix)
		return (ft_strdup(value));
	result = ft_strjoin(home, suffix);
	free(suffix);
	if (!result)
		return (ft_strdup(value));
	return (result);
}

int	handle_floaty(char *path, t_env **env)
{
	char	*target_dir;
	int		result;

	target_dir = expand_floaty(path, env);
	if (!target_dir)
		return (ft_putendl_fd("cd: expansion failed", STDERR_FILENO), -1);
	result = chdir(target_dir);
	free(target_dir);
	return (result);
}

int	change_directory(char **args, t_env **env, int *print)
{
	int		result;
	char	*target_dir;

	*print = 0;
	if (!args || !args[0])
	{
		target_dir = get_env_value(*env, "HOME");
		if (!target_dir)
			return (ft_putendl_fd("cd: HOME not set", STDERR_FILENO), -1);
	}
	else if (args[0][0] == '-' && args[0][1] == '\0')
	{
		target_dir = get_env_value(*env, "OLDPWD");
		if (!target_dir)
			return (ft_putendl_fd("cd: OLDPWD not set", STDERR_FILENO), -1);
		*print = 1;
	}
	else
		return (handle_floaty(args[0], env));
	result = chdir(target_dir);
	return (result);
}

void	update_pwd(t_env **env, char *old_pwd, char *new_pwd)
{
	char	*oldpwd_var;
	char	*pwd_var;
	int		x[1];

	x[0] = 0;
	pwd_var = ft_strjoin("PWD=", new_pwd);
	oldpwd_var = ft_strjoin("OLDPWD=", old_pwd);
	if (pwd_var)
	{
		export_helper(env, ft_strchr(pwd_var, '='), pwd_var, x);
		free(pwd_var);
	}
	if (oldpwd_var)
	{
		export_helper(env, ft_strchr(oldpwd_var, '='), oldpwd_var, x);
		free(oldpwd_var);
	}
}

/*
for 	char	*old_new_pwd[2];
old_new_pwd[0] == old_Pwd;
old_new_pwd[1] == new_pwd
*/

int	builtin_cd(char **args, t_env **env)
{
	int		status;
	int		print;
	char	*old_new_pwd[2];
	char	*pwd_env;

	if (size_of_arr(args) > 1)
		return (ft_putendl_fd("cd: too many arguments", STDERR_FILENO), 1);
	pwd_env = get_env_value(*env, "PWD");
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
		old_new_pwd[1] = ft_strdup("/");
	else if (!old_new_pwd[1])
		old_new_pwd[1] = ft_strjoin(pwd_env, "/..");
	update_pwd(env, old_new_pwd[0], old_new_pwd[1]);
	if (print == 1)
		printf("%s\n", old_new_pwd[1]);
	return (free (old_new_pwd[0]), free(old_new_pwd[1]), (0));
}

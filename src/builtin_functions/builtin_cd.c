/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odana <odana@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/06 04:22:23 by yitani            #+#    #+#             */
/*   Updated: 2025/07/27 16:15:08 by odana            ###   ########.fr       */
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

// int	builtin_cd(char **args, t_env **env)
// {
// 	int		status;
// 	int		print;
// 	char	*new_value[2];

// 	new_value[0] = safe_getcwd(env);
// 	if (size_of_arr(args) > 1)
// 		return (ft_putendl_fd("cd: too many arguments", STDERR_FILENO),
// 			free(new_value[0]), 1);
// 	status = change_directory(args, env, &print);
// 	if (status == -1)
// 		return (perror("cd"), free(new_value[0]), 1);
// 	new_value[1] = getcwd(NULL, 0);
// 	update_pwd(env, new_value[0], new_value[1]);
// 	if (print == 1)
// 		printf("%s\n", new_value[1]);
// 	free (new_value[0]);
// 	free(new_value[1]);
// 	return (0);
// }

// static int	handle_pwd_update(t_env **env, char *old_pwd, int print)
// {
// 	char	*new_pwd;
// 	char	*new_pwd_copy;
// 	char	*temp_pwd;

// 	new_pwd = get_env_value(*env, "PWD");
// 	if (!new_pwd)
// 	{
// 		temp_pwd = safe_getcwd(env);
// 		if (!temp_pwd)
// 		{
// 			ft_putendl_fd("cd: unable to determine new directory", STDERR_FILENO);
// 			return (1);
// 		}
// 		update_pwd(env, old_pwd, temp_pwd);
// 		free(temp_pwd);
// 	}
// 	else
// 	{
// 		new_pwd_copy = ft_strdup(new_pwd);
// 		if (new_pwd_copy)
// 		{
// 			update_pwd(env, old_pwd, new_pwd_copy);
// 			free(new_pwd_copy);
// 		}
// 	}
// 	if (print == 1)
// 	{
// 		new_pwd = get_env_value(*env, "PWD");
// 		if (new_pwd)
// 			printf("%s\n", new_pwd);
// 	}
// 	return (0);
// }

// /* 

// int	builtin_cd(char **args, t_env **env)
// {
// 	int		status;
// 	int		print;
// 	char	*old_pwd;

// 	if (size_of_arr(args) > 1)
// 		return (ft_putendl_fd("cd: too many arguments", STDERR_FILENO), 1);
// 	old_pwd = safe_getcwd(env);
// 	if (!old_pwd)
// 	{
// 		ft_putendl_fd("cd: unable to determine current directory", STDERR_FILENO);
// 		old_pwd = ft_strdup("/");
// 	}
// 	status = change_directory(args, env, &print);
// 	if (status == -1)
// 	{
// 		free(old_pwd);
// 		return (1);
// 	}
// 	status = handle_pwd_update(env, old_pwd, print);
// 	free(old_pwd);
// 	return (status);
// }

int	builtin_cd(char **args, t_env **env)
{
	int		status;
	int		print;
	char	*old_pwd;
	char	*new_pwd;

	// ✅ FIX: Check args before allocating memory
	if (size_of_arr(args) > 1)
		return (ft_putendl_fd("cd: too many arguments", STDERR_FILENO), 1);
	
	old_pwd = safe_getcwd(env);
	if (!old_pwd)
	{
		ft_putendl_fd("cd: unable to determine current directory", STDERR_FILENO);
		old_pwd = ft_strdup("/");
	}
	
	status = change_directory(args, env, &print);
	if (status == -1)
	{
		free(old_pwd);
		return (1);
	}
	
	// ✅ FIX: Don't call safe_getcwd() again - use PWD environment variable
	new_pwd = get_env_value(*env, "PWD");
	if (!new_pwd)
	{
		// Only fallback to safe_getcwd if PWD is not set
		char *temp_pwd = safe_getcwd(env);
		if (!temp_pwd)
		{
			ft_putendl_fd("cd: unable to determine new directory", STDERR_FILENO);
			free(old_pwd);
			return (1);
		}
		update_pwd(env, old_pwd, temp_pwd);
		free(temp_pwd);
	}
	else
	{
		// PWD is set correctly, duplicate it for update_pwd
		char *new_pwd_copy = ft_strdup(new_pwd);
		if (new_pwd_copy)
		{
			update_pwd(env, old_pwd, new_pwd_copy);
			free(new_pwd_copy);
		}
	}
	
	if (print == 1)
	{
		char *current_pwd = get_env_value(*env, "PWD");
		if (current_pwd)
			printf("%s\n", current_pwd);
	}
	
	free(old_pwd);
	return (0);
}

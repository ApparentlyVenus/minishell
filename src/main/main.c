/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yitani <yitani@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/15 18:50:40 by odana             #+#    #+#             */
/*   Updated: 2025/07/30 01:09:01 by yitani           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	g_signal_received = 0;

int	process_input(t_shell *shell, char *input)
{
	if (!shell_tokenize(shell, input))
		return (0);
	if (!shell_parse(shell))
		return (0);
	if (!shell_execute(shell))
		return (0);
	return (1);
}

char	*ft_strjoin3(char *s1, char *s2, char *s3)
{
	char	*tmp;
	char	*res;

	tmp = ft_strjoin(s1, s2);
	if (!tmp)
		return (NULL);
	res = ft_strjoin(tmp, s3);
	free(tmp);
	return (res);
}

char	*get_prompt(t_env **env)
{
	char	*cwd = getcwd(NULL, 0);
	char	*path = cwd ? cwd : get_env_value(*env, "PWD");
	char	*temp;
	char	*prompt;

	if (!path)
		return (ft_strdup("minishell$ "));

	temp = ft_strjoin("minishell: @", path);
	if (cwd)
		free(cwd);
	if (!temp)
		return (ft_strdup("minishell$ "));
	prompt = ft_strjoin3(BLUE, temp, "$ " RESET);
	free(temp);
	if (!prompt)
		return (ft_strdup("minishell$ "));
	return (prompt);
}

int	main_loop(t_shell *shell)
{
	char	*input;

	while (1)
	{
		set_shell(shell);
		if (shell->interactive)
			signals_prompt();
		input = get_input_line(&shell->env);
		if (!input)
		{
			if (shell->interactive)
				ft_putendl_fd("exit", STDOUT_FILENO);
			break ;
		}
		if (!*input)
		{
			free(input);
			continue ;
		}
		process_input(shell, input);
		shell_reset_phase(shell);
		free(input);
	}
	return (shell->exit_code);
}

int	main(int argc, char **argv, char **envp)
{
	t_shell	*shell;
	int		exit_code;

	if (argc > 1)
	{
		printf("minishell: %s: No such file or directory\n", argv[1]);
		exit(ERROR);
	}
	rl_catch_signals = 0;
	shell = shell_init(envp);
	if (!shell)
	{
		ft_putendl_fd("minishell: initialization failed\n", STDERR_FILENO);
		return (EXIT_GENERAL_ERROR);
	}
	update_shlvl(shell);
	if (shell->interactive)
		art();
	exit_code = main_loop(shell);
	shell_cleanup(shell);
	rl_clear_history();
	return (exit_code);
}

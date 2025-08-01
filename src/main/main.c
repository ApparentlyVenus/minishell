/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yitani <yitani@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/15 18:50:40 by odana             #+#    #+#             */
/*   Updated: 2025/08/01 13:16:48 by yitani           ###   ########.fr       */
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

int	main_loop(t_shell *shell)
{
	char	*input;

	while (1)
	{
		set_shell(shell);
		if (shell->interactive)
			signals_prompt();
		g_signal_received = 0;
		input = get_input_line();
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
		ft_putendl_fd("minishell: initialization failed", STDERR_FILENO);
		return (EXIT_FAILURE);
	}
	update_shlvl(shell);
	if (shell->interactive)
		art();
	exit_code = main_loop(shell);
	shell_cleanup(shell);
	rl_clear_history();
	return (exit_code);
}

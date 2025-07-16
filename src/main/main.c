/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yitani <yitani@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/15 18:50:40 by odana             #+#    #+#             */
/*   Updated: 2025/07/16 14:32:07 by yitani           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

static int process_input(t_shell *shell, char *input)
{
	if (!shell_tokenize(shell, input))
		return (0);
	if (!shell_expand(shell))
		return (0);
	if (!shell_parse(shell))
		return (0);
	if (!shell_execute(shell))
		return (0);
	return (1);
}

static char *get_prompt(void)
{
	char *cwd;
	char *prompt;
	char *temp;
	
	cwd = getcwd(NULL, 0);
	if (!cwd)
		return (ft_strdup("minishell$ "));
	temp = ft_strjoin("minishell:", cwd);
	free(cwd);
	if (!temp)
		return (ft_strdup("minishell$ "));
	prompt = ft_strjoin(temp, "$ ");
	free(temp);
	if (!prompt)
		return (ft_strdup("minishell$ "));
	return (prompt);
}

static int main_loop(t_shell *shell)
{
	char *input;

	while (1)
	{
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


int main(int argc, char **argv, char **envp)
{
	t_shell	*shell;
	int		exit_code;
	
	(void)argc;
	(void)argv;
	shell = shell_init(envp);
	if (!shell)
	{
		ft_putendl_fd("minishell: initialization failed", STDERR_FILENO);
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


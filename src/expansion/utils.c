/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yitani <yitani@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/08 09:38:35 by odana             #+#    #+#             */
/*   Updated: 2025/07/08 11:15:46 by yitani           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	is_valid_var_start(char c)
{
	return (ft_isalpha(c) || c == '_');
}

int	is_valid_var_char(char c)
{
	return (ft_isalnum(c) || c == '_');
}

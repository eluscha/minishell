/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal_handlers.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eleonora <eleonora@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/24 14:34:28 by auspensk          #+#    #+#             */
/*   Updated: 2024/10/09 09:12:04 by eleonora         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handle_sigint(int sig)
{
	lastsignal = sig;
	printf("\n");
	rl_replace_line("", 0);
	rl_on_new_line();
	rl_redisplay();
}

void	handle_sigint_ex(int sig)
{
	lastsignal = sig;
}

void	handle_sigquit_ex(int sig) // this handler united with prev one ?
{
	lastsignal = sig;
	write(2, "Quit\n", 5);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal_handlers.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: auspensk <auspensk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/24 14:34:28 by auspensk          #+#    #+#             */
/*   Updated: 2024/09/24 14:35:39 by auspensk         ###   ########.fr       */
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
	//rl_clear_display();

}

void	handle_sigint_ex(int sig)
{
	lastsignal = sig;
	printf("\n");
}

void	handle_sigquit_ex(int sig)
{
	lastsignal = sig;
	write (2, "Quit\n", 5);
}

/*
int g_signal_code = 0;

void	handle_sigint(int sig)
{
	(void)sig;
	g_signal_code = 1;
	//rl_on_new_line();
	// if (exec_chld)
	// 	write(1, "\n", 1);
	// else
	// 	write(1, "\nminishell> ", ft_strlen("\nminishell> "));
	// rl_replace_line("", 0);
	// rl_redisplay();
	// write(1, "\nminishell> ", ft_strlen("\nminishell> "));
	// fflush(stdout); //not allowed function, but was advised in codevault
}

void	handle_sigint_ch(int sig)
{
	(void)sig;
}

*/

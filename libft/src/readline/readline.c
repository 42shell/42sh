/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   readline.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fratajcz <fratajcz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/21 20:08:08 by fratajcz          #+#    #+#             */
/*   Updated: 2020/05/08 18:45:44 by fratajcz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "readline.h"

static char	*rl_return(int ret)
{
	char	*line;

	rl_carriage_return(false);
	tcsetattr(STDIN_FILENO, TCSADRAIN, &g_rl_oldterm);
	if (g_rl_error || !(line = ft_strdup(g_rl_line.dstr->str)))
	{
		if (!g_rl_error)
			g_rl_error = RL_ALLOC_ERROR;
		g_rl_last_ret = g_rl_error;
		ft_putstr_fd("readline: fatal error", 2);
		exit(1);
	}
	if (ret != RL_ENTER)
		g_rl_last_ret = ret;
	if (g_rl_line.backup)
		ft_dstr_del(&g_rl_line.backup);
	ft_dstr_clear(g_rl_line.dstr, 32);
	g_rl_hist.ptr = g_rl_hist.head;
	g_rl_prompt = NULL;
	g_rl_oldkey = 0;
	g_rl_line.i = 0;
	g_rl_posx = 0;
	g_rl_posy = 0;
	return (line);
}

static int	rl_init(void)
{
	if (!g_rl_is_init)
	{
		if (rl_init_term() != 0 || rl_bind_keys() != 0)
			return (-1);
		if (!(g_rl_line.dstr = ft_dstr_new(32)))
		{
			g_rl_error = RL_ALLOC_ERROR;
			return (-1);
		}
		g_rl_is_init = true;
	}
	tcsetattr(STDIN_FILENO, TCSADRAIN, &g_rl_term);
	tputs(g_rl_caps[C_KS], 20, ft_putc);
	return (0);
}

/*
** -utf-8 ?
** -term.h ft_term..
** -scroll
*/

char		*readline(const char *prompt)
{
	int		ret;

	if ((ret = rl_init()) != 0)
		return (rl_return(ret));
	rl_print_prompt(prompt);
	while ((ret = readc(STDIN_FILENO, g_rl_key.bytes)))
	{
		if (ret == -1)
		{
			g_rl_error = RL_SYSCALL_ERROR;
			break ;
		}
		if ((ret = g_rl_keymap[*g_rl_key.bytes]()) != RL_CONTINUE)
			break ;
		g_rl_oldkey = *(long *)g_rl_key.bytes;
		*(long *)g_rl_key.bytes = 0;
		g_rl_key.i = 0;
	}
	return (rl_return(ret));
}

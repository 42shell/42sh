/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fratajcz <fratajcz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/21 20:08:08 by fratajcz          #+#    #+#             */
/*   Updated: 2020/05/06 18:18:40 by fratajcz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

static void	increase_shlvl(void)
{
	char	*shlvl_str;
	char	buf[10];
	int		shlvl_int;

	shlvl_str = get_var_value("SHLVL");
	if (shlvl_str != NULL)
		shlvl_int = ft_atoi(shlvl_str) + 1;
	else
		shlvl_int = 1;
	if (shlvl_int >= 1000)
	{
		dprintf(2, "42sh: warning: shell level (%d) too high, resetting to 1",
				shlvl_int);
		shlvl_int = 1;
	}
	set_var("SHLVL", ft_itoa(shlvl_int, buf), V_EXPORT);
}

static int	parse_args(int argc, char **argv)
{
	int		fd;

	if (argc > 0)
	{
		if ((fd = open(argv[0], O_RDONLY)) == -1)
		{
			write(STDERR_FILENO, "42sh: Could not open file\n", 26);
			exit(1);
		}
		g_shell.get_input = &input_batch;
		dup2(fd, STDIN_FILENO);
		close(fd);
	}
	else
	{
		g_shell.get_input = &input_interactive;
		g_shell.interactive_mode = true;
		g_rl_complete.get_matches = &get_autocomplete_list;
		g_rl_retain_nl = true;
		//g_rl_prompt_cr = true;
		g_rl_hist_doubl = false;
	}
	return (0);
}

static void	init_vars(void)
{
	extern char	**environ;
	extern char	g_shell_pid[10];
	t_var		*var;

	ft_itoa(getpid(), g_shell_pid);
	g_shell.vars = ht_new(1024, free_var);
	import_env(environ);
	increase_shlvl();
	var = make_new_var("?", NULL, V_HIDDEN | V_SPECIAL, get_last_exit_status);
	ht_put(g_shell.vars, "?", var);
	var = make_new_var("$", NULL, V_HIDDEN | V_SPECIAL, get_shell_pid);
	ht_put(g_shell.vars, "$", var);
	var = make_new_var("!", NULL, V_HIDDEN | V_SPECIAL, get_last_bg_job_pid);
	ht_put(g_shell.vars, "!", var);
}

int			init(int argc, char **argv)
{

	if (!isatty(STDIN_FILENO))
	{
		ft_dprintf(2, "42sh: stdin is not a tty\n");
		exit(1);
	}
	init_vars();
	parse_args(argc, argv);
	if (g_shell.interactive_mode)
	{
		g_job_control_enabled = true;
		while (tcgetpgrp(STDIN_FILENO) != (g_shell.pgid = getpgrp()))
    		kill(-g_shell.pgid, SIGTTIN);
		init_sig();
		/* Put ourselves in our own process group. */
		g_shell.pgid = getpid();
		if (setpgid (g_shell.pgid, g_shell.pgid) < 0)
		{
			ft_dprintf(2, "42sh: Couldn't put the shell in its own process group\n");
			exit(1);
		}
    	/* Grab control of the terminal. */
		tcgetattr(STDIN_FILENO, &g_shell.tmodes);
		tcsetpgrp(STDIN_FILENO, g_shell.pgid);
	}
	return (0);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fratajcz <fratajcz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/21 20:08:08 by fratajcz          #+#    #+#             */
/*   Updated: 2020/04/02 02:05:26 by fratajcz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

static void	increase_shlvl(void)
{
	char	*shlvl_str;
	int		shlvl_int;

	shlvl_str = get_env_var("SHLVL");
	if (shlvl_str != NULL)
		shlvl_int = ft_atoi(shlvl_str) + 1;
	else
		shlvl_int = 1;
	shlvl_str = ft_itoa(shlvl_int);
	set_env_var("SHLVL", shlvl_str);
	free(shlvl_str);
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
		g_rl_retain_nl = true;
		//g_rl_prompt_cr = true;
		g_rl_hist_doubl = false;
	}
	init_sig();
	return (0);
}

int			init(int argc, char **argv)
{
	extern char	**environ;

	if (!isatty(STDIN_FILENO))
	{
		ft_dprintf(2, "42sh: stdin is not a tty\n");
		exit(1);
	}
	while (tcgetpgrp(STDIN_FILENO) != (g_shell.pgid = getpgrp()))
        kill(-g_shell.pgid, SIGTTIN);
	/* Put ourselves in our own process group. */
	g_shell.pgid = getpid();
	tcgetattr(STDIN_FILENO, &g_shell.tmodes);
	if (setpgid (g_shell.pgid, g_shell.pgid) < 0)
	{
		ft_dprintf(2, "42sh: Couldn't put the shell in its own process group\n");
		exit (1);
	}
    /* Grab control of the terminal. */
	signal(SIGTTOU, SIG_IGN);
	tcsetpgrp(STDIN_FILENO, g_shell.pgid);
	g_env = env_dup(environ);
	parse_args(argc, argv);
	increase_shlvl();
	return (0);
}

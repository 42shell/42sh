/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fratajcz <fratajcz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/21 20:08:08 by fratajcz          #+#    #+#             */
/*   Updated: 2020/04/11 13:44:50 by fratajcz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

static void	increase_shlvl(void)
{
	char	*shlvl_str;
	int		shlvl_int;

	shlvl_str = get_env_var("SHLVL", g_env);
	if (shlvl_str != NULL)
		shlvl_int = ft_atoi(shlvl_str) + 1;
	else
		shlvl_int = 1;
	shlvl_str = ft_itoa(shlvl_int);
	set_env_var("SHLVL", shlvl_str, g_env);
	free(shlvl_str);
}

static int	check_bin_file(int fd, char *filename)
{
	char	buf[80];
	int		i;

	i = 0;
	if (read(fd, buf, 80) == -1)
	{
		ft_dprintf(STDERR_FILENO, "42sh: %s: unable to read file\n");
		return (-1);
	}
	while (i < 80 && buf[i] && buf[i] != '\n')
		i++;
	if (buf[i] == 0)
	{
		ft_dprintf(STDERR_FILENO,
		"42sh: %s: cannot execute binary file\n", filename);
		return (-1);
	}
	return (0);
}

static int	parse_args(int argc, char **argv)
{
	int		fd;

	if (argc > 0)
	{
		if ((fd = open(argv[0], O_RDONLY)) == -1)
		{
			write(STDERR_FILENO, "42sh: could not open file\n", 26);
			exit(1);
		}
		else if (check_bin_file(fd, argv[0]) == -1)
		{
			close(fd);
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

int			init(int argc, char **argv)
{
	extern char	**environ;

	if (!isatty(STDIN_FILENO))
	{
		ft_dprintf(2, "42sh: stdin is not a tty\n");
		exit(1);
	}
	g_env = env_dup(environ);
	increase_shlvl();
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
			exit (1);
		}
    	/* Grab control of the terminal. */
		tcgetattr(STDIN_FILENO, &g_shell.tmodes);
		tcsetpgrp(STDIN_FILENO, g_shell.pgid);
	}
	return (0);
}

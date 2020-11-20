/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fratajcz <fratajcz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/21 20:08:08 by fratajcz          #+#    #+#             */
/*   Updated: 2020/06/05 17:16:01 by fratajcz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

static void	init_vars(void)
{
	extern char	**environ;
	extern char	g_shell_pid[12];
	t_var		*var;

	g_lexer.brack_stack = array_new(4);
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
	var = make_new_var("!!", NULL, V_HIDDEN | V_SPECIAL, get_last_bg_job_pgid);
	ht_put(g_shell.vars, "!!", var);
	var = make_new_var("0", NULL, V_HIDDEN | V_SPECIAL, get_shell_name);
	ht_put(g_shell.vars, "0", var);
}

static void	init_builtins(void)
{
	g_builtins = ht_new(32, free);
	ht_put(g_builtins, "env", builtin_env);
	ht_put(g_builtins, "exit", builtin_exit);
	ht_put(g_builtins, "unsetenv", builtin_unsetenv);
	ht_put(g_builtins, "setenv", builtin_setenv);
	ht_put(g_builtins, "echo", builtin_echo);
	ht_put(g_builtins, "cd", builtin_cd);
	ht_put(g_builtins, "export", builtin_export);
	ht_put(g_builtins, "unset", builtin_unset);
	ht_put(g_builtins, "set", builtin_set);
	ht_put(g_builtins, "fg", builtin_fg);
	ht_put(g_builtins, "bg", builtin_bg);
	ht_put(g_builtins, "jobs", builtin_jobs);
	ht_put(g_builtins, "test", builtin_test);
	ht_put(g_builtins, "hash", builtin_hash);
	ht_put(g_builtins, "type", builtin_type);
}

static int	set_input_fd(char *filename)
{
	int		fd;

	if (file_may_be_binary(filename))
		exit(1);
	if ((fd = open(filename, O_RDONLY)) == -1)
	{
		write(STDERR_FILENO, "42sh: could not open file\n", 26);
		exit(1);
	}
	g_shell.input_file_fd = fd;
	return (0);
}

bool		g_job_control_enabled;

static int	init_interactive_mode(void)
{
	pid_t	shell_pid;

	g_job_control_enabled = true;
	while (tcgetpgrp(STDIN_FILENO) != (g_shell.pgid = getpgrp()))
		kill(-g_shell.pgid, SIGTTIN);
	init_sig();
	shell_pid = getpid();
	if (g_shell.pgid != shell_pid && setpgid(0, 0) < 0)
	{
		ft_dprintf(2,
		"42sh: Couldn't put the shell in its own process group\n");
		exit(1);
	}
	g_shell.pgid = shell_pid;
	tcgetattr(STDIN_FILENO, &g_shell.tmodes);
	tcsetpgrp(STDIN_FILENO, g_shell.pgid);
	g_shell.get_input = &input_interactive;
	g_rl_complete.get_matches = &get_autocomplete_list;
	g_rl_hist_doubl = false;
	g_rl_retain_nl = true;
	g_shell.get_input = &input_interactive;
	g_rl_retain_nl = true;
	return (0);
}

int			g_msg_qid;

int			init(int argc, char **argv)
{
	g_shell.interactive_mode = true;
	if (!isatty(STDIN_FILENO))
	{
		g_shell.input_file_fd = STDIN_FILENO;
		g_shell.get_input = &input_batch;
		g_shell.interactive_mode = false;
	}
	if (argc > 0)
	{
		set_input_fd(argv[0]);
		g_shell_name = argv[0];
		g_shell.get_input = &input_batch;
		g_shell.interactive_mode = false;
	}
	if (g_shell.interactive_mode)
		init_interactive_mode();
	init_builtins();
	init_vars();
	g_msg_qid = msgget(IPC_PRIVATE, IPC_CREAT | 0600);
	g_binaries = ht_new(256, free);
	return (0);
}

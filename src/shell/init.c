/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fratajcz <fratajcz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/21 20:08:08 by fratajcz          #+#    #+#             */
/*   Updated: 2020/01/29 14:18:16 by fratajcz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

static void	increase_shlvl(t_env *env)
{
	char	*shlvl_str;
	int		shlvl_int;

	shlvl_str = get_env_var("SHLVL", env);
	if (shlvl_str != NULL)
		shlvl_int = ft_atoi(shlvl_str) + 1;
	else
		shlvl_int = 1;
	shlvl_str = ft_itoa(shlvl_int);
	set_env_var("SHLVL", shlvl_str, env);
	free(shlvl_str);
}

/*
** check this, readline write on 2 ? etc...
*/

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
		dup2(fd, STDIN_FILENO);
		close(fd);
		return (0);
	}
	g_shell.interactive_mode = true;
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
	if (parse_args(argc, argv) == 0)
	{
		if (g_shell.interactive_mode)
		{
			g_shell.get_input = &input_interactive;
			g_rl_retain_nl = true;
			g_rl_prompt_cr = true;
			g_rl_hist_doubl = false;
			init_sig();
		}
		else
			g_shell.get_input = &input_batch;
	}
	g_env = env_dup(environ);
	increase_shlvl(g_env);
	return (0);
}

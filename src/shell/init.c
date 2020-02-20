/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fratajcz <fratajcz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/21 20:08:08 by fratajcz          #+#    #+#             */
/*   Updated: 2020/02/19 15:38:13 by fratajcz         ###   ########.fr       */
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

static int	parse_args(t_sh *shell, int argc, char **argv)
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
	shell->input.interactive = true;
	return (0);
}

int			init(t_sh *shell, int argc, char **argv)
{
	extern char	**environ;

	if (!isatty(STDIN_FILENO))
	{
		ft_dprintf(2, "42sh: stdin is not a tty\n");
		exit(1);
	}
	ft_bzero(shell, sizeof(*shell));
	parse_args(shell, argc, argv);
	if (shell->input.interactive)
	{
		init_sig(shell);
		init_term(&shell->term);
	}
	init_input(&shell->input, &shell->term);
	init_lexer(&shell->lexer, &shell->input);
	shell->env = env_dup(environ);
	g_env = &shell->env;
	g_sh_vars = ht_new(1024, free_var);
	import_env(g_sh_vars, g_env->env);
	increase_shlvl(&shell->env);
	return (0);
}

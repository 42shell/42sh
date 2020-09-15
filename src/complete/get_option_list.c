/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_option_list.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fratajcz <fratajcz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/10 19:02:32 by fratajcz          #+#    #+#             */
/*   Updated: 2020/09/10 19:02:32 by fratajcz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

static size_t	get_command_pos(char *context, size_t opt_pos)
{
	size_t	i;

	i = opt_pos;
	while (i-- > 0)
	{
		if (context[i] == '&' || context[i] == ';' || context[i] == '|')
			return (i + 1);
	}
	return (i + 1);
}

static char		*get_command_name(char *context, size_t opt_pos)
{
	size_t i;
	size_t cmd_start;

	i = get_command_pos(context, opt_pos);
	while (ft_iswhitespace(context[i]))
		i++;
	cmd_start = i;
	while (context[i] && !ft_iswhitespace(context[i]))
		i++;
	return (ft_strndup(context + cmd_start, i - cmd_start));
}

t_list_head		*get_options_from_file(char *partial, char *filename)
{
	int			fd;
	char		*line;
	t_list_head	*option_list;

	option_list = ft_list_first_head("");
	fd = open(filename, O_RDONLY);
	while (get_next_line(fd, &line) > 0)
	{
		if (ft_strstr(line, partial) == line)
		{
			ft_list_add_tail(ft_strdup(line), option_list);
			(*g_comp_list_count)++;
		}
		free(line);
	}
	close(fd);
	return (option_list);
}

t_list_head		*get_options_from_sh(char *partial,
									char **sh_argv, char **sh_env)
{
	char		*tmp_file;
	t_list_head	*option_list;
	int			pid;
	int			fd1;
	int			fd2;

	tmp_file = ft_mktemp(ft_strdup("/tmp/42sh_XXXXXX"));
	if ((pid = fork()) == 0)
	{
		fd1 = open(tmp_file, O_WRONLY | O_CREAT);
		fd2 = open("/dev/null", O_RDWR);
		dup2(fd1, 1);
		dup2(fd2, 2);
		execve("/bin/sh", sh_argv, sh_env);
	}
	free(sh_env[0]);
	waitpid(pid, NULL, 0);
	option_list = get_options_from_file(partial, tmp_file);
	unlink(tmp_file);
	free(tmp_file);
	return (option_list);
}

t_list_head		*get_option_list(char *partial, char *context, size_t opt_pos)
{
	char	*sh_env[2];
	char	*sh_argv[4];
	char	*cmd_name;

	cmd_name = get_command_name(context, opt_pos);
	sh_env[0] = ft_strjoin("CMD=", cmd_name);
	free(cmd_name);
	sh_env[1] = NULL;
	sh_argv[0] = "/bin/sh";
	sh_argv[1] = "-c";
	sh_argv[2] = "man $CMD"
		" | grep -oE -- '-[[:alnum:]][[:space:]\\,]|\\-\\-[[:alnum:]\\-]+' "
		" | sed -e '/\\-\\-\\-/d'  -e 's/\\,//g' -e 's/[[:space:]]//g'"
		" | sort -u";
	sh_argv[3] = NULL;
	return (get_options_from_sh(partial, sh_argv, sh_env));
}

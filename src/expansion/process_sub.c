/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_sub.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fratajcz <fratajcz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/22 18:06:18 by fratajcz          #+#    #+#             */
/*   Updated: 2020/09/22 18:06:18 by fratajcz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

static char		**g_cmd;
static size_t	g_i = 0;

void		reset_subshell_globals(void)
{
	g_shell.interactive_mode = false;
	g_job_control_enabled = false;
	g_already_forked = false;
	g_jobs = NULL;
	g_current_jobs = NULL;
}

static int	input_process_sub(const char *prompt, bool heredoc)
{
	char			*tmp;

	(void)prompt;
	(void)heredoc;
	if (g_cmd && g_cmd[g_i])
	{
		if (g_lexer.line)
		{
			tmp = g_lexer.line;
			g_lexer.line = ft_strjoin_triple(g_lexer.line, g_cmd[g_i++], "\n");
			free(tmp);
		}
		else
			g_lexer.line = ft_strjoin(g_cmd[g_i++], "\n");
		return (0);
	}
	else
	{
		if (!g_lexer.line)
			builtin_exit(NULL, NULL);
		return (INPUT_EOF);
	}
	return (0);
}

/*
** The fd will be closed when the job is done
*/

static void	add_fd_to_job_close_list(int fd)
{
	int	*fd_ptr;

	fd_ptr = ft_xmalloc(sizeof(int));
	if (g_current_jobs->fds_to_close == NULL)
		g_current_jobs->fds_to_close = array_new(2);
	*fd_ptr = fd;
	array_append(g_current_jobs->fds_to_close, fd_ptr);
}

int			exec_procsub(int *fildes, int type)
{
	int			pid;
	void		*ignore_fake_leak[2];

	if ((pid = fork()) == 0)
	{
		dup2(open("/dev/null", O_RDWR), STDIN_FILENO);
		dup2(fildes[type], type);
		close(fildes[type]);
		close(fildes[!type]);
		ignore_fake_leak[0] = g_cmd;
		ignore_fake_leak[1] = g_jobs;
		(void)ignore_fake_leak;
		reset_subshell_globals();
		g_i = 0;
		g_shell.get_input = input_process_sub;
		reset_signals();
		reset_lexer();
		main_loop();
	}
	close(fildes[type]);
	add_fd_to_job_close_list(fildes[!type]);
	return (pid < 0 ? -1 : 0);
}

#define INPUT_PROCSUB 1
#define OUTPUT_PROCSUB 0

int			process_sub(t_token *token, int *i)
{
	int		end;
	char	*dev_fd;
	int		fildes[2];
	char	buf[12];
	int		type;

	type = (token->value->str[0] == '<') ? INPUT_PROCSUB : OUTPUT_PROCSUB;
	end = get_end_of_braces(token->value->str, *i);
	token->value->str[end] = '\0';
	token->exp_info->str[end] = '\0';
	g_cmd = ft_strsplit(token->value->str + *i + 2, '\n');
	if (pipe(fildes) < 0)
		return (1 && ft_dprintf(2, "42sh: could not open procsub pipe\n"));
	if (exec_procsub(fildes, type) < 0)
		return (1 && ft_dprintf(2, "42sh: process substitution error"));
	dev_fd = ft_strjoin("/dev/fd/", ft_itoa(fildes[!type], buf));
	token_replace_between(token, *i, end, dev_fd);
	*i += ft_strlen(dev_fd) - 1;
	free(dev_fd);
	free_arr(g_cmd);
	g_cmd = NULL;
	return (0);
}

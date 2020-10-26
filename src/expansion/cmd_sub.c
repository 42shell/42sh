/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_sub.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fratajcz <fratajcz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/29 16:17:45 by fratajcz          #+#    #+#             */
/*   Updated: 2020/06/29 16:17:45 by fratajcz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

static char	**g_cmd;
static int	g_i;

void		wait_for_all_jobs(void)
{
	t_job	*job;
	t_job	*next;

	job = g_jobs;
	while (job != NULL)
	{
		next = job->next;
		wait_for_job(job);
		del_job_from_list(&g_jobs, job);
		job = next;
	}
}

static int	input_cmd_sub(const char *prompt
		, __attribute__((unused)) bool heredoc)
{
	char			*tmp;

	(void)prompt;
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
		{
			wait_for_all_jobs();
			builtin_exit(NULL, NULL);
		}
		return (INPUT_EOF);
	}
}

static char	*get_output(int fd)
{
	char	*str;
	char	*tmp;
	int		ret;
	char	buf[256];

	str = NULL;
	while ((ret = read(fd, buf, 255)) > 0)
	{
		buf[ret] = '\0';
		tmp = ft_strjoin(str, buf);
		free(str);
		str = tmp;
	}
	close(fd);
	if (str == NULL && ret != -1)
		return (ft_strdup(""));
	ret = ft_strlen(str) - 1;
	while (ret >= 0 && str[ret] == '\n')
		str[ret--] = '\0';
	return (str);
}

static char	*exec_cmd_sub(int *fildes)
{
	int		pid;
	char	*output;
	void	*ignore_fake_leak[2];

	if ((pid = fork()) == 0)
	{
		dup2(fildes[1], STDOUT_FILENO);
		close(fildes[1]);
		close(fildes[0]);
		ignore_fake_leak[0] = g_cmd;
		ignore_fake_leak[1] = g_jobs;
		(void)ignore_fake_leak;
		reset_subshell_globals();
		g_i = 0;
		g_shell.get_input = input_cmd_sub;
		reset_signals();
		reset_lexer();
		main_loop();
	}
	close(fildes[1]);
	output = pid > 0 ? get_output(fildes[0]) : NULL;
	if (pid > 0 && waitpid(pid, NULL, 0) > 0 && g_interrupt)
		output[0] = '\0';
	return (output);
}

int			cmd_sub(t_token *token, int *i)
{
	int		end;
	char	*output;
	int		fildes[2];

	end = get_end_of_braces(token->value->str, *i);
	token->value->str[end] = '\0';
	token->exp_info->str[end] = '\0';
	g_cmd = ft_strsplit(token->value->str + *i + 2, '\n');
	if (pipe(fildes) < 0)
		return (1 && ft_dprintf(2, "42sh: could not open cmd sub pipe\n"));
	if ((output = exec_cmd_sub(fildes)) == NULL)
		return (1 && ft_dprintf(2, "42sh: command substitution error\n"));
	token_replace_between(token, *i, end, output);
	*i += ft_strlen(output) - 1;
	free(output);
	free_arr(g_cmd);
	g_cmd = NULL;
	return (0);
}

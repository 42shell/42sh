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

static char	*g_cmd;

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
	if (g_interrupt)
		kill(getpid(), SIGINT);
}

static int	input_cmd_sub(const char *prompt, bool heredoc)
{
	(void)prompt;
	(void)heredoc;
	if (g_cmd)
	{
		g_lexer.line = g_cmd;
		g_cmd = NULL;
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
	return (0);
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

	if ((pid = fork()) < 0)
		return (NULL);
	if (pid == 0)
	{
		dup2(fildes[1], STDOUT_FILENO);
		close(fildes[1]);
		close(fildes[0]);
		g_jobs = NULL;
		g_current_jobs = NULL;
		g_shell.interactive_mode = false;
		g_job_control_enabled = false;
		g_shell.get_input = input_cmd_sub;
		reset_signals();
		reset_lexer();
		main_loop();
	}
	close(fildes[1]);
	output = get_output(fildes[0]);
	waitpid(pid, NULL, 0);
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
	g_cmd = ft_strdup(token->value->str + *i + 2);
	if (pipe(fildes) < 0)
		return (1 && ft_dprintf(2, "42sh: could not open cmd sub pipe\n"));
	if ((output = exec_cmd_sub(fildes)) == NULL)
		return (1 && ft_dprintf(2, "42sh: command substitution error\n"));
	token_replace_between(token, *i, end, output);
	*i += ft_strlen(output) - 1;
	free(output);
	free(g_cmd);
	g_cmd = NULL;
	return (0);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fratajcz <fratajcz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/21 19:37:33 by fratajcz          #+#    #+#             */
/*   Updated: 2020/01/23 10:55:57 by fratajcz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

/*
** echo blblabla; /!\
** EOF in batch mode /!\
** 		cat << EOF
** 		cat << EOF2 -> EOF no heredoc delim
*/

int			remove_escaped_newlines(char *line)
{
	char	quote;
	int		i;

	i = 0;
	quote = 0;
	while (line[i])
	{
		if (line[i] == BSLASH)
		{
			i++;
			if (line[i] == '\n')
				ft_memmove(&line[i - 1], &line[i + 1], ft_strlen(&line[i + 1]) + 1);
		}
		else if (line[i] == SQUOTE)
		{
			if (quote == SQUOTE)
				quote = 0;
			else if (quote == 0)
				quote = SQUOTE;
		}
		else if (line[i] == DQUOTE)
		{
			if (quote == DQUOTE)
				quote = 0;
			else if (quote == 0)
				quote = DQUOTE;
		}
		i++;
	}
	return (0);
}

int			get_input(const char *prompt)
{
	char	*line;
	char	*tmp;
	//int		ret;

	if (g_shell.interactive_mode)
	{
		if (!(line = readline(prompt)))
			exit(1);//error()
		else if (!*line)//ctrl-C || ctrl-D
		{
			g_parser.error = SILENT_ABORT;
			if (g_rl_last_ret == RL_EOF)
			{
				if (!g_lexer.line)
					exit(0);//builtin_exit()
			}
			free(line);
			free(g_lexer.line);
			g_lexer.line = NULL;
			return (g_rl_last_ret);
		}
		remove_escaped_newlines(line);
	}
	else
	{
		//if ((ret = get_next_line(0, line)) == -1);
		//	exit(1);//error();
		//else if (ret == 0)
		//	return (EOF);
		return (0);
	}
	if (g_lexer.line)
	{
		tmp = g_lexer.line;
		g_lexer.line = ft_strjoin(g_lexer.line, line);
		free(tmp);
	}
	else
		g_lexer.line = ft_strdup(line);
	free(line);
	return (0);

}

int			loop_interactive(void)
{
	return (0);
}

int			main(int argc, char **argv)
{
	t_job	*ptr;
	int		ret;

	init(argc - 1, argv + 1);
	while ((ret = get_input(PS1)) != RL_EOF)
	{
		g_parser.error = NOERR;
		g_parser.token = get_next_token();
		if ((g_shell.jobs = complete_command()))
		{
			ptr = g_shell.jobs;
			while (ptr && ptr->ast)
			{
				print_ast(ptr->ast, 0);
				printf("\n");
				ptr = ptr->next;
			}
			//run(ast);
			job_del(&g_shell.jobs);// if job is complete, job_delone
		}
		if (g_lexer.line)
		{
			g_lexer.line[ft_strlen(g_lexer.line) - 1] = 0;
			rl_add_history(g_lexer.line);
		}
		reset_lexer();
	}
	return (0);
}

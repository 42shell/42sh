/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fratajcz <fratajcz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/21 19:37:33 by fratajcz          #+#    #+#             */
/*   Updated: 2020/01/23 10:55:57 by fratajcz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

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

int			input_batch(const char *prompt)
{
	(void)prompt;
	exit(0);
}

int			input_interactive(const char *prompt)
{
	char	*line;
	char	*tmp;

	if (!(line = readline(prompt)))
		exit(1);//error()
	printf("SF\n");
	if (!*line)//ctrl-C || ctrl-D
	{
		free(line);
		if (g_rl_last_ret == RL_INT)
		{
			g_parser.error = SILENT_ABORT;
			ft_memdel((void **)&g_lexer.line);
		}
		if (g_rl_last_ret == RL_EOF)
		{
			//??
			if (!g_lexer.line)
				exit(0);//builtin_exit()
		}
		return (g_rl_last_ret == RL_EOF ? INPUT_EOF : INPUT_INT);
	}
	printf("SF\n");
	remove_escaped_newlines(line);
	printf("SF\n");
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

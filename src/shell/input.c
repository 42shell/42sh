/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fratajcz <fratajcz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/21 19:37:33 by fratajcz          #+#    #+#             */
/*   Updated: 2020/04/09 21:38:48 by fratajcz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

int			remove_escaped_newlines(char *line, bool ignore_quotes)
{
	char	quote;
	int		i;

	i = 0;
	quote = 0;
	while (line[i])
	{
		if (line[i] == BSLASH && (ignore_quotes ? 1 : !quote))
		{
			i++;
			if (line[i] == '\n')
				ft_memmove(&line[i - 1], &line[i + 1], ft_strlen(&line[i + 1]) + 1);
		}
		else if (!ignore_quotes && line[i] == SQUOTE)
		{
			if (quote == SQUOTE)
				quote = 0;
			else if (quote == 0)
				quote = SQUOTE;
		}
		else if (!ignore_quotes && line[i] == DQUOTE)
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

char		*get_line()
{
	char	c;
	int		ret;
	char	*str;
	t_dstr	*line;

	line = ft_dstr_new(64);
	while (1)
	{
		if ((ret = readc(STDIN_FILENO, &c)) == -1)
		{
			ft_dstr_del(&line);
			return (NULL);
		}
		if (ret > 0)
			ft_dstr_add(line, c);
		if (c == '\n' || ret == 0)
		{
			str = line->str;
			free(line);
			return (str);
		}
	}
}

int			input_batch(const char *prompt, bool heredoc)
{
	char	*line;
	char	*tmp;

	(void)prompt;
	if (!(line = get_line()))
		exit(1);
	if (!*line)//ctrl-C || ctrl-D
	{
		free(line);
		if (!g_lexer.line)
			exit(0);//builtin_exit()
		return (INPUT_EOF);
	}
	if (g_lexer.line)
	{
		tmp = g_lexer.line;
		g_lexer.line = ft_strjoin(g_lexer.line, line);
		free(tmp);
	}
	else
		g_lexer.line = ft_strdup(line);
	remove_escaped_newlines(g_lexer.line, heredoc);
	free(line);
	return (0);
}

int			input_interactive(const char *prompt, bool heredoc)
{
	char	*line;
	char	*tmp;

	if (!(line = readline(prompt)))
		exit(1);//error()
	if (!*line)//ctrl-C || ctrl-D
	{
		free(line);
		if (g_rl_last_ret == RL_INTERRUPT)
		{
			g_parser.status = USER_ABORT;
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
	if (g_lexer.line)
	{
		tmp = g_lexer.line;
		g_lexer.line = ft_strjoin(g_lexer.line, line);
		free(tmp);
	}
	else
		g_lexer.line = ft_strdup(line);
	remove_escaped_newlines(g_lexer.line, heredoc);
	free(line);
	return (0);
}

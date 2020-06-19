/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fratajcz <fratajcz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/21 19:37:33 by fratajcz          #+#    #+#             */
/*   Updated: 2020/05/29 22:27:29 by fratajcz         ###   ########.fr       */
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
		if (line[i] == BSLASH && (ignore_quotes ? 1 : quote != SQUOTE)
				&& line[i + 1])
		{
			i++;
			if (line[i] == '\n')
				ft_memmove(&line[i - 1], &line[i + 1],
						ft_strlen(&line[i + 1]) + 1);
		}
		else if (!ignore_quotes && (line[i] == SQUOTE || line[i] == DQUOTE))
		{
			if (quote == NONE)
				quote = line[i];
			else if (quote == line[i])
				quote = NONE;
		}
		i++;
	}
	return (0);
}

char		*get_line(void)
{
	char	c;
	int		ret;
	char	*str;
	t_dstr	*line;

	line = ft_dstr_new(64);
	c = 0;
	while (1)
	{
		if ((ret = readc(g_shell.stdin, &c)) == -1)
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
	if (!*line)
	{
		free(line);
		if (!g_lexer.line)
			builtin_exit(NULL, NULL);
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
	normalize_lexer_line();
	free(line);
	return (0);
}

static int	handle_ctrl_c_d(void)
{
	if (g_rl_last_ret == RL_INTERRUPT)
	{
		g_parser.status = USER_ABORT;
		ft_memdel((void **)&g_lexer.line);
	}
	if (g_rl_last_ret == RL_EOF)
		if (!g_lexer.line)
			builtin_exit(NULL, NULL);
	return (g_rl_last_ret == RL_EOF ? INPUT_EOF : INPUT_INT);
}

int			input_interactive(const char *prompt, bool heredoc)
{
	char	*line;
	char	*tmp;

	if (!(line = readline(prompt)))
		exit(1);
	if (!*line)
	{
		free(line);
		return (handle_ctrl_c_d());
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
	normalize_lexer_line();
	free(line);
	return (0);
}

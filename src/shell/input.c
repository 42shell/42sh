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

/*
** EOF in batch mode /!\ currently quit with mo error msg
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

int			input_batch(const char *prompt)
{
	char	buff[BUFF_SIZE];
	char	*tmp;
	int		ret;

	(void)prompt;
	if ((ret = read(STDIN_FILENO, buff, BUFF_SIZE - 1)) > 0)
	{
		buff[ret] = 0;
		remove_escaped_newlines(buff);
		if (g_lexer.line)
		{
			tmp = g_lexer.line;
			g_lexer.line = ft_strjoin(g_lexer.line, buff);
			free(tmp);
		}
		else
			g_lexer.line = ft_strdup(buff);
	}
	else if (ret == 0) //heredoc with no end
	{
		free(g_lexer.line);
		g_lexer.line = NULL;
		g_parser.error = SILENT_ABORT;
		exit(0);
	}
	else if (ret == -1)
		exit(1);//error
	return (0);
}

int			input_interactive(const char *prompt)
{
	char	*line;
	char	*tmp;

	if (!(line = readline(prompt)))
		exit(1);//error()
	else if (!*line)//ctrl-C || ctrl-D
	{
		free(line);
		g_parser.error = SILENT_ABORT;
		if (g_rl_last_ret == RL_EOF)
		{
			if (!g_lexer.line)
				exit(0);//builtin_exit()
		}
		ft_memdel((void **)&g_lexer.line);
		return (g_rl_last_ret == RL_EOF ? INPUT_EOF : INPUT_INT);
	}
	remove_escaped_newlines(line);
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

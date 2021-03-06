/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fratajcz <fratajcz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/08 19:46:45 by fratajcz          #+#    #+#             */
/*   Updated: 2020/04/10 00:08:46 by fratajcz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

void		print_command(t_command *command)
{
	t_dstr	*buf;

	buf = ft_dstr_new(128);
	format_command(buf, command);
	ft_dprintf(2, "%s\n", buf->str);
}

int			get_required_reserv_word(int expect_type)
{
	int		type;

	if (!g_parser.token)
		return (0);
	if ((int)g_parser.token->type == expect_type)
		return (expect_type);
	if ((type = is_reserved_word(g_parser.token->value->str))
	&& type == expect_type)
		return (g_parser.token->type = type);
	return (0);
}

/*
** heredoc utils
*/

void		add_heredoc(t_token *heredoc)
{
	t_token		*ptr;

	if (!g_parser.heredocs)
		g_parser.heredocs = heredoc;
	else
	{
		ptr = g_parser.heredocs;
		while (ptr->next)
			ptr = ptr->next;
		ptr->next = heredoc;
	}
}

int			handle_heredoc_eof(char *delim)
{
	ft_dprintf(2,
	"42sh: warning: here-document delimited by end-of-file (wanted '%.*s')\n",
	(int)(ft_strlen(delim) - 1), delim);
	g_parser.status = NOERR;
	return (0);
}

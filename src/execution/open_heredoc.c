/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   open_heredoc.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fratajcz <fratajcz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/29 14:38:47 by fratajcz          #+#    #+#             */
/*   Updated: 2020/06/17 04:54:36 by fratajcz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

char	*g_tmp_file = NULL;

/*
** Ignores quote termination except when they are inside expansions
*/

static bool		braces_are_closed(const char *heredoc)
{
	t_array			*brack_stack;
	int				i;

	brack_stack = array_new(4);
	i = 0;
	while (heredoc[i])
	{
		if (heredoc[i] == '$' || brack_stack->size > 0)
			set_bracket_status(heredoc, i, brack_stack, true);
		i++;
	}
	i = brack_stack->size;
	if (i != 0)
		ft_dprintf(2, "42sh: bad substitution: no closing bracket in %s",
				heredoc);
	array_destroy(brack_stack);
	return (i == 0);
}

static t_token	*create_tmp_token(t_dstr *heredoc)
{
	t_token *tmp;

	tmp = ft_xmalloc(sizeof(t_token));
	tmp->value = heredoc;
	tmp->exp_info = ft_dstr_dup(heredoc);
	ft_memset(tmp->exp_info->str, '0', tmp->exp_info->len);
	return (tmp);
}

int				open_heredoc(t_dstr *heredoc)
{
	int		fd;
	t_token	*tmp;

	free(g_tmp_file);
	g_tmp_file = ft_mktemp(ft_strdup("/tmp/42sh_XXXXXX"));
	while (g_tmp_file != NULL && (fd = open(g_tmp_file, O_WRONLY)) == -1)
	{
		free(g_tmp_file);
		g_tmp_file = ft_mktemp(ft_strdup("/tmp/42sh_XXXXXX"));
	}
	if (g_tmp_file == NULL)
		return (-1);
	tmp = create_tmp_token(heredoc);
	if (!braces_are_closed(heredoc->str) || dollar_expand(tmp, 0, true) == 1)
	{
		ft_memdel((void **)&g_tmp_file);
		token_del(&tmp);
		return (ERROR_REDIR_EXPAND);
	}
	remove_bslash(heredoc);
	ft_putstr_fd(heredoc->str, fd);
	close(fd);
	token_del(&tmp);
	return (open(g_tmp_file, O_RDONLY));
}

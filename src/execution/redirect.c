/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fratajcz <fratajcz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/15 14:52:04 by fratajcz          #+#    #+#             */
/*   Updated: 2020/06/17 04:54:52 by fratajcz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

#define RIGHTS 0666

static t_list_head	**g_backup_list = NULL;

static int	get_flags(int operator_type)
{
	if (operator_type == GREAT || operator_type == GREATAND)
		return (O_CREAT | O_WRONLY | O_TRUNC);
	if (operator_type == DGREAT)
		return (O_CREAT | O_WRONLY | O_APPEND);
	if (operator_type == LESS || operator_type == LESSAND)
		return (O_RDONLY);
	if (operator_type == DLESS)
		return (O_RDWR | O_CREAT);
	return (0);
}

static int	get_redirection_fd(t_redir *redir)
{
	t_dstr	*filename;
	int		type;

	type = redir->operator->type;
	filename = redir->right_op->value;
	if (type == DLESS)
		return (open_heredoc(ft_dstr_dup(filename)));
	else if ((type == GREATAND || type == LESSAND)
	&& ft_strisnbr(filename->str))
		return (ft_atoi(filename->str));
	return (open(filename->str, get_flags(type), RIGHTS));
}

static int	redirect(t_redir *redir, int redirected_fd)
{
	int		redirection_fd;

	if (ft_strequ(redir->right_op->value->str, "-"))
	{
		dup2_and_backup(g_backup_list, -1, redirected_fd);
		close(redirected_fd);
		return (0);
	}
	else if ((redirection_fd = get_redirection_fd(redir)) < 0
	|| !is_valid_fd(*g_backup_list, redirection_fd))
	{
		if (redirection_fd == ERROR_REDIR_EXPAND)
			return (ERROR_REDIR_EXPAND);
		return (redirection_fd == -1 ? ERROR_REDIR_OPEN : ERROR_REDIR_BAD_FD);
	}
	if (redirected_fd == redirection_fd)
		move_fd(&redirection_fd);
	dup2_and_backup(g_backup_list, redirection_fd, redirected_fd);
	if (redir->operator->type != LESSAND && redir->operator->type != GREATAND)
		close(redirection_fd);
	return (0);
}

static int	get_redirected_fd(t_redir *redir)
{
	if (!redir->left_op)
	{
		if (redir->operator->type == LESS
		|| redir->operator->type == LESSAND
		|| redir->operator->type == DLESS)
			return (0);
		else
			return (1);
	}
	return (ft_atoi(redir->left_op->value->str));
}

extern char	*g_tmp_file;

int			set_redir(t_redir *redir_list, t_list_head **backup_list)
{
	int		redirected_fd;
	int		ret;

	g_backup_list = backup_list;
	while (redir_list)
	{
		if ((redirected_fd = get_redirected_fd(redir_list)) > 255)
		{
			restore_fds(backup_list);
			return (redir_error(ERROR_REDIR_BAD_FD));
		}
		else if ((ret = redirect(redir_list, redirected_fd)) < 0)
		{
			restore_fds(backup_list);
			return (redir_error(ret));
		}
		if (redir_list->operator->type == DLESS)
			unlink(g_tmp_file);
		redir_list = redir_list->next;
	}
	return (0);
}

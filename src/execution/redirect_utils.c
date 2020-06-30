/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fratajcz <fratajcz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/14 16:48:51 by fratajcz          #+#    #+#             */
/*   Updated: 2020/05/08 18:23:02 by fratajcz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

static t_fd_backup	*fd_used_for_backup(t_list_head *backup_list, int fildes)
{
	t_list_head *cur;

	if (backup_list == NULL)
		return (NULL);
	cur = backup_list->next;
	while (cur != backup_list)
	{
		if (((t_fd_backup *)cur->data)->backup == fildes)
			return (cur->data);
		cur = cur->next;
	}
	return (NULL);
}

bool				is_valid_fd(t_list_head *backup_list, int fd)
{
	struct stat	buf;

	return (!fd_used_for_backup(backup_list, fd) && fstat(fd, &buf) != -1);
}

void				move_fd(int *fd)
{
	int		new;

	new = dup(*fd);
	close(*fd);
	*fd = new;
}

int					dup2_and_backup(t_list_head **backup_list,
					int fd_from, int fd_to)
{
	t_fd_backup *backup_store;
	t_fd_backup *backup_to_move;

	if (!backup_list)
		return (-1);
	if (*backup_list == NULL)
		*backup_list = ft_list_first_head(NULL);
	backup_store = ft_xmalloc(sizeof(t_fd_backup));
	if ((backup_to_move = fd_used_for_backup(*backup_list, fd_to)) != NULL)
		move_fd(&backup_to_move->backup);
	backup_store->orig_number = fd_to;
	backup_store->backup = dup(fd_to);
	ft_list_add(backup_store, *backup_list);
	return (dup2(fd_from, fd_to));
}

/*
** if backup->backup == -1, it means the fd didn't exist before being redirected
** so we close it. (ex: ls 5> file opens file at fd 5)
*/

int					restore_fds(t_list_head **backup_list)
{
	t_list_head *cur;
	t_list_head *tmp;
	t_fd_backup *backup;

	if (!backup_list || !*backup_list)
		return (0);
	cur = (*backup_list)->next;
	while (!ft_list_empty(*backup_list))
	{
		backup = cur->data;
		if (backup->backup == -1)
			close(backup->orig_number);
		else
		{
			dup2(backup->backup, backup->orig_number);
			close(backup->backup);
		}
		free(cur->data);
		tmp = cur;
		cur = cur->next;
		ft_list_del(tmp);
	}
	ft_memdel((void **)backup_list);
	return (0);
}

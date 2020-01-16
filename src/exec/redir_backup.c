/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_backup.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fratajcz <fratajcz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/14 16:48:51 by fratajcz          #+#    #+#             */
/*   Updated: 2020/01/15 14:25:08 by fratajcz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

static t_list_head *g_backups = NULL;

t_fd_backup	*fd_used_for_backup(int fildes)
{
	t_list_head *cur;

	if (g_backups == NULL)
		return (NULL);
	cur = g_backups->next;
	while (!ft_list_is_last(cur, g_backups))
	{
		if (((t_fd_backup *)cur->data)->backup == fildes)
			return (cur->data);
		cur = cur->next;
	}
	if (((t_fd_backup *)cur->data)->backup == fildes)
		return (cur->data);
	return (NULL);
}

bool		is_valid_fd(int fd)
{
	struct stat	buf;

	return (fd < 256 && !fd_used_for_backup(fd) && fstat(fd, &buf) != -1);
}

static void	move_backup(t_fd_backup *backup)
{
	int	new_backup_fd;

	new_backup_fd = dup(backup->backup);
	close(backup->backup);
	backup->backup = new_backup_fd;
}

int			dup2_and_backup(int fd_from, int fd_to, bool backup)
{
	t_fd_backup *backup_store;
	t_fd_backup *backup_to_move;

	if (backup == false)
		return (dup2(fd_from, fd_to));
	if (g_backups == NULL)
		g_backups = ft_list_first_head(NULL);
	backup_store = ft_xmalloc(sizeof(t_fd_backup));
	if ((backup_to_move = fd_used_for_backup(fd_to)) != NULL)
		move_backup(backup_to_move);
	backup_store->orig_number = fd_to;
	backup_store->backup = dup(fd_to);
	ft_list_add(backup_store, g_backups);
	return (dup2(fd_from, fd_to));
}

/*
** if backup->backup == -1, it means the fd didn't exist before being redirected
** so we close it. (ex: ls 5> file opens file at fd 5)
*/

int			restore_fds(void)
{
	t_list_head *cur;
	t_list_head *tmp;
	t_fd_backup *backup;

	if (g_backups == NULL)
		return (0);
	cur = g_backups->next;
	while (!ft_list_empty(g_backups))
	{
		backup = cur->data;
		if (backup->backup == -1)
			close(backup->orig_number);
		else
			dup2(backup->backup, backup->orig_number);
		free(cur->data);
		tmp = cur;
		cur = cur->next;
		ft_list_del(tmp);
	}
}

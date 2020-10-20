/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_exec_path.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fratajcz <fratajcz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/15 09:08:47 by fratajcz          #+#    #+#             */
/*   Updated: 2020/05/25 02:46:23 by fratajcz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

t_ht	*g_binaries;

/*
** Add all messages sent from fork to hash table
*/

void		add_binary_msgs_to_hash(void)
{
	struct s_msgbuf	msg;
	size_t			cmd_len;
	char			*cmd_ptr;
	char			*path_ptr;

	while (msgrcv(g_msg_qid, &msg, MSG_SIZE, 1, IPC_NOWAIT) > 0)
	{
		cmd_ptr = msg.mtext;
		cmd_len = ft_strlen(msg.mtext);
		path_ptr = msg.mtext + cmd_len + 1;
		ht_put(g_binaries, cmd_ptr, ft_strdup(path_ptr));
	}
}

/*
** send a message containing command\0path
*/

static void	cache_msg_send(char *command, char *path)
{
	struct s_msgbuf	msg;
	size_t			cmd_len;
	size_t			path_len;

	cmd_len = ft_strlen(command);
	path_len = ft_strlen(path);
	msg.mtype = 1;
	ft_memcpy(msg.mtext, command, cmd_len);
	msg.mtext[cmd_len] = '\0';
	ft_memcpy(msg.mtext + cmd_len + 1, path, path_len);
	msg.mtext[cmd_len + path_len + 1] = '\0';
	msgsnd(g_msg_qid, &msg, cmd_len + path_len + 2, IPC_NOWAIT);
}

static char	*cache_search(char *command)
{
	char	*ret;

	if (!command)
		return (NULL);
	if (!g_binaries || !(ret = ht_get(g_binaries, command)))
		return (NULL);
	return (ret);
}

char		*get_exec_path(char *command, t_array *env)
{
	char		**path;
	char		*ret;
	int			i;
	struct stat b;

	i = 0;
	path = NULL;
	if ((ret = cache_search(command)))
		return (ft_strdup(ret));
	else if (ft_strchr(command, '/') && (stat(command, &b) == 0))
		ret = ft_strdup(command);
	else if ((path = split_path(get_env_var("PATH", env)))
	|| (path  = split_path(get_var_value("PATH"))))
	{
		while (path && path[i] && (ret = append_filename(path[i++], command)))
		{
			if (stat(ret, &b) == 0 && (S_IXUSR & b.st_mode)
					&& !S_ISDIR(b.st_mode))
				break ;
			ft_memdel((void **)&ret);
		}
		if (ret)
			cache_msg_send(command, ret);
	}
	free_arr(path);
	return (ret);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   argv.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fratajcz <fratajcz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/15 09:08:47 by fratajcz          #+#    #+#             */
/*   Updated: 2020/02/07 00:18:00 by fratajcz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

void		free_argv(t_argv *argv)
{
	free(argv->cmd_path);
	free_arr(argv->argv);
	free(argv);
}

static int	get_argc(t_node *cmd)
{
	int		argc;
	int		i;

	i = 0;
	argc = 0;
	while (i < cmd->nb_children)
	{
		if (node_token(cmd->child[i])->type == WORD)
			argc++;
		i++;
	}
	return (argc);
}

static char	**words_to_array(t_node *cmd)
{
	char	**array;
	int		i;
	int		j;

	i = -1;
	j = 0;
	array = ft_xmalloc((get_argc(cmd) + 1) * sizeof(char *));
	while (++i < cmd->nb_children)
	{
		if (node_token(cmd->child[i])->type == WORD)
			array[j++] = ft_strdup(node_token(cmd->child[i])->value->str);
	}
	array[j] = NULL;
	return (array);
}

t_argv		*get_argv(t_node *cmd, t_env *env)
{
	t_argv	*argv;

	argv = ft_xmalloc(sizeof(t_argv));
	argv->cmd_path = NULL;
	if ((argv->argv = words_to_array(cmd)) && argv->argv[0] != NULL)
	{
		if (is_builtin(argv->argv[0]))
			return (argv);
		argv->cmd_path = get_executable_path(argv->argv[0], env);
		return (argv);
	}
	free_argv(argv);
	set_redir(cmd, true);
	restore_fds();
	return (NULL);
}

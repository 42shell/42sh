/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fratajcz <fratajcz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/15 09:52:31 by fratajcz          #+#    #+#             */
/*   Updated: 2020/02/07 00:36:28 by fratajcz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

#define ERROR "42sh: an error has occured\n"


static void	interrupt_fork(int sig)
{
	if (sig == SIGINT)
		write(1, "\n", 1);
	g_last_exit_st = 130;
}

/*
int			exec_builtin(t_node *command_node, char **argv) //bool free_argv
{
	if (set_redir(cmd, true) > 0)
	{
		free_argv(argv);
		restore_fds();
		return (1);
	}
	if (ft_strequ(argv->argv[0], "env"))
		builtin_env(argv->argv, env);
	else if (ft_strequ(argv->argv[0], "exit"))
		builtin_exit(argv->argv);
	else if (ft_strequ(argv->argv[0], "unsetenv"))
		builtin_unsetenv(argv->argv, env);
	else if (ft_strequ(argv->argv[0], "setenv"))
		builtin_setenv(argv->argv, env);
	else if (ft_strequ(argv->argv[0], "echo"))
		builtin_echo(argv->argv);
	else if (ft_strequ(argv->argv[0], "cd"))
		builtin_cd(argv->argv, env);
	restore_fds();
	if (free_av)
		free_argv(argv);
	return (0);
}

int			exec_command_env(char **argv, t_env *env)
{
	pid_t		pid;
	int			status;

	pid = fork();
	signal(SIGINT, interrupt_fork);
	if (pid == 0)
	{
		if ((execve(argv[0], argv, env->env) == -1))
		{
			ft_dprintf(2, "env: %s: No such file or directory\n", argv[0]);
			exit(g_last_exit_st);
		}
		exit(0);
	}
	wait(&status);
	g_last_exit_st = WIFEXITED(status) ? WEXITSTATUS(status)
		: g_last_exit_st;
	return (g_last_exit_st);
}
*/

int			launch_process(t_argv *argv)
{
	(void)argv;
	return (0);
}

int			exec_command(t_command *command)
{
	pid_t		pid;
	int			status;
	t_argv		*argv;

	//if (is_builtin(argv[0])) //is_builtin(t_command *)
	//	return (exec_builtin(command, argv));
	if (!(argv = get_argv(command)))
		return (command_not_found(command->argv[0]->value->str));
	pid = fork();
	signal(SIGINT, interrupt_fork);
	if (pid == 0)
	{
		if (set_redir(command, false) > 0)
			exit(1);
		if (execve(argv->path, argv->argv, g_env->env) == -1)
		{
			ft_dprintf(2, "42sh: %s: cannot execute command\n", argv->argv[0]);
			exit(g_last_exit_st);
		}
		exit(0);
	}
	wait(&status);
	argv_del(&argv);
	g_last_exit_st = WIFEXITED(status) ? WEXITSTATUS(status) : g_last_exit_st;
	
	return (g_last_exit_st);
}

int			execute(t_node *node)  // bool bg || exec_bg
{
	int			i;

	i = 0;
	if (node == NULL)
		return (1);
	else if (node->type == NODE_COMMAND /*&& expand(node, env) == 0*/)
		return (exec_command((t_command *)node->data));
	else if (node->type == NODE_AND_IF)
	{
		if (execute(node->child[0]) == 0)
			return (execute(node->child[1]));
		return (1);
	}
	else if (node->type == NODE_OR_IF)
	{
		if (execute(node->child[0]) != 0)
			return (execute(node->child[1]));
		return (0);
	}
	/*
	else if (((t_token *)node->data)->type == PIPE)
		return (exec_pipe(node, env));
	while (i < node->nb_children)
		execute(node->child[i++], env);
	*/
	return (1);
}

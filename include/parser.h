/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fratajcz <fratajcz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/08 23:49:26 by fratajcz          #+#    #+#             */
/*   Updated: 2020/02/13 18:27:54 by fratajcz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_H
# define PARSER_H

# include "shell.h"

# define PARSE_ERROR			20 //?

enum							e_parse_error
{
	SILENT_ABORT = -1,
	NOERR,
	TOKENS_LEFT,
	NULL_AST,
	NULL_AST_NODE,
	NO_REDIR_FILENAME,
	NO_CMD_BEFORE_PIPE,
	NO_CMD_AFTER_PIPE,
	NO_CMD_AFTER_AND_OR,
	NO_CMD_AFTER_SEP,
	HEREDOC_NO_DELIM
};

/*
** -A single process is returned by ps_process()
*/

typedef struct					s_redir
{
	struct s_redir				*next;
	t_token						*left_op;
	t_token						*operator;
	t_token						*right_op;
}								t_redir;

typedef struct					s_process
{
	struct s_process			*next;
	t_token						*words;
	t_redir						*redirs;
	char						**argv;
	char						*path;
	int							stdin;
	int							stdout;
	pid_t						pid;
	int							status;
	bool						stopped;
	char						done;
}								t_process;

/*
** -A single pipeline is returned by ps_pipeline(),
**	it contains a list of processes
** -sep is the operator used to separate the pipeline from the next, '&&' or '||'
*/

typedef struct					s_pipeline
{
	struct s_pipeline			*next;
	struct s_process			*processes;
	int							sep;
}								t_pipeline;

/*
** -A single job is returned by get_job(),
**  it contains a list of pipelines returned by ps_and_or()
** -A list of jobs is returned by get_jobs_list() via ps_list()
*/

typedef struct					s_job
{
	struct s_job				*next;
	struct s_pipeline			*pipelines;
	pid_t						pgid;
	bool						notified;
	bool						bg;
}								t_job;

typedef struct					s_list
{
	struct s_list				*next;
	struct s_job				*jobs;
}								t_list;

/*
** typedef struct 				s_compound_command
** {
** 								;
** }
*/

/*
** parser:
** -heredocs is a pointer to the right operand token of the
**  first io_here redir struct.
**  The tokens are chained if there are more than 1 heredocs,
**  and they are unchained in get_all_heredocs to avoid segfaults in
**  token_del()
*/

typedef struct					s_parser
{
	t_token						*token;
	t_token						*heredocs;
	int							error;
}								t_parser;

t_parser						g_parser;

t_list							*ps_list(void);
t_job							*ps_job(void);
t_pipeline						*ps_pipeline(void);
t_process						*ps_process(void);
t_redir							*ps_io_redirect(void);
t_token							*ps_separator(void);
t_token							*ps_separator_op(void);
void							ps_newline_list(void);
void							ps_linebreak(int last_token_type);
int								ps_get_all_heredocs(void);

int								ps_error(char *near);
int								ps_heredoc_eof(char *delim);

int								list_del(t_list **list);
int								job_del(t_job **job);
int								pipeline_del(t_pipeline **pipeline);
int								process_del(t_process **processe);
int								redir_del(t_redir **redir);

void							print_jobs(t_job *jobs);

#endif

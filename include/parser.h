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

# define PARSE_ERROR	20 //?

enum					e_parse_error
{
	SILENT_ABORT = -1,
	NOERR,
	TOKENS_LEFT,
	NULL_AST,
	NULL_AST_NODE,
	NO_REDIR_FILENAME,
	NO_CMD_AFTER_PIPE,
	NO_CMD_AFTER_AND_OR,
	NO_CMD_AFTER_SEP,
	HEREDOC_NO_DELIM
};

/*
** -A single process is returned by simple command()
*/

typedef struct			s_redir
{
	struct s_redir		*next;
	t_token				*left_op;
	t_token				*operator;
	t_token				*right_op;
}						t_redir;

typedef struct			s_process
{
	struct s_process	*next;
	char				**argv;
	t_redir				*redirs;
	char				*path;
	pid_t				pid;
	int					status;
	bool				stopped;
	char				done;
}						t_process;

/*
** -A single pipeline is returned by pipe_sequence(),
**	it contains a list of processes
** -sep is the token used to separate the pipeline from the next, '&&' or '||'
*/

typedef struct			s_pipeline
{
	struct s_pipeline	*next;
	struct s_process	*processes; //pipeline
	//pid_t				pgid; ??
	t_token				*sep;
}						t_pipeline;

/*
** -A single job is returned by and_or(), it contains a list of pipelines
** -A list of jobs is returned by complete_command() and list()
** -sep is the token used to separate the job from the next, ';' or '&'
*/

typedef struct			s_job
{
	struct s_job		*next;
	struct s_pipeline	*pipelines;
	pid_t				pgid;
	bool				notified;	/* true if user told about stopped job */
	t_token				*sep;
}						t_job;

/*
** parser:
** -heredocs are pointers to right operands of '<<' redir structs
*/

typedef struct			s_parser
{
	t_token				*token;
	t_token				**heredocs;
	int					error;
}						t_parser;

t_parser				g_parser;

t_job					*complete_command(void);
t_job					*list(void);
t_job					*and_or(void);
t_pipeline				*pipe_sequence(void);
t_process				*simple_command(void);
t_redir					*io_redirect(void);
t_token					*separator(void);
t_token					*separator_op(void);
void					newline_list(void);
void					linebreak(int last_token_type);
void					get_all_heredocs(void);

int						parse_error(int code, char *near);
int						heredoc_eof(char *delim);

int						free_pipelines(t_pipeline **pipelines);
int						free_jobs(t_job **jobs);
int						free_redirs(t_redir **redirs);
int						free_processes(t_process **processes);

void					print_jobs(t_job *jobs);

#endif

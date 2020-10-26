/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fratajcz <fratajcz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/08 16:11:34 by fratajcz          #+#    #+#             */
/*   Updated: 2020/06/17 04:53:55 by fratajcz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WORD_EXPANSION_H
# define WORD_EXPANSION_H

# include "shell.h"

# define AMBIG_REDIR	2

int		expand_simple_cmd(t_simple_cmd *command);
int		expand_cmd(t_command *command);
int		expand_redir_list(t_redir *redir_list, char *home_dir);
int		dollar_expand(t_token *token, int start, bool heredoc);
int		arith_expand(t_token *token, int *i);
int		cmd_sub(t_token *token, int *i);
int		process_sub(t_token *token, int *i);
long	eval_expr(const char *str);
int		param_expand(t_token *token, int *i, bool brace);
void	path_expand(t_token *token);
t_array	*get_matches(char *path);
bool	is_match(char *str, char *pat, char quote, bool is_first_char);
bool	has_glob_char(char *str);

bool	quote_start(const char *str, int i, char *quote_status);
bool	quote_stop(const char *str, int i, char *quote_status);
void	remove_quotes(t_token *token);
void	remove_bslash(t_dstr *str);
void	sort_matches(char **array, int size);

int		get_end_of_braces(const char *str, int start);
void	token_replace_between(t_token *token, int start, int end,
									char *replacement);
void	split_fields(t_token *token);

t_redir	*redir_dup(t_redir *redir);
t_redir	*redir_list_dup(t_redir *list);
void	dup_command_args(t_simple_cmd *command);
void	dup_group_redir_list(t_command *command);

void	reset_subshell_globals(void);

bool	is_escaped_brack(const char *str, int i);

#endif

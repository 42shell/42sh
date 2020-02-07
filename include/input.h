/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fratajcz <fratajcz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/21 20:09:52 by fratajcz          #+#    #+#             */
/*   Updated: 2020/02/07 02:43:41 by fratajcz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef INPUT_H
# define INPUT_H

# include "shell.h"

# define BUFSIZE	256
# define CONTINUE	1
# define EOL		10
# define FILES		0
# define DIRONLY	1
# define EXECONLY	2

enum				e_keys
{
	K_UP,
	K_DOWN,
	K_LEFT,
	K_RIGHT,
	K_HOME,
	K_END,
	K_BSP,
	K_SPC,
	K_DEL,
	K_ENTER,
	K_ESC,
	K_NXTW,
	K_PRVW,
	K_REDRAW,
	K_CUTW,
	K_CUTA,
	K_CUTB,
	K_PAST,
	KEYS_NB
};

typedef struct		s_input
{
	struct s_term	*termp;
	unsigned long	keys[KEYS_NB];
	unsigned char	key[8];
	unsigned long	oldkey;
	t_list_head		*head;
	t_list_head		*curr;
	t_dstr			*line;
	t_dstr			*temp;
	t_dstr			*clip;
	const char		*last_prompt;
	size_t			pos;
	size_t			pos_min;
	bool			esc;
	bool			first_line;
	char			char_after_nl;
	bool			multiline;
	bool			interactive;
	bool			first_tab_press;
	bool			complete;
}					t_input;

typedef int			(*t_key_func)(t_input *);

int					init_input(t_input *input, struct s_term *term);
t_key_func			*init_functable(void);
char				*readline(t_input *input, const char *prompt);

int					addchar(t_input *input, uint8_t **bufp);
int					backspace(t_input *input);
int					delete(t_input *input);
int					enter(t_input *input);

int					move_left(t_input *input);
int					move_right(t_input *input);
int					move_home(t_input *input);
int					move_end(t_input *input);
int					move_nextword(t_input *input);
int					move_prevword(t_input *input);

int					history_up(t_input *input);
int					history_down(t_input *input);

int					cut_before(t_input *input);
int					cut_after(t_input *input);
int					cut_word(t_input *input);
int					paste(t_input *input);

int					draw_prompt(t_input *input, const char *prompt);
int					redraw(t_input *input);

int					cpos_to_prevline(t_input *input);
int					cpos_to_nextline(t_input *input);

int					ctrl_c(t_input *input);
int					ctrl_d(t_input *input);

int					escape(t_input *input, uint8_t **bufp);

int					rl_complete(t_input *input);
void				rl_put_match(t_input *input, char *partial, char *match);
void				rl_print_match_list(t_list_head *comp_list, char *partial,
					t_input *input);
t_list_head			*comp_get_command_list(char *partial, t_env *env);
t_list_head			*comp_get_file_list(char *partial, int flags);
char				*get_dir_to_search(char *partial);
bool				is_cd(char *str, int i);
bool				is_exec(char *path);
bool				is_dir(char *path);
void				free_comp_list(t_list_head **comp_list);

#endif

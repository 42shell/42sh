/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   readline.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fratajcz <fratajcz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/21 20:09:52 by fratajcz          #+#    #+#             */
/*   Updated: 2020/05/25 02:52:58 by fratajcz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef READLINE_H
# define READLINE_H

# include <unistd.h>
# include <termios.h>
# include <term.h>
# include <sys/ioctl.h>
# include "libft.h"

/*
** error macros
*/

# define RL_SYSCALL_ERROR	-1
# define RL_ALLOC_ERROR		-2
# define RL_TERM_ERROR		-3

/*
** ret macros
*/

# define RL_CONTINUE		0
# define RL_INTERRUPT		3
# define RL_EOF				4
# define RL_ENTER			10

/*
** autocomplete macros
*/

# define FILES				0
# define DIRONLY			1
# define EXECONLY			2

/*
** termcaps and keys for interactive mode
*/

/*
** caps in term.h
*/

enum						e_caps
{
	C_UP,
	C_DO,
	C_LE,
	C_ND,
	C_HO,
	C_CH,
	C_CE,
	C_CD,
	C_DC,
	C_SC,
	C_RC,
	C_CR,
	C_SF,
	C_SR,
	C_CM,
	C_CL,
	C_BL,
	C_KS,
	CAPS_SIZE
};

enum						e_keys
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
	K_CTRLC,
	K_CTRLD,
	K_TAB,
	KEYS_SIZE
};

/*
** term mode, backup, window size and cursor position
*/

extern struct termios				g_rl_term;
extern struct termios				g_rl_oldterm;
extern size_t						g_rl_sizex;
extern size_t						g_rl_sizey;
extern size_t						g_rl_posx;
extern size_t						g_rl_posy;

/*
** arrays (termcaps, keymap, key buffer)
*/

/*
** caps in term.h
*/

extern char						*g_rl_caps[CAPS_SIZE];
extern unsigned long				g_rl_keys[KEYS_SIZE];

struct						s_rl_key
{
	unsigned char			bytes[8];
	size_t					i;
};

extern struct s_rl_key				g_rl_key;
extern unsigned long				g_rl_oldkey;

/*
** function table
*/

typedef int					(*t_key_func)(void);
extern t_key_func					g_rl_keymap[255];

/*
** history list
*/

struct						s_rl_hist
{
	t_list_head				*head;
	t_list_head				*ptr;
};

extern struct s_rl_hist			g_rl_hist;

/*
** the line
** the backup is used for history
*/

struct						s_rl_line
{
	t_dstr					*dstr;
	t_dstr					*clipboard;
	t_dstr					*backup;
	size_t					i;
};

extern struct s_rl_line			g_rl_line;

/*
** the function used to autocomplete,
** takes the current line (word ?) as a parameter to get the context,
** returns an array of matches which will be displayed.
*/

typedef t_list_head			*(*t_complete_func)(char *context, size_t i,
							int *count, char *partial_word);
typedef	int					(*t_cmp_func)(void *a, void *b);

struct						s_rl_complete
{
	t_complete_func			get_matches;
	t_list_head				*matches;
	int						match_count;
	char					*old_partial_word;
};

extern struct s_rl_complete		g_rl_complete;

/*
** the prompt
*/

extern const char					*g_rl_prompt;
extern int							g_rl_prompt_len;

/*
** bool set to true when rl_init is called
*/

extern bool						g_rl_is_init;

/*
** error status, if set, readline will return NULL
** and all the allocated memory will be freed, history included.
*/

extern int							g_rl_error;

/*
** options
** -retain_newline: if set, the line is returned with a newline.
** -cr_prompt: if set, a newline will be output before writing the prompt
** -hist_doubl: if set, rl_history_add will store consecutive identical lines
** -line_limit: if set, readline will read, at max, line_size_max characters.
*/

extern bool						g_rl_retain_nl;
extern bool						g_rl_prompt_cr;
extern bool						g_rl_hist_doubl;
extern bool						g_rl_line_limit;
extern unsigned int				g_rl_line_size_max;

/*
** ret
*/

extern int							g_rl_last_ret;

/*
** initialization
*/

int							rl_init_term(void);
int							rl_bind_keys(void);

/*
** user functions
*/

char						*readline(const char *prompt);
int							rl_add_history(char *str);
int							rl_del(void);

/*
** key functions
*/

int							rl_insert_char(void);
int							rl_backspace(void);
int							rl_delete(void);
int							rl_enter(void);
int							rl_move_left(void);
int							rl_move_right(void);
int							rl_move_prevw(void);
int							rl_move_nextw(void);
int							rl_move_home(void);
int							rl_move_end(void);
int							rl_cut_before(void);
int							rl_cut_after(void);
int							rl_cut_word(void);
int							rl_paste(void);
int							rl_history_up(void);
int							rl_history_down(void);
int							rl_complete(void);
int							rl_interrupt(void);
int							rl_eof(void);

/*
** autocomplete
*/

int							rl_complete(void);
void						rl_put_match(char *match, char *partial_word);
void						rl_print_match_list(char *partial_word);

/*
** utils
*/

int							rl_handle_esc_seq(void);
void						rl_print_prompt(const char *prompt);
void						rl_print_line(bool restore_i);
void						rl_print_line_from_i(bool restore_i);
void						rl_carriage_return(bool echo_missing_nl);
bool						y_n_prompt(void);

/*
** term.h
*/

int							tputstr(const char *str);
int							tputnstr(char *str, size_t n);
void						clearfromc(void);
void						movcnl(void);
void						movcleft(void);
void						movcright(void);
int							get_winsize(void);
int							get_cpos(void);

#endif

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nbousset <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/11 20:07:44 by nbousset          #+#    #+#             */
/*   Updated: 2019/04/15 01:08:47 by nbousset         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PRINTF_H
# define FT_PRINTF_H

# include <stdarg.h>
# include "libft.h"

# define BUFF_SIZE		48

/*
** Flags
*/

# define HASH_FLAG		0x1
# define ZERO_FLAG		0x2
# define MINUS_FLAG		0x4
# define EQU_FLAG		0x8
# define SPACE_FLAG 	0x10
# define PLUS_FLAG		0x20
# define PREC_FLAG		0x40
# define UP_FLAG		0x80

/*
** Length modifiers
*/

# define HH_LEN			0x1
# define H_LEN			0x2
# define L_LEN			0x4
# define LL_LEN			0x8
# define LD_LEN			0x10
# define J_LEN			0x20
# define Z_LEN			0x40

typedef struct			s_printf
{
	char				buff[BUFF_SIZE];
	size_t				buff_index;
	const char			*format_start;
	const char			*format;
	va_list				*args;
	int					ret;
	int					fd;
	uint16_t			flags;
	size_t				field_width;
	size_t				precision;
	uint8_t				len_modif;
	uint8_t				conv;
	char				*str;
}						t_printf;

/*
** Main
*/

int						ft_printf(const char *format, ...);
int						ft_dprintf(int fd, const char *format, ...);
int						pf_parse_arg(t_printf *pf);
void					pf_handle_arg(t_printf *pf);
void					pf_handle_color(t_printf *pf);

void					pf_pad_field_width(t_printf *pf);
void					pf_pad_precision(t_printf *pf);

/*
** Printers
*/

void					pf_print_int(t_printf *pf);
void					pf_print_uint(t_printf *pf, uint32_t base);
void					pf_print_string(t_printf *pf);

/*
** Buffer
*/

void					pf_bufferize(t_printf *pf, const char *data,
															size_t size);

/*
** Utils
*/

int						pf_init(t_printf *pf, const char *format,
														va_list *args, int fd);

int						pf_isflag(char c);
int						pf_ismodif(char c);
int						pf_isconv(char c);

void					pf_pad_field_width(t_printf *pf);
void					pf_pad_precision(t_printf *pf);

void					pf_del(t_printf *pf);
void					pf_error(t_printf *pf, char *message);
void					pf_format_error(t_printf *pf);

/*
** Old lib
*/

char					*ft_strprefix(char *str, char *prefix, int alloc);
char					*ft_strsuffix(char *str, char *suffix, int alloc);
void					ft_strtolower(char *str);
char					*ft_utoa_base(uintmax_t n, unsigned int base);
char					*ft_utoa(uintmax_t n);

#endif

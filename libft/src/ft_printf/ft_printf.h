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

typedef unsigned char	t_uint8;
typedef unsigned short	t_uint16;
typedef unsigned int	t_uint32;
typedef unsigned long	t_uint64;

typedef struct			s_printf
{
	char				buff[BUFF_SIZE];
	size_t				buff_index;
	const char			*format_start;
	const char			*format;
	va_list				*args;
	int					ret;
	int					fd;
	t_uint16			flags;
	size_t				field_width;
	size_t				precision;
	t_uint8				len_modif;
	t_uint8				conv;
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
void					pf_print_uint(t_printf *pf, t_uint32 base);
void					pf_print_double(t_printf *pf);
void					pf_print_ldouble(t_printf *pf);
void					pf_print_double_sci(t_printf *pf);
void					pf_print_ldouble_sci(t_printf *pf);
void					pf_print_double_hex(t_printf *pf);
void					pf_print_ldouble_hex(t_printf *pf);
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

int						pf_get_double_exp(double *f);
int						pf_get_ldouble_exp(long double *lf);
char					*pf_mantissa_to_hex(t_double *t_f, size_t precision);
char					*pf_lmantissa_to_hex(t_double *t_f, size_t precision);
void					print_inf_nan(t_printf *pf, t_double *t_f);

void					pf_del(t_printf *pf);
void					pf_error(t_printf *pf, char *message);
void					pf_format_error(t_printf *pf);

/*
** Old lib
*/

void					ft_ceil_double(double *fp, size_t precision);
void					ft_ceil_ldouble(long double *fp, size_t precision);
void					ft_frexpl(t_double *t_f, long double lf);
void					ft_frexp(t_double *t_f, double f);
char					*ft_ldtoa(long double lf, size_t precision);
char					*ft_strprefix(char *str, char *prefix, int alloc);
char					*ft_strsuffix(char *str, char *suffix, int alloc);
void					ft_strtolower(char *str);
char					*ft_utoa_base(uintmax_t n, unsigned int base);
char					*ft_utoa(uintmax_t n);

#endif

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nbousset <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/12 16:27:46 by nbousset          #+#    #+#             */
/*   Updated: 2020/02/14 17:23:00 by fratajcz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIBFT_H
# define LIBFT_H

# include <unistd.h>
# include <stdlib.h>
# include <string.h>
# include <stdbool.h>
# include <stdint.h>

/*
** -----------------------------Struct double-----------------------------
*/

# define DOUBLE_EXPONENT_MASK		0x7ff
# define DOUBLE_MANTISSA_MASK		0xfffffffffffff
# define DOUBLE_EXPONENT_MAX		0x7ff
# define DOUBLE_EXPONENT_SHIFT		0x3ff

# define LDOUBLE_EXPONENT_MASK		0x7fff
# define LDOUBLE_MANTISSA_MASK		0xffffffffffffffff
# define LDOUBLE_EXPONENT_MAX		0x7fff
# define LDOUBLE_EXPONENT_SHIFT		0x3fff

typedef struct			s_double
{
	double				f;
	long double			lf;
	uint8_t				neg;
	uint16_t			exponent;
	uint64_t			mantissa;
	int					real_exponent;
	uint8_t				is_denormalized;
	uint8_t				is_inf;
	uint8_t				is_nan;
}						t_double;

/*
** -----------------------------Struct list-----------------------------
*/

typedef struct			s_list_head
{
	struct s_list_head	*prev;
	struct s_list_head	*next;
	void				*data;
}						t_list_head;

t_list_head				*ft_list_first_head(void *data);
t_list_head				*ft_list_new_head(void *data);
void					ft_list_add(void *data, t_list_head *head);
void					ft_list_add_tail(void *data, t_list_head *head);
void					ft_list_del(t_list_head *head);
void					ft_list_replace(t_list_head *old, t_list_head *new);
int						ft_list_is_first(const t_list_head *list,
						const t_list_head *head);
int						ft_list_is_last(const t_list_head *list,
						const t_list_head *head);
int						ft_list_empty(const t_list_head *head);
void					ft_list_foreach(t_list_head *head,
						void (*f)(void *a, void *priv),
						void *priv);
void					ft_list_foreach_rev(t_list_head *head,
						void (*f)(void *a, void *priv), void *priv);
void					ft_list_split(t_list_head *head,
						t_list_head **a, t_list_head **b);
void					ft_list_sort(t_list_head **head,
						int (*cmp)(void *a, void *b));

/*
** --------------------------------Struct node-------------------------------
*/

typedef struct			s_node
{
	void				*data;
	int					nb_children;
	int					capacity;
	struct s_node		**child;
}						t_node;

t_node					*node_new(void *data);
int						node_add_child(t_node *parent, t_node *child);


/*
**--------------------------Struct array----------------------------------
*/

typedef struct			s_array
{
	void				**array;
	size_t				size;
	size_t				allocated;
}						t_array;


t_array					*array_new();
void					array_realloc(t_array *array);
void					array_append(t_array *array, void *data);
void					array_destroy(t_array *array);

/*
**------------------------------Hash table-------------------------------------
*/

typedef struct			s_pair
{
	char				*key;
	void				*value;
}						t_pair;

typedef struct			s_bucket
{
	size_t				size;
	t_pair				*pairs;
}						t_bucket;

typedef void(*t_ht_free_func)(void *value);

typedef struct			s_ht
{
	size_t				size;
	t_bucket			*buckets;
	t_ht_free_func		free_value;
}						t_ht;

typedef void(*t_ht_enum_func)(const char *key, const void *value,
								const void *obj);

t_ht					*ht_new(size_t size, t_ht_free_func free_value);
void					ht_delete(t_ht *map);
void					*ht_get(const t_ht *map, const char *key);
bool					ht_exists(const t_ht *map, const char *key);
void					ht_put(t_ht *map, const char *key, void *value);
int						ht_get_count(const t_ht *map);
int						ht_enum(const t_ht *map, t_ht_enum_func enum_func,
								const void *obj);

/*
** -------------------------Dynamic strings------------------------
*/

typedef struct			s_dstr
{
	char				*str;
	size_t				size;
	size_t				len;
}						t_dstr;

t_dstr					*ft_dstr_new(char *str, size_t len, size_t size);
void					ft_dstr_del(void **ptr, void *priv);
int						ft_dstr_add(t_dstr *dstr, char c);
int						ft_dstr_insert(t_dstr *dstr, size_t i, char *str,
						size_t len);
int						ft_dstr_append(t_dstr *dstr, char *str);
int						ft_dstr_remove(t_dstr *dstr, size_t i, size_t len);
int						ft_dstr_clear(t_dstr *dstr, size_t size);

/*
** -----------------------------Memory-----------------------------
*/

void					*ft_xmalloc(size_t size);
void					*ft_memalloc(size_t size);
void					*ft_memrealloc(void *ptr, size_t old_size,
														size_t new_size);
void					ft_bzero(void *m, size_t n);
void					*ft_memcpy(void *dst, const void *src, size_t n);
void					ft_memdel(void **m);
void					*ft_memset(void *m, char c, size_t len);
int						ft_memcmp(const void *s1, const void *s2, size_t n);
void					*ft_memmove(void *dst, const void *src, size_t len);

/*
** -----------------------------Strings-----------------------------
*/

char					*ft_strnew(size_t size);
char					*ft_strdup(const char *s1);
char					*ft_strcpy(char *dst, const char *src);
int						ft_strcmp(const char *s1, const char *s2);
char					ft_strequ(const char *s1, const char *s2);
char					*ft_strchr(const char *str, int c);
char					*ft_strrchr(const char *s, int c);
char					*ft_strtrim(char *str);
char					*ft_strtok(char *str, const char *sep);
int						ft_strbrkt(char *str, int len);
void					ft_strremove(char *str, size_t len);
void					ft_strinsert(char *str, char *insert, ssize_t len);
char					*ft_strstr(const char *haystack, const char *needle);
char					*ft_strsub(char const *s, unsigned int start,
																size_t len);
char					*ft_strjoin(const char *s1, const char *s2);
int						ft_strquot(char *str);
bool					ft_strisnbr(char *str);
char					*ft_strcat(char *s1, const char *s2);
size_t					ft_strnlen(const char *s, size_t maxlen);
char					*ft_strndup(const char *s1, size_t n);

void					ft_putstr_fd(const char *str, int fd);
void					ft_putendl(char const *s);

/*
** -----------------------------Char-----------------------------
*/

int						ft_isprint(uint32_t c);
int						ft_iswhitespace(int c);
int						ft_isctrl(uint32_t c);
int						ft_isalpha(int c);
int						ft_isdigit(int c);
int						ft_isalnum(int c);
bool					ft_isblank(char c);
bool					ft_isquote(char c);

/*
** -----------------------------Length-----------------------------
*/

size_t					ft_charlen(uint8_t c);
size_t					ft_charlen_rev(char *c);
size_t					ft_strlen(const char *str);
size_t					ft_nbrlen(long n, uint32_t base);
size_t					ft_strarray_len(char **array);

/*
** -----------------------------Math-----------------------------
*/

double					ft_power(double nb, double pwr);
char					ft_isprime(long n);
long					ft_nextprime(long n);
unsigned int			ft_next_power_of_two(unsigned int v);

/*
** -----------------------------Conv-----------------------------
*/

uint32_t				ft_base_isvalid(char *base);
long					ft_atoi(const char *str);
long					ft_atoi_base(const char *nbr, uint32_t base);
char					*ft_itoa(long n);
char					*ft_itoa_base(long n, uint32_t base);

/*
** -----------------------------Old lib-----------------------------
*/

char					*ft_strprefix(char *str, char *prefix, int alloc);
char					*ft_strsuffix(char *str, char *suffix, int alloc);
void					ft_strtolower(char *str);
char					*ft_utoa_base(uintmax_t n, unsigned int base);
char					*ft_utoa(uintmax_t n);
int						ft_dtoc_36(int d);
int						ft_islower(int c);
int						ft_isupper(int c);
void					ft_putstr(const char *s);
char					*ft_strpad(char *str, size_t field_width,
						int side, char c);

#endif

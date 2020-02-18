/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nbousset <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/12 16:27:46 by nbousset          #+#    #+#             */
/*   Updated: 2020/01/03 15:32:07 by fratajcz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIBFT_H
# define LIBFT_H

# include <unistd.h>
# include <stdlib.h>
# include <string.h>
# include <stdbool.h>

typedef unsigned char	t_uint8;
typedef unsigned short	t_uint16;
typedef unsigned int	t_uint32;
typedef unsigned long	t_uint64;

/*
** -----------------------------Struct buffer-----------------------------
*/

typedef struct			s_buff
{
	unsigned char		*bytes;
	size_t				size;
	size_t				i;
	void				(*flush_func)(struct s_buff *buff);
	int					fd;
}						t_buff;

t_buff					*ft_buff_new(size_t buff_size, int fd,
									void (*flush_func)(t_buff *buff));
void					ft_bufferize(t_buff *buff, const char *data,
															size_t data_size);
void					ft_bufferize_nbr(t_buff *buff, long nbr, t_uint32 base,
																	int prefix);
void					ft_buff_flush(t_buff *buff);
void					ft_buff_del(t_buff *buff);

/*
** -----------------------------Struct t_list-----------------------------
*/

typedef struct			s_list
{
	void				*data;
	struct s_list		*prev;
	struct s_list		*next;
}						t_list;

t_list					*ft_lstnew(void *data);
void					ft_lstdel(t_list *head, void (*del)(void **));
void					ft_lstdelone(t_list *head, void (*del)(void **));
void					ft_lstadd(t_list *head, t_list *new);

/*
** -------------------------Dynamic strings------------------------
*/

typedef struct			s_dstr
{
	char				*str;
	size_t				size;
	size_t				len;
}						t_dstr;

t_dstr					*ft_dstrnew(size_t size);
void					ft_dstrdel(void **ptr);
int						ft_dstradd(t_dstr *dstr, char c);
int						ft_dstrinsert(t_dstr *dstr, size_t i, char *str,
						size_t len);
int						ft_dstrcat(t_dstr *dstr, char *str);
t_dstr					*ft_dstrdup(t_dstr *dstr);
t_dstr					*ft_dstrfromstr(char *str);
int						ft_dstrremove(t_dstr *dstr, size_t i, size_t len);
int						ft_dstrclear(t_dstr *dstr, size_t size);

/*
** ------------------------------Trie-----------------------------
*/

typedef struct			s_trie
{
	struct s_trie		*parent;
	struct s_trie		**nodes;
	int					nodes_count;
	int					nodes_size;
	unsigned char		character;
	bool				isword;
	void				*data;
	long				flags;
}						t_trie;

t_trie					*ft_trie_new(unsigned char character, void *data,
						bool isword, long flags);
int						ft_trie_get_index(t_trie *trie,
						unsigned char character);
void					ft_trie_addnode(t_trie *root, t_trie *node);
void					ft_trie_addentry(t_trie *root, unsigned char *word,
						void *data, long flags);
t_trie					*ft_trie_getentry(t_trie *root, unsigned char *word);
t_trie					*ft_trie_getnode(t_trie *root, unsigned char *word);
void					ft_trie_del(t_trie **root);
t_trie					*ft_trie_delentry(t_trie *trie, unsigned char *word);

/*
** -----------------------------Memory-----------------------------
*/

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
int						ft_strnequ(char const *s1, char const *s2, size_t n);
char					*ft_strchr(const char *str, int c);
char					*ft_strtrim(char *str);
char					*ft_strstr(const char *haystack, const char *needle);
char					*ft_strsub(char const *s, unsigned int start,
																size_t len);
char					*ft_strjoin(const char *s1, const char *s2);
char					*ft_strcat(char *s1, const char *s2);
bool					ft_strisnbr(char *str);
char					*ft_strmerge(char *str, char *suffix,
						bool free_str, bool free_suffix);

/*
** -----------------------------Char-----------------------------
*/

int						ft_isupper(int c);
int						ft_isprint(t_uint32 c);
int						ft_iswhitespace(int c);
int						ft_isctrl(t_uint32 c);
int						ft_isalpha(int c);
int						ft_isdigit(int c);
int						ft_isalnum(int c);
bool					ft_isblank(char c);
bool					ft_isquote(char c);

/*
** -----------------------------Put-------------------------------
*/

int						ft_putc(int c);
int						ft_putstr(char *str);

/*
** -----------------------------Length-----------------------------
*/

size_t					ft_charlen(t_uint8 c);
size_t					ft_charlen_rev(char *c);
size_t					ft_strlen(const char *str);
size_t					ft_nbrlen(long n, t_uint32 base);

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

t_uint32				ft_base_isvalid(char *base);
long					ft_atoi(const char *str);
long					ft_atoi_base(const char *nbr, t_uint32 base);
char					*ft_itoa(long n);
char					*ft_itoa_base(long n, t_uint32 base);

#endif

NAME = libft.a
CC = gcc
CFLAGS = -Wall -Wextra -Werror
DIR_SRC = src/
DIR_OBJ = obj/
DIR_INC = inc/
SRC =	mem/ft_memalloc.c \
		mem/ft_memrealloc.c \
		mem/ft_bzero.c \
		mem/ft_memcpy.c \
		mem/ft_memdel.c \
		mem/ft_memset.c \
		mem/ft_memcmp.c \
		mem/ft_memmove.c \
		str/ft_strnew.c \
		str/ft_strdup.c \
		str/ft_strcpy.c \
		str/ft_strcmp.c \
		str/ft_strequ.c \
		str/ft_strchr.c \
		str/ft_strtrim.c \
		str/ft_strstr.c \
		str/ft_strsub.c \
		str/ft_strjoin.c \
		str/ft_strisnbr.c \
		str/ft_strcat.c \
		str/ft_strnlen.c \
		str/ft_strnequ.c \
		str/ft_strmerge.c \
		char/ft_isupper.c \
		char/ft_iswhitespace.c \
		char/ft_isprint.c \
		char/ft_isalpha.c \
		char/ft_isdigit.c \
		char/ft_isalnum.c \
		char/ft_isctrl.c \
		char/ft_isquote.c \
		char/ft_isblank.c \
		put/ft_putc.c \
		put/ft_putstr.c \
		len/ft_charlen.c \
		len/ft_charlen_rev.c \
		len/ft_strlen.c \
		len/ft_nbrlen.c \
		conv/ft_base_isvalid.c \
		conv/ft_atoi.c \
		conv/ft_atoi_base.c \
		conv/ft_itoa.c \
		conv/ft_itoa_base.c \
		math/ft_power.c \
		math/ft_isprime.c \
		math/ft_nextprime.c \
		math/ft_next_power_of_two.c \
		readline/autocomplete.c \
		readline/basic_features.c \
		readline/binding.c \
		readline/copypaste.c \
		readline/escape.c \
		readline/history.c \
		readline/interrupt.c \
		readline/move_basic.c \
		readline/move_spec.c \
		readline/print.c \
		readline/readline.c \
		readline/term_cursor.c \
		readline/term_display.c \
		readline/term_utils.c \
		readline/term.c \
		readline/utils.c \
		struct/buffer/ft_buff_new.c \
		struct/buffer/ft_bufferize.c \
		struct/buffer/ft_bufferize_nbr.c \
		struct/buffer/ft_buff_flush.c \
		struct/buffer/ft_buff_del.c \
		struct/list/ft_lstnew.c \
		struct/list/ft_lstadd.c \
		struct/list/ft_lstdel.c \
		struct/list/ft_lstdelone.c \
		struct/dstr/ft_dstrnew.c \
		struct/dstr/ft_dstradd.c \
		struct/dstr/ft_dstrinsert.c \
		struct/dstr/ft_dstrcat.c \
		struct/dstr/ft_dstrdup.c \
		struct/dstr/ft_dstrfromstr.c \
		struct/dstr/ft_dstrremove.c \
		struct/dstr/ft_dstrdel.c \
		struct/dstr/ft_dstrclear.c \
		struct/trie/ft_trie_new.c \
		struct/trie/ft_trie_addnode.c \
		struct/trie/ft_trie_addentry.c \
		struct/trie/ft_trie_getnode.c \
		struct/trie/ft_trie_getentry.c \
		struct/trie/ft_trie_delentry.c \
		struct/trie/ft_trie_del.c \
		struct/trie/ft_trie_get_index.c


OBJ = $(addprefix $(DIR_OBJ), $(SRC:.c=.o))

all : $(NAME)

$(NAME) : $(OBJ)
	ar -rcs $(NAME) $^

$(DIR_OBJ)%.o : $(DIR_SRC)%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@ -I$(DIR_INC)

clean :
	rm -rf $(OBJ)
	rm -rf $(DIR_OBJ)

fclean : clean
	rm -rf $(NAME)

re : fclean $(NAME)

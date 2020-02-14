NAME = libft.a
CC = gcc
CFLAGS = -Wall -Wextra -Werror
DIR_SRC = src/
DIR_OBJ = obj/
DIR_INC = inc/
SRC =	mem/ft_xmalloc.c \
		mem/ft_memalloc.c \
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
		str/ft_strtok.c \
		str/ft_strbrkt.c \
		str/ft_strquot.c \
		str/ft_strchr.c \
		str/ft_strremove.c \
		str/ft_strinsert.c \
		str/ft_strtrim.c \
		str/ft_strstr.c \
		str/ft_strsub.c \
		str/ft_strjoin.c \
		str/ft_strisnbr.c \
		str/ft_strndup.c \
		str/ft_strcat.c \
		str/ft_strnlen.c \
		str/ft_strnequ.c \
		str/ft_putstr_fd.c \
		str/ft_putendl.c\
		str/ft_strrchr.c\
		char/ft_iswhitespace.c \
		char/ft_isprint.c \
		char/ft_isalpha.c \
		char/ft_isdigit.c \
		char/ft_isalnum.c \
		char/ft_isctrl.c \
		char/ft_isquote.c \
		char/ft_isblank.c \
		len/ft_charlen.c \
		len/ft_charlen_rev.c \
		len/ft_strlen.c \
		len/ft_nbrlen.c \
		len/ft_strarray_len.c \
		conv/ft_base_isvalid.c \
		conv/ft_atoi.c \
		conv/ft_atoi_base.c \
		conv/ft_itoa.c \
		conv/ft_itoa_base.c \
		math/ft_power.c \
		math/ft_isprime.c \
		math/ft_nextprime.c \
		math/ft_next_power_of_two.c\
		struct/list/ft_list_first_head.c \
		struct/list/ft_list_new_head.c \
		struct/list/ft_list_add.c \
		struct/list/ft_list_add_tail.c \
		struct/list/ft_list_del.c \
		struct/list/ft_list_is_first.c \
		struct/list/ft_list_is_last.c \
		struct/list/ft_list_empty.c \
		struct/list/ft_list_foreach.c \
		struct/list/ft_list_foreach_rev.c \
		struct/list/ft_list_sort.c \
		struct/node/node.c\
		struct/dstr/ft_dstr_new.c \
		struct/dstr/ft_dstr_add.c \
		struct/dstr/ft_dstr_insert.c \
		struct/dstr/ft_dstr_remove.c \
		struct/dstr/ft_dstr_del.c \
		struct/dstr/ft_dstr_clear.c \
		struct/dstr/ft_dstr_append.c \
		struct/array/array.c \
		oldlib/ft_strprefix.c \
		oldlib/ft_strsuffix.c \
		oldlib/ft_strtolower.c \
		oldlib/ft_utoa_base.c \
		oldlib/ft_dtoc_36.c \
		oldlib/ft_islower.c \
		oldlib/ft_isupper.c \
		oldlib/ft_putstr.c \
		oldlib/ft_strpad.c \
		oldlib/ft_utoa.c \
		ft_printf/pf_print_int/pf_print_int.c \
		ft_printf/pf_print_uint/pf_print_uint.c \
		ft_printf/pf_print_string/pf_print_string.c \
		ft_printf/pf_utils/pf_del.c \
		ft_printf/pf_utils/pf_error.c \
		ft_printf/pf_utils/pf_format_error.c \
		ft_printf/pf_utils/pf_init.c \
		ft_printf/pf_utils/pf_isconv.c \
		ft_printf/pf_utils/pf_isflag.c \
		ft_printf/pf_utils/pf_ismodif.c \
		ft_printf/pf_utils/pf_pad.c \
		ft_printf/pf_utils/pf_buffer.c \
		ft_printf/pf_handlers/pf_handle_arg.c \
		ft_printf/pf_handlers/pf_handle_color.c \
		ft_printf/pf_parser/pf_parser.c \
		ft_printf/ft_printf.c


OBJ = $(addprefix $(DIR_OBJ), $(SRC:.c=.o))

all : $(NAME)

$(NAME) : $(OBJ)
	ar -rcs $(NAME) $^

$(DIR_OBJ)%.o : $(DIR_SRC)%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@ -Iinc

clean :
	rm -rf $(OBJ)
	rm -rf $(DIR_OBJ)

fclean : clean
	rm -rf $(NAME)

re : fclean $(NAME)

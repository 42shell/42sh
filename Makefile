NAME = 21sh 
CC = gcc 
CFLAGS = -Wall -Wextra $(DBG_FLAGS)# -Werror
LFLAGS = -ltermcap
DIR_SRC = src/
DIR_OBJ = obj/
DIR_INC = include/
DIR_LIB = libft/
SRC =		shell/main.c \
			shell/init.c \
			shell/del.c \
			shell/env_utils.c\
			shell/env_utils2.c\
			shell/path_utils.c\
			shell/utils.c\
			shell/get_opt.c\
			signal/init.c\
			signal/sig_action.c \
			signal/sig_handle.c \
			term/init.c \
			term/print.c \
			term/cursor.c \
			term/clear.c \
			term/window.c \
			input/init.c \
			input/move_basic.c \
			input/move_spec.c \
			input/move_utils.c \
			input/delchar.c \
			input/enter.c \
			input/draw.c \
			input/readline.c \
			input/copypaste.c \
			input/history.c \
			input/ctrl_c_d.c\
			input/escape.c\
			input/complete.c\
			input/comp_cmd.c\
			input/comp_files.c\
			input/comp_line_edit.c\
			input/comp_utils.c\
			exec/utils.c\
			exec/argv.c\
			exec/exec.c\
			exec/redir.c\
			exec/redir_utils.c\
			exec/pipe.c\
			lexer/init.c \
			lexer/token.c \
			lexer/append.c\
			lexer/delim.c \
			lexer/quote.c \
			lexer/eat.c \
			lexer/utils.c\
			parser/parser.c\
			parser/pipeline.c\
			parser/redirect.c\
			parser/utils.c\
			parser/heredoc.c\
			expansion/expand.c\
			expansion/param_exp.c\
			expansion/quotes.c\
			builtins/env.c\
			builtins/builtins.c\
			builtins/cd.c\
			builtins/cd_utils.c

OBJ = $(addprefix $(DIR_OBJ), $(SRC:.c=.o))
LIB = $(addprefix $(DIR_LIB), libft.a)

all : $(NAME)

$(NAME) : $(OBJ)
	@$(MAKE) -C $(DIR_LIB)
	$(CC) $(CFLAGS) -o $(NAME) $^ $(LIB) $(LFLAGS)

$(DIR_OBJ)%.o : $(DIR_SRC)%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@ -I$(DIR_INC) -I$(DIR_LIB)inc

clean :
	@$(MAKE) clean -C $(DIR_LIB)
	rm -rf $(DIR_OBJ)

fclean : clean
	@$(MAKE) fclean -C $(DIR_LIB)
	rm -rf $(NAME)

re : fclean $(NAME)

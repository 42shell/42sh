######################################################################
#                            DEFINITIONS                             #
######################################################################

#------------------------------------------------#
#                     PROJECT                    |
#------------------------------------------------#

#possible values: debug, release
BUILDTYPE       := debug
NAME            := 42sh
PROJECT         := 42sh

#------------------------------------------------#
#                   LIBRARIES                    |
#------------------------------------------------#

LIB_FT_DIR      := libft
LIB_FT_INC      := inc
LIB_FT_INC      := $(addprefix -I$(LIB_FT_DIR)/, $(LIB_FT_INC))
LIB_FT_NAME     := libft.a
LIB_FT          := $(LIB_FT_DIR)/$(LIB_FT_NAME)
LIB_FT_FLAGS    := -lft
LIB             := $(LIB_FT)
LDFLAGS         := -L$(LIB_FT_DIR)
LDLIBS          := $(LIB_FT_FLAGS) $(LIB_HT_FLAGS)
LFLAGS          := -ltermcap

#------------------------------------------------#
#                     FLAGS                      |
#------------------------------------------------#

CC              := gcc
CFLAGS          := -Wall -Wextra
ifeq ($(BUILDTYPE), debug)
	CFLAGS := $(CFLAGS) $(DBG_FLAGS)
endif
PFLAGS          := -Iinclude $(LIB_FT_INC)

#------------------------------------------------#
#                    SOURCES                     |
#------------------------------------------------#

SRC_BUILTINS    := builtins.c\
                   cd.c\
                   cd_utils.c\
                   env.c\
				   export.c
SRC_BUILTINS    := $(addprefix builtins/,$(SRC_BUILTINS))

SRC_EXEC        := argv.c\
                   exec.c\
                   open_heredoc.c\
                   pipe.c\
                   redir.c\
                   redir_utils.c\
                   utils.c
SRC_EXEC        := $(addprefix exec/,$(SRC_EXEC))

SRC_EXPANSION   := expand.c\
                   param_exp.c\
                   quotes.c\
				   path_exp.c\
				   is_match.c\
				   sort_matches.c
SRC_EXPANSION   := $(addprefix expansion/,$(SRC_EXPANSION))

SRC_INPUT       := comp_cmd.c\
                   comp_files.c\
                   comp_line_edit.c\
                   comp_utils.c\
                   complete.c\
                   copypaste.c\
                   ctrl_c_d.c\
                   delchar.c\
                   draw.c\
                   enter.c\
                   escape.c\
                   history.c\
                   init.c\
                   move_basic.c\
                   move_spec.c\
                   move_utils.c\
                   readline.c
SRC_INPUT       := $(addprefix input/,$(SRC_INPUT))

SRC_LEXER       := append.c\
                   delim.c\
                   eat.c\
                   init.c\
                   quote.c\
                   token.c\
                   utils.c
SRC_LEXER       := $(addprefix lexer/,$(SRC_LEXER))

SRC_PARSER      := heredoc.c\
                   parser.c\
                   pipeline.c\
                   redirect.c\
                   utils.c
SRC_PARSER      := $(addprefix parser/,$(SRC_PARSER))

SRC_SHELL       := del.c\
                   env_utils.c\
                   env_utils2.c\
                   ft_mktemp.c\
                   get_opt.c\
                   init.c\
                   main.c\
                   path_utils.c\
				   variables.c\
                   utils.c
SRC_SHELL       := $(addprefix shell/,$(SRC_SHELL))

SRC_SIGNAL      := init.c\
                   sig_action.c\
                   sig_handle.c
SRC_SIGNAL      := $(addprefix signal/,$(SRC_SIGNAL))

SRC_TERM        := clear.c\
                   cursor.c\
                   init.c\
                   print.c\
                   window.c
SRC_TERM        := $(addprefix term/,$(SRC_TERM))

SRC_PATH        := src
SRC_NAME        := $(SRC_BUILTINS)\
                   $(SRC_EXEC)\
                   $(SRC_EXPANSION)\
                   $(SRC_HISTORY)\
                   $(SRC_INPUT)\
                   $(SRC_LEXER)\
                   $(SRC_PARSER)\
                   $(SRC_SHELL)\
                   $(SRC_SIGNAL)\
                   $(SRC_TERM)
SRC             := $(addprefix $(SRC_PATH)/,$(SRC_NAME))

OBJ_PATH        := obj
OBJ_NAME        := $(SRC_NAME:.c=.o)
OBJ             := $(addprefix $(OBJ_PATH)/,$(OBJ_NAME))

INCDIR          := ./include

#------------------------------------------------#
#                     BUILD                      |
#------------------------------------------------#

BUILD_PATH      := build
NAME            := $(NAME)
OBJ             := $(addprefix $(BUILD_PATH)/,$(OBJ))

#------------------------------------------------#
#                     EXTRA                      |
#------------------------------------------------#

CLEAR           := "\033[0K\n\033[F"
CR              := "\r"$(CLEAR)
EOC             := "\033[0;0m"
RED             := "\033[0;31m"
GREEN           := "\033[0;32m"
BASENAME        := `basename $(PWD)`

######################################################################
#                               RULES                                #
######################################################################

#------------------------------------------------#
#                 BUILD-RULES                    |
#------------------------------------------------#

all: $(NAME)

$(NAME): $(OBJ) $(LIB) 
	@printf $(CR)$(GREEN)"[ $(PROJECT): All object files created ]"$(EOC)"\n"
	@$(CC) -o $(NAME) $(OBJ) $(LDFLAGS) $(LDLIBS) $(LFLAGS) $(CFLAGS)
	@printf $(CR)$(GREEN)"[ $(PROJECT): $(NAME) created ]\n"$(EOC)

$(BUILD_PATH)/$(OBJ_PATH)/%.o: $(SRC_PATH)/%.c
	@mkdir -p $(dir $@) 2>/dev/null || true
	@$(CC) $(CFLAGS) $(PFLAGS) -c $< -o $@
	@printf $(CR)"[ $(PROJECT): %s ]"$(CLEAR) $@

#------------------------------------------------#
#                 LIBRARY-RULES                  |
#------------------------------------------------#

### LIBFT

$(LIB_FT): FORCE
	$(MAKE) -C $(LIB_FT_DIR)

libft_clean:
	$(MAKE) -C $(LIB_FT_DIR) clean

libft_fclean:
	$(MAKE) -C $(LIB_FT_DIR) fclean

#------------------------------------------------#
#                  CLEAN-RULES                   |
#------------------------------------------------#

libclean: libft_clean

libfclean: libft_fclean

soft: clean all

re: fclean all

clean:
	@if [ -d $(BUILD_PATH)/$(OBJ_PATH) ]; then \
		$(RM) -r $(OBJ) \
		&& $(RM) -r $(BUILD_PATH)/$(OBJ_PATH) \
		&& printf $(CR)$(RED)"[ $(PROJECT): All object files cleaned ]\n"$(EOC); \
	fi

fclean: libfclean clean
	@if [ -e $(NAME) ]; then \
		$(RM) -r $(NAME) \
		&& printf $(CR)$(RED)"[ $(PROJECT): $(NAME) cleaned ]\n"$(EOC); \
	fi

#------------------------------------------------#
#                  OTHER-RULES                   |
#------------------------------------------------#

norme:
	@if [ -e norminette ]; then \
		norminette $(SRC) $(INCDIR) $(LIBFT_SRC) $(LIBFT_INC); \
	else \
		printf "norminette isn't installed\n"; \
	fi

check: $(NAME)
	./tests/test.sh

FORCE:

.SILENT:

.PHONY: all, clean, fclean, re, norme

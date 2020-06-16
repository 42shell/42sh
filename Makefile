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

ifeq ($(CC),cc)
CC              := clang
endif
CFLAGS          := -Wall -Wextra #-Werror
ifeq ($(BUILDTYPE), debug)
	CFLAGS := $(CFLAGS) $(DBG_FLAGS)
endif
PFLAGS          := -Iinclude $(LIB_FT_INC)

#------------------------------------------------#
#                    SOURCES                     |
#------------------------------------------------#

SRC_BUILTINS    :=	builtins.c\
					builtin_utils.c\
					cd.c\
					cd_utils.c\
					var_builtins.c\
					jobs.c\
					jobs_fgbg_utils.c\
					env.c\
				    exec_command_env.c\
					fgbg.c
SRC_BUILTINS    := $(addprefix builtins/,$(SRC_BUILTINS))

SRC_COMPLETE    := get_autocomplete_list.c\
                   get_command_list.c\
                   get_file_list.c\
                   utils.c
SRC_COMPLETE    := $(addprefix complete/,$(SRC_COMPLETE))

SRC_VARIABLES	:= env_dup.c\
                   env_utils.c\
				   export_env.c\
				   var_utils.c\
				   special_params.c\
				   variables.c
SRC_VARIABLES	:= $(addprefix variables/,$(SRC_VARIABLES))

SRC_EXEC        := eval_command.c\
				   eval_operator.c\
				   execute.c\
				   get_argv.c\
				   get_exec_path.c\
				   redirect.c\
				   redirect_utils.c\
				   redirect_error.c\
                   open_heredoc.c
SRC_EXEC        := $(addprefix execution/,$(SRC_EXEC))

SRC_JOB       	:= launch_job.c\
				   launch_process.c\
				   job_current.c\
				   job_control.c\
				   job_get_status.c\
				   job_set_status.c\
				   job_print.c\
				   job_format.c\
				   job_utils.c\
				   job_new_del.c
SRC_JOB			:= $(addprefix job_control/,$(SRC_JOB))

SRC_EXPANSION   := expand.c\
                   param_exp.c\
                   quotes.c\
				   is_match.c\
				   sort_matches.c\
				   has_glob_char.c\
				   path_exp.c
SRC_EXPANSION   := $(addprefix expansion/,$(SRC_EXPANSION))

SRC_LEXER       := append.c\
                   delim.c\
                   get_next_token.c\
                   quote.c\
                   token.c\
				   lx_end.c\
				   utils_operator.c\
                   utils.c
SRC_LEXER       := $(addprefix lexer/,$(SRC_LEXER))

SRC_PARSER      := and_or.c\
				   pipeline.c\
				   simple_command.c\
				   command.c\
				   complete_command.c\
				   compound_command.c\
				   group_command.c\
				   compound_list.c\
                   io_redirect.c\
				   separator.c\
				   heredoc.c\
				   error.c\
				   debug.c\
                   new_del.c\
				   utils.c
SRC_PARSER      := $(addprefix parser/,$(SRC_PARSER))

SRC_SHELL       := init.c\
                   main.c\
				   signal.c\
				   file_is_binary.c\
				   input.c
SRC_SHELL       := $(addprefix shell/,$(SRC_SHELL))

SRC_UTILS      := str_utils.c\
				  array_utils.c\
				  path_utils.c\
                  ft_mktemp.c\
                  get_opt.c
SRC_UTILS      := $(addprefix utils/,$(SRC_UTILS))

SRC_PATH        := src
SRC_NAME        := $(SRC_BUILTINS)\
				   $(SRC_COMPLETE)\
				   $(SRC_VARIABLES)\
                   $(SRC_EXEC)\
                   $(SRC_JOB)\
                   $(SRC_EXPANSION)\
                   $(SRC_HISTORY)\
                   $(SRC_LEXER)\
                   $(SRC_PARSER)\
                   $(SRC_SHELL)\
				   $(SRC_UTILS)
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

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
CFLAGS          := -Wall -Wextra 
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
					cd_utils2.c\
					export.c\
					set_unset.c\
					jobs.c\
					env.c\
				    exec_command_env.c\
					test.c\
					test_op1.c\
					test_op2.c\
					test_op3.c\
					test_op4.c\
					test_op5.c\
					hash.c\
					type.c\
					fgbg.c
SRC_BUILTINS    := $(addprefix builtins/,$(SRC_BUILTINS))

SRC_COMPLETE    := get_autocomplete_list.c\
                   get_command_list.c\
                   get_file_list.c\
				   get_option_list.c\
				   get_variable_list.c\
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
				   eval_compound.c\
				   eval_operator.c\
				   eval_script.c\
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
				   job_control.c\
				   job_status.c\
				   job_list.c\
				   update_jobs.c\
				   print_job.c\
				   format_job.c\
				   utils_job.c\
				   utils_process.c\
				   new_del.c\
				   wait.c
SRC_JOB			:= $(addprefix job_control/,$(SRC_JOB))

SRC_EXPANSION   := expand_simple_cmd.c\
				   expand_cmd.c\
                   dollar_exp.c\
                   param_exp.c\
				   arith_exp.c\
				   arith/eval.c\
				   arith/eval_arith_op.c\
				   arith/eval_logic_op.c\
				   arith/eval_rel_op.c\
				   arith/eval_assign_op.c\
				   arith/eval_plusplus_op.c\
				   arith/lexer.c\
				   arith/init_lexer.c\
				   arith/parse_expr1.c\
				   arith/parse_expr2.c\
				   arith/parse_factor.c\
				   arith/parser_utils.c\
				   arith/token_utils.c\
                   quotes.c\
				   is_match.c\
				   sort_matches.c\
				   has_glob_char.c\
				   field_splitting.c\
				   path_exp.c\
				   cmd_sub.c\
				   process_sub.c\
				   utils.c
SRC_EXPANSION   := $(addprefix expansion/,$(SRC_EXPANSION))

SRC_LEXER       := append.c\
                   delim.c\
                   get_next_token.c\
                   quote.c\
                   token.c\
				   lx_end.c\
				   utils_operator.c\
                   utils.c\
				   get_prompt.c
SRC_LEXER       := $(addprefix lexer/,$(SRC_LEXER))

SRC_PARSER      := and_or.c\
				   pipeline.c\
				   simple_command.c\
				   command.c\
				   complete_command.c\
				   compound_command.c\
				   group_command.c\
				   compound_list.c\
				   if_clause.c\
				   if_clause_else.c\
				   while_clause.c\
                   io_redirect.c\
				   separator.c\
				   heredoc.c\
				   error.c\
                   new_del.c\
				   format_command.c\
				   format_compound.c\
				   utils.c
SRC_PARSER      := $(addprefix parser/,$(SRC_PARSER))

SRC_SHELL       := init.c\
                   main.c\
				   signal.c\
				   utils.c\
				   input.c
SRC_SHELL       := $(addprefix shell/,$(SRC_SHELL))

SRC_UTILS      := str_utils.c\
				  array_utils.c\
				  path_utils.c\
                  ft_mktemp.c\
                  brack_stack.c\
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

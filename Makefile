SRC_FILES = 			\
src/wildcard_utils.c	\
src/exit_minishell.c	\
src/wildcard.c	\
src/main.c	\
src/exec/handle_redir.c	\
src/exec/main_execution.c	\
src/exec/heredoc.c	\
src/exec/exec_cmd.c	\
src/exec/builtin/echo.c	\
src/exec/builtin/env.c	\
src/exec/builtin/pwd.c	\
src/exec/builtin/exit.c	\
src/exec/builtin/unset.c	\
src/exec/builtin/export_utils.c	\
src/exec/builtin/export.c	\
src/exec/builtin/cd.c	\
src/exec/exec_logic.c	\
src/exec/utils.c	\
src/exec/utils2.c	\
src/utils/signals.c	\
src/utils/main_utils.c	\
src/utils/envp.c	\
src/parsing/tokenization/token.c	\
src/parsing/tokenization/check_if_sep.c	\
src/parsing/tokenization/str_tokinize.c	\
src/parsing/main_parsing.c	\
src/parsing/utils/quotes_utils.c\
src/parsing/utils/split_util.c	\
src/parsing/utils/expand_utils.c	\
src/parsing/utils/strjoin.c	\
src/parsing/utils/pro_lstnew.c	\
src/parsing/utils/tree_utils.c	\
src/parsing/utils/str_is_a_path.c	\
src/parsing/utils/strdup.c	\
src/parsing/utils/skip_quotes.c	\
src/parsing/utils/replace_value_in_token.c	\
src/parsing/utils/substr.c	\
src/parsing/utils/array_utils.c	\
src/parsing/parsing/parse_root.c	\
src/parsing/parsing/binaries.c	\
src/parsing/parsing/env_variables.c	\
src/parsing/parsing/parentheses.c	\
src/parsing/parsing/logical_operators.c	\
src/parsing/parsing/redirs.c	\
src/parsing/parsing/replace_args.c	\
src/parsing/parsing/quotes.c	\


INCLUDES = -I./includes -I./libft/includes -DREADLINE_LIBRARY -I$(HOME)/.local/lib/readline/8.2.1/include/readline

READLINE = -lreadline -lhistory -L$(HOME)/.local/lib/readline/8.2.1/lib

CC = cc

OBJ_FILES = ${SRC_FILES:%.c=%.o}

FLAGS = -Wall -Werror -Wextra $(INCLUDES)

NAME = minishell

%.o : %.c
	@$(CC) $(FLAGS) -c $^ -o $@

all : $(NAME)

WHITE_TEXT = \033[1;37m
RED_TEXT = \033[0;31m
NC_TEXT = \033[0m
GREEN_TEXT= \033[0;32m

./libft/libft.a :
	@echo "$(WHITE_TEXT)Archiving Libft..."
	@make -C ./libft


$(NAME) : $(OBJ_FILES) ./libft/libft.a
	@echo "$(WHITE_TEXT)Compiling Binary..."
	@$(CC) $(FLAGS) ${READLINE} $^ -o $(NAME)
	@echo "$(GREEN_TEXT)	Success : $(NAME) Compiled Successfully"
	@echo "$(WHITE_TEXT)Finished"
	@echo "$(NC_TEXT)"

TEST_OBJ = tests/test

dev : devflag $(NAME)

devflag :
	$(eval FLAGS := -Wall -Wextra -fsanitize=address $(INCLUDES))

lin : linuxflag $(NAME)

linuxflag :
	$(eval FLAGS := -Wall -Wextra -fsanitize=address -DLINUX $(INCLUDES))

clean :
	@echo "$(RED_TEXT)Cleaning $(NAME) Object Files"
	@rm -rf	$(OBJ_FILES) 
	@make -C ./libft clean
	@echo "$(NC_TEXT)"

fclean : clean
	@echo "$(RED_TEXT)Cleaning $(NAME) Binaries"
	@rm -rf $(NAME)
	@make -C ./libft fclean
	@echo "$(NC_TEXT)"

re : fclean all

bonus : all

.PHONY : clean fclean re all
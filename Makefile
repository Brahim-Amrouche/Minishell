SRC_FILES = ${wildcard ./src/**/*.c} ${wildcard ./src/*.c} ${wildcard ./src/**/**/*.c}

INCLUDES = -I./includes -I./libft/includes -DREADLINE_LIBRARY -I$(HOME)/.local/lib/readline/8.2.1/include/readline

READLINE = -lreadline -lhistory -L$(HOME)/.local/lib/readline/8.2.1/lib

CC = cc

OBJ_FILES = ${SRC_FILES:%.c=%.o}

FLAGS = -Wall -Werror -Wextra $(INCLUDES) -fsanitize=address

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

test : 
	gcc tests/test.c -o tests/test
	./tests/test

ctest :
	@cd tests && rm -rf	std1 std2 bash1 bash2 test

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

.PHONY : clean fclean re all
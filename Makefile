SRC_FILES = ${wildcard ./src/**/*.c} ${wildcard ./src/*.c}

INCLUDES = -I./includes -I./libft/includes

CC = cc

OBJ_FILES = ${SRC_FILES:%.c=%.o}

FLAGS = -Wall -Werror -Wextra -fsanitize=address $(INCLUDES)

NAME = minishell

%.o : %.c
	@$(CC) $(FLAGS) $(INCLUDES) -c $^ -o $@

all : $(NAME)

WHITE_TEXT = \033[1;37m
RED_TEXT = \033[0;31m
NC_TEXT = \033[0m
GREEN_TEXT= \033[0;32m

./libft/libft.a :
	@echo "$(WHITE_TEXT)Archiving Libft..."
	@make -C ./libft


$(NAME) : ./libft/libft.a $(OBJ_FILES)
	@echo "$(WHITE_TEXT)Compiling Binary..."
	@$(CC) $(FLAGS) $(INCLUDES) $^ -o $(NAME)
	@echo "$(GREEN_TEXT)	Success : $(NAME) Compiled Successfully"
	@echo "$(WHITE_TEXT)Finished"
	@echo "$(NC_TEXT)"

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

OBJ_FILES = ${SRC_FILES:%.c=%.o} 
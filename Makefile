### COMPILATION ###
CC = cc
CFLAGS = -Wall -Wextra -Werror

### INCLUDES ###
PROJECT = pipex
PROJECT_DIR = ./

SRC_DIR = src
SRC_FILE = parse_command.c \
			error.c \
			split_path.c \
			pipex_utils.c pipex.c
			   
### HEADER FILE ###
HEADER_DIR = includes

# LIBFT
FT_DIR = ./libft
FT = ft
FT_FLAG = -L$(FT_DIR) -l$(FT)

## OBJECT FILE ###
OBJ_DIR = .obj
OBJ_SRC = $(addprefix $(OBJ_DIR)/, $(SRC_FILE:.c=.o))

.PHONY=bonus

### RULES ###
all : $(PROJECT)

bonus : all

$(PROJECT) : $(OBJ_SRC)
	make -C $(FT_DIR)
	$(CC) -g3 $(CFLAGS) $(OBJ_SRC) -o $(PROJECT) $(FT_FLAG)

$(OBJ_DIR)/%.o : $(SRC_DIR)/%.c
	@mkdir -p $(@D)
	$(CC) -g3 $(CFLAGS) -I ./$(HEADER_DIR)  -c $< -o $@

fclean : clean
	rm -f $(PROJECT)
	make fclean -C $(FT_DIR)

#Suppresion des fichiers objet
clean :
	rm -f $(OBJ_DIR)/*.o
	@rm -df $(OBJ_DIR)/
#	rm -f $(BONUS_DIR)/$(OBJ_DIR)/*.o
#	@rm -df $(BONUS_DIR)/$(OBJ_DIR)/
	make clean -C $(FT_DIR)

re : fclean all

norminette :
	norminette $(FT_DIR) $(SRC_DIR) $(HEADER_DIR)
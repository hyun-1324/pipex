NAME = pipex

CC = cc
CFLAGS = -Wall -Wextra -Werror -g
RM = rm -f
INCLUDE = -Iinclude

SRCS_PATH = ./src/
LIBFT_PATH = ./libft/

LIBFT = $(LIBFT_PATH)libft.a
SRCS	= $(SRCS_PATH)parse_cmd_path.c \
		  $(SRCS_PATH)parse_cmd.c \
		  $(SRCS_PATH)utils.c \
		  $(SRCS_PATH)main.c \

OBJS = $(SRCS:.c=.o)

.SECONDARY: $(OBJS)

all : $(LIBFT) $(NAME)

$(NAME) : $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) $(LIBFT) -o $(NAME)

$(SRCS_PATH)%.o : $(SRCS_PATH)%.c
	$(CC) $(CFLAGS) $(INCLUDE) -c $< -o $@

$(LIBFT):
	@make -C $(LIBFT_PATH)

clean :
	$(RM) $(OBJS)
	@make clean -C $(LIBFT_PATH)

fclean : clean
	$(RM) $(NAME)
	@make fclean -C $(LIBFT_PATH)

re : fclean all

.PHONY : all clean fclean re

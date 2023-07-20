SRCS = pipex.c pipex_utils.c
SRCS_BONUS = pipex_bonus.c pipex_utils.c

OBJS = $(SRCS:.c=.o)
OBJS_BONUS = $(SRCS_BONUS:.c=.o)

EXEC = pipex
EXEC_BONUS = pipex

%.o: %.c
	gcc  -c $< -o $@ 

# __________ TARGETS  __________

all: $(EXEC) 

bonus: $(OBJS_BONUS) libft/libft.a
	gcc -Wall -Werror -Wextra $(OBJS_BONUS) -Llibft -lft -o $(EXEC_BONUS) -fsanitize=address -g3

$(EXEC): $(OBJS) libft/libft.a
	gcc -Wall -Werror -Wextra $(OBJS) -Llibft -lft -o $(EXEC) -fsanitize=address -g3

libft/libft.a:
	$(MAKE) -C libft

# __________ CLEANING __________

fclean: clean
	rm -rf $(EXEC) $(EXEC_BONUS)
	$(MAKE) -C libft fclean

clean:
	rm -rf $(OBJS) $(OBJS_BONUS)
	$(MAKE) -C libft clean

re: fclean all

.PHONY: libft/libft.a bonus








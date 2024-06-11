NAME	:= cub3D

CFLAGS	:= -Wextra -Wall -Werror -g -fsanitize=address -O1

LIBMLX	:= ./lib/MLX42

HEADERS	:= -I ./inc -I $(LIBMLX)/include -I ./lib/libft

LIBS	:= $(LIBMLX)/build/libmlx42.a -ldl -lglfw -pthread -lm
# LIBS	+= -L"/opt/homebrew/Cellar/glfw/3.4/lib/"

VPATH	:=	src \
			src/geometry \
			src/loop \
			src/map \
			src/hooks \
			src/player

SRCS	:=  main.c \
			loop.c \
			initMap.c \
			showMap.c \
			key.c \
			player.c

OBJDIR	:=	obj
OBJECTS	:=	$(addprefix $(OBJDIR)/, $(SRCS:.c=.o))

CC		:= cc

all: libmlx $(NAME)

libft:
	make -C ./lib/libft

libmlx:
	cmake $(LIBMLX) -B $(LIBMLX)/build && make -C $(LIBMLX)/build -j4

$(OBJDIR)/%.o: %.c
	@mkdir -p $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(NAME): $(OBJECTS) libft
	$(CC) $(CFLAGS) $(OBJECTS) $(LIBS) ./lib/libft/libft.a $(HEADERS) -o $(NAME)

clean:
	make -C ./lib/libft clean
	rm -rf $(OBJECTS)
	rm -rf $(LIBMLX)/build

fclean: clean
	make -C ./lib/libft fclean
	rm -rf $(NAME)

re: clean all

.PHONY: all, clean, fclean, re, libmlx

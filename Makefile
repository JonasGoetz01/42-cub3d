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
			src/player \
			src/3dview \
			src/utils

SRCS	:=  main.c \
			loop.c \
			initMap.c \
			showMap.c \
			key.c \
			player.c \
			geometry.c \
			resize.c \
			cursor.c \
			image.c \
			time.c

OBJDIR	:=	obj
OBJECTS	:=	$(addprefix $(OBJDIR)/, $(SRCS:.c=.o))

LIBFT 	:= ./lib/libft/libft.a
MLX42	:= ./lib/MLX42/build/libmlx42.a

CC		:= cc

all: $(NAME)

$(LIBFT):
	make -C ./lib/libft

$(MLX42):
	cmake $(LIBMLX) -B $(LIBMLX)/build && make -C $(LIBMLX)/build -j4

$(OBJDIR)/%.o: %.c ./inc/cub3d.h ./inc/geometry.h
	@mkdir -p $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(NAME): $(OBJECTS) $(LIBFT) $(MLX42)
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

NAME	:= cub3D

CFLAGS	:= -Wextra -Wall -Werror -g -fsanitize=address -O1

LIBMLX	:= ./lib/MLX42

HEADERS	:= -I ./inc -I $(LIBMLX)/include -I ./lib/libft

LIBS	:= $(LIBMLX)/build/libmlx42.a
ifeq ($(shell uname),Darwin)
	LIBS += $(LIBMLX)/build/libmlx42.a -framework Cocoa -framework OpenGL -framework IOKit -lglfw
else ifeq ($(shell uname),Linux)
	LIBS += $(LIBMLX)/build/libmlx42.a -Iinc -Ilib/libft -ldl -lglfw -pthread -lm
endif
# LIBS	+= -L"/opt/homebrew/Cellar/glfw/3.4/lib/"

VPATH	:=	src \
			src/geometry \
			src/loop \
			src/map \
			src/hooks \
			src/player \
			src/3dview \
			src/utils \
			src/parsing

SRCS	:=  main.c \
			main1.c \
			loop.c \
			loop1.c \
			loop2.c \
			initMap.c \
			showMap.c \
			key.c \
			player.c \
			player1.c \
			player2.c \
			geometry.c \
			geometry1.c \
			geometry2.c \
			geometry3.c \
			resize.c \
			cursor.c \
			image.c \
			image1.c \
			image2.c \
			door.c \
			door1.c \
			time.c \
			parse.c \
			parse2.c \
			validate_file.c \
			validate_map.c \
			parse_utils.c \
			parse_utils2.c \
			validate_color.c \
			map_size.c \
			add_segments.c \
			add_segments_utils.c \
			add_segments_utils2.c \
			add_segments_utils3.c \

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

re: fclean all

.PHONY: all, clean, fclean, re, libmlx


# export DISPLAY=:99 && Xvfb :99 -screen 0 1024x768x16 & vgf ./cub3D test.cub & sleep 1 && xdotool search --name "cub3D" key w && sleep 1 && xdotool search --name "cub3D" key right && sleep 1 && xdotool search --name "cub3D" key escape
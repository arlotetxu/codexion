# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: joflorid <joflorid@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/09/22 11:11:03 by joflorid          #+#    #+#              #
#    Updated: 2026/04/24 12:37:24 by joflorid         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

define HEADER


 ██████╗ ██████╗ ██████╗ ███████╗██╗  ██╗██╗ ██████╗ ███╗   ██╗
██╔════╝██╔═══██╗██╔══██╗██╔════╝╚██╗██╔╝██║██╔═══██╗████╗  ██║
██║     ██║   ██║██║  ██║█████╗   ╚███╔╝ ██║██║   ██║██╔██╗ ██║
██║     ██║   ██║██║  ██║██╔══╝   ██╔██╗ ██║██║   ██║██║╚██╗██║
╚██████╗╚██████╔╝██████╔╝███████╗██╔╝ ██╗██║╚██████╔╝██║ ╚████║
 ╚═════╝ ╚═════╝ ╚═════╝ ╚══════╝╚═╝  ╚═╝╚═╝ ╚═════╝ ╚═╝  ╚═══╝



endef
export HEADER

#=======================DEFINITIONS======================
DEF_COLOR 	= \033[0;39m
GRAY 		= \033[0;90m
RED 		= \033[0;31m
GREEN 		= \033[0;92m
YELLOW 		= \033[0;93m
BLUE 		= \033[0;94m
MAGENTA 	= \033[0;95m
CYAN 		= \033[0;96m
WHITE 		= \033[0;97m

AR = ar

AFLAGS = rcs

CC = cc

CFLAGS = -Wall -Wextra -Werror -pthread

#=======================FILES======================
NAME = codexion

SRC_DIR = ./src

SRC = aux2.c create_thr.c one_coder.c prints.c watcher.c aux.c heap_op.c \
parser2.c routine.c compiling.c init_data.c parser.c take_dongles.c \
codexion.c

OBJ_DIR = ./obj

OBJS = $(addprefix $(OBJ_DIR)/, $(SRC:.c=.o))

H_FILES = ./inc/codexion.h

#=========================TARGETS========================
all: $(NAME)
	@echo "$(GREEN)✓ Everything is up to date!$(DEF_COLOR)"

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c $(H_FILES) | $(OBJ_DIR)
	@$(CC) $(CFLAGS) -c $< -o $@

$(NAME): $(OBJS)
	@$(CC) $(CFLAGS) $(OBJS) -o $(NAME)
	@echo "$(CYAN)$$HEADER$(DEF_COLOR)"
	@echo "$(GREEN) Compilation process finished successly!!$(DEF_COLOR)"

clean:
	@echo "$(CYAN)Removing source files...$(DEF_COLOR)"
	rm -rf $(OBJS)
	rm -rf $(OBJ_DIR)

fclean: clean
	@echo "$(CYAN)Removing $(NAME) ...file$(DEF_COLOR)"
	rm -rf $(NAME)

re: fclean all

.PHONY: all clean fclean re

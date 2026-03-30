# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: joflorid <joflorid@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/09/22 11:11:03 by joflorid          #+#    #+#              #
#    Updated: 2026/03/30 10:58:39 by joflorid         ###   ########.fr        #
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

CFLAGS = -Wall -Wextra -Werror

#=======================FILES======================
#NAME = $(SRC_DIR)/push_swap
NAME = codexion

SRC_DIR = ./src

SRC = ps_algorithm_3_2.c ps_index_target_a.c ps_initial_checks.c \
	ps_last_sort.c ps_load_stack_a.c ps_mov_a2b2a.c ps_moves_1.c ps_moves_2.c \
	ps_moves_a.c ps_moves_b.c ps_target_b.c ps_utils_1.c ps_utils_2.c \
	push_swap.c


OBJ_DIR = ./src/obj

OBJS = $(addprefix $(OBJ_DIR)/, $(SRC:.c=.o))

#OBJS_BONUS = $(addprefix $(OBJ_DIR)/, $(SRC_BONUS:.c=.o))

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

.PHONY: all libft_make clean libft_clean fclean libft_fclean re bonus

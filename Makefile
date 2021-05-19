# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: aborboll <aborboll@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2020/02/24 15:33:18 by aborboll          #+#    #+#              #
#    Updated: 2021/05/19 18:29:25 by aborboll         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

#-fsanitize=address
NAME				=	minishell.a
BONUS				=	minishell_bonus
OUTPUT				=	minishell
PID					=	.pid

CC					=	@gcc
SH					=	@bash
RM					=	@/bin/rm -rf
FLAGS				=	-Wextra -Wall -g -fsanitize=address

HEADER_DIR			=	includes/
OBJ_DIR				=	obj/
BUILTINS_DIR		=	builtins/
LISTS_DIR			=	lists/
UTILS_DIR			=	utils/
SRC_DIR				=	srcs/
BONUS_DIR			=	bonus/
LIBFT_DIR			=	libft/
ifeq ($(shell whoami), runner)
	COVID_NORM		=	ruby ~/.norminette/norminette.rb
else
	COVID_NORM		=	ruby -e STDOUT.sync=true -e 'load($$0=ARGV.shift)' ~/.norminette/norminette.rb
endif
RESULT				=	$(shell cat output.txt)
AUTHOR				=	$(shell cat author)
LAST_COMMIT_DATE	=	$(shell git log -1 --date=format:"%m/%d/%Y" --format="%ad   [%cr]")
LAST_COMMIT_HASH	=	$(shell git log -1 --date=format:"%m/%d/%y %T" --format="%H")
LAST_COMMIT_MESSAGE	=	$(shell git log -1 --date=format:"%m/%d/%y %T" --format=\'%s\')
OS					=	$(shell lsb_release -si)
USER				=	$(shell whoami)
ARCH				=	$(shell uname -m | sed 's/x86_//;s/i[3-6]86/32/')
VER					=	$(shell lsb_release -sr)

# Mandatory part

SRCS				=	init.c

BUILTINS			=	builtins/echo.c		builtins/pwd.c	builtins/env.c	builtins/cd.c	builtins/export.c \
						builtins/unset.c

UTILS				=	utils/builtins.c	utils/file.c	utils/signals.c	utils/sys.c

LISTS				=	lists/ft_slstadd_back.c		lists/ft_slstadd_front.c	lists/ft_slstclear.c	lists/ft_slstdelone.c	lists/ft_slstiter.c \
						lists/ft_slstlast.c			lists/ft_slstnew.c			lists/ft_slstsize.c

SOURCES				=	$(SRCS) $(BUILTINS) $(UTILS) $(LISTS)

# Bonus part


B_SRCS				=

BONUS_SOURCES		=	$(B_SRCS)

LEAKS_FLAGS			=	--tool=memcheck --leak-check=full --leak-resolution=high --show-leak-kinds=all --track-origins=yes

ifndef MAP
	LEAKS_EXE		=	./tools/memory_leak.sh ${OUTPUT} maps/map.cub ${LEAKS_FLAGS}
else
	LEAKS_EXE		=	./tools/memory_leak.sh ${OUTPUT} $(MAP) ${LEAKS_FLAGS}
endif
NORME				=	$(addsuffix *.h,$(HEADER_DIR)) \
						$(addprefix $(SRC_DIR),$(SOURCES)) \
						$(addprefix $(BONUS_DIR),$(BONUS_SOURCES)) \
						$(addsuffix **/*.h,$(LIBFT_DIR)) \
						$(addsuffix **/*.c,$(LIBFT_DIR)) \
						$(addsuffix **/**/*.c,$(LIBFT_DIR))


# Mandatory Objects
OFILE				=	$(SOURCES:%.c=%.o)
OBJS				=	$(addprefix $(OBJ_DIR), $(OFILE))
# Bonus Objects
BONUS_OFILE			=	$(BONUS_SOURCES:%.c=%.o)
BONUS_OBJS			=	$(addprefix $(OBJ_DIR), $(BONUS_OFILE))

# Functions
disp_indent			=	for I in `seq 1 $(MAKELEVEL)`; do \
						test "$(MAKELEVEL)" '!=' '0' && printf "\t"; \
						done

exec_color			=	$(call disp_indent); \
						echo $(1)$(2) $(3) $(4) $(5) $(6) $(7) $(8) $(9) $(10)"\033[31m"; \
						$(2) $(3) $(4) $(5) $(6) $(7) $(8) $(9) $(10)

disp_title			=	$(call disp_indent); \
						echo "$(2)[  $(1) \#$(MAKELEVEL)  ]\033[0m"


################
##   COLORS   ##
################

Y					=	$(shell printf "\033[33m")
R					=	$(shell printf "\033[31m")
G					=	$(shell printf "\033[32m")
CYAN				=	$(shell printf "\033[36m")
B					=	$(shell printf "\033[34m")
X					=	$(shell printf "\033[0m")
UP					=	$(shell printf "\033[A")
CUT					=	$(shell printf "\033[K")
W					=	$(shell printf "\033[37m")
UND					=	$(shell printf "\033[4m")
BLINK				=	$(shell printf "\033[5m")
BOLD				=	$(shell printf "\033[1m")
UP					=	$(shell printf "\033[5A")
NORM_COLOR_T		=	$(shell printf "\033[0;1;141m")
NORM_COLOR			=	$(shell printf "\033[0;38;5;153m")
NORM_COLOR_WAR		=	$(shell printf "\033[38;5;214m")
NORM_COLOR_ERR		=	$(shell printf "\033[0;41m")
BG_X				=	$(shell printf "\033[0;39m")

# Make all files.
all:
			@make $(NAME)

# Objects directory
$(OBJ_DIR):
			@make -C $(LIBFT_DIR)
			@echo ${CUT}[${Y}$(OUTPUT)]${X} ${B}Creating: ${R}$(OBJ_DIR)${X}
			@mkdir -p $(OBJ_DIR)
			@mkdir -p $(OBJ_DIR)/$(BUILTINS_DIR)
			@mkdir -p $(OBJ_DIR)/$(UTILS_DIR)
			@mkdir -p $(OBJ_DIR)/$(LISTS_DIR)

# Normal objects
$(NAME): $(OBJ_DIR) $(OBJS)
			@echo ${B}[-----------------------------------------]${X}
			@echo ${B}[------------ ${BOLD}OK${X}${B} - ${BOLD}$(NAME)${X}${B} -----------]${X}
			@echo ${B}[----- 🦑 Iä, iä, Cthulhu fhtagn! 🦑 -----]${X}
			@echo ${B}[-----------------------------------------]${X}

$(OBJS):
			@printf '${CUT}[${Y}$(OUTPUT)]${X} ${B}Compiling: ${X}'
			@make $(OFILE)
			@echo ${B}Crafting: ${R}$(OUTPUT)${X}
			$(CC) main.c $(OBJS) $(LIBFT_DIR)libft.a -I $(HEADER_DIR) $(FLAGS) -o $(OUTPUT)
			@echo

$(OFILE):
			@printf ${R}'$(@:%.o=%.c) '${X}
			$(CC) -o $(OBJ_DIR)$@ -I$(HEADER_DIR) -c $(SRC_DIR)$(@:%.o=%.c) $(FLAGS)

# Bonus objects
$(BONUS): $(OBJ_DIR) $(BONUS_OBJS) $(CRAFT)
			@echo ${B}[-----------------------------------------]${X}
			@echo ${B}[------------ ${BOLD}OK${X}${B} - ${BOLD}$(NAME)${X}${B} -----------]${X}
			@echo ${B}[----- 🦑 Iä, iä, Cthulhu fhtagn! 🦑 -----]${X}
			@echo ${B}[-----------------------------------------]${X}

$(BONUS_OBJS):
			@printf '${CUT}[${Y}$(OUTPUT)]${X} ${B}Compiling: ${X}'
			@make $(BONUS_OFILE)
			@echo ${B}Crafting: ${R}$(OUTPUT)${X}
			$(CC) main.c $(BONUS_OBJS) $(LIBFT_DIR)libft.a -I $(HEADER_DIR) $(FLAGS) -o $(OUTPUT)

$(BONUS_OFILE):
			@printf ${R}'$(@:%.o=%.c) '${X}
			$(CC) -o $(OBJ_DIR)$@ -I$(HEADER_DIR) -c $(BONUS_DIR)$(@:%.o=%.c) $(FLAGS)

##@ Cleaning
clean:		## Clean all objects.
			$(RM) $(OBJ_DIR)


fclean:		## Remove all objects and executables.
			$(RM) $(OUTPUT)
			$(RM) $(OBJ_DIR)
			$(RM) $(NAME)
			@echo ${CUT}[${Y}$(OUTPUT)]${X} [Garbage collector:${Y} ${OUTPUT} $(NAME) *.o${X}][${G}OK!${X}]
			@make fclean -C $(LIBFT_DIR)

##@ Checks
normi:		## Check norminette.
			@$(call disp_title,Checking norminette,$(NORM_COLOR_T))
			@if [ ${OS} = "Ubuntu" ]; then\
				norminette $(NORME) | sed "s/Norme/$(NORM_COLOR_T)➤  $(NORM_COLOR)Norme/g;s/Warning/\t $(NORM_COLOR_WAR)Warning/g;s/Error/\t🚨 $(NORM_COLOR_ERR)Error/gm;s/$$/$(X)/g"; \
			else\
				norminette $(NORME) | sed "s/Norme/$(NORM_COLOR_T)➤  $(NORM_COLOR)Norme/g;s/Warning/\t $(NORM_COLOR_WAR)Warning/g;s/Error/\t🚨 $(NORM_COLOR_ERR)Error/g"; \
			fi

##@ Compilation
bonus:		## Make bonus
			@make clean
			@make $(BONUS)

re:			## Call fclean => all
			@make clean
			@make all

##@ Testing
testback:		## Make minishell test
			cd ./tools/tester && ./test_map_valid_function.sh -f

test:		## Make minishell test
			cd ./minishell-tester && ./destroyer.sh --process 3

leak:		## Run memory leak for valid cub file.
			@if [ $(shell ./tools/memory_leak.sh $(OUTPUT) $(LEAKS_FLAGS) && cat valgrind_out | grep "definitely lost:" | cut -d : -f 2 | cut -d b  -f 1 | tr -d " " | tr -d ",") ]; then\
				echo ${BOLD}${UND}${R}🚨 Memory leaks detected${X};\
				if [ $(USER) = "runner" ]; then\
					cat valgrind_out;\
				else\
					echo ${G};cat valgrind_out | grep -A2 "HEAP SUMMARY:" | cut -d = -f 5 | cut -c 2-;\
					echo ${CYAN};cat valgrind_out | grep -A5 "LEAK SUMMARY:" | cut -d = -f 5 | cut -c 2-;\
				fi;\
				exit 1;\
			else\
				echo ☕ ${BOLD}${UND}${G}No memory leaks detected${X} ☕;\
				echo ${CYAN};cat valgrind_out | grep -A4 "HEAP SUMMARY:" | cut -d = -f 5 | cut -c 2-;\
			fi

##@ Helpers
kill:		## Kill all minishells
			@pkill -f $(OUTPUT)
##@ Help
help:		## View all available commands.
			$(shell $(TARGETS_EXE))
			@echo ${CYAN} Proyecto: ${UND}${BOLD}$(OUTPUT)${CYAN}${X}${CYAN}${UND}\\t\\t\\t\\t\\tAuthor: $(AUTHOR)${X}
			@echo ${CYAN} Last commit:${X}
			@echo ${CYAN} \\tDate: $(LAST_COMMIT_DATE)
			@echo ${CYAN} \\tHash: $(LAST_COMMIT_HASH)${X}
			@echo ${CYAN} \\tMessage: $(LAST_COMMIT_MESSAGE)${X}
			@echo ${CYAN}--------------------------------------------------------------------------
			@echo ${CYAN}Available commands: "\\t\\t\\t\\t(${G}Usage:${X} make ${CYAN}<target>${X})"
			@awk 'BEGIN {FS = ":.*##"; printf ""} /^[a-zA-Z_-]+:.*?##/ { printf "  ${CYAN}%-25s${X} %s\n", $$1, $$2 } /^##@/ { printf "\n${UND}${BOLD}%s${X}\n", substr($$0, 5) } ' $(MAKEFILE_LIST)
			@echo ${CYAN}--------------------------------------------------------------------------

.PHONY:		all clean flclean re bonus test
